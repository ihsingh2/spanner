#include <graph.hpp>
#include <util.hpp>
#include <cmath>
#include <ctime>
#include <limits>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

#ifdef TIMING
#include <chrono>
#define clock() std::chrono::high_resolution_clock::now()
#define clock_t std::chrono::time_point<std::chrono::high_resolution_clock>
#endif

#define UNCLUSTERED -1
#define IS_CLUSTERED(u) C_i[u] != UNCLUSTERED
#define IS_UNCLUSTERED(u) C_i[u] == UNCLUSTERED
#define IS_CLUSTER_CENTER(u) C_i[u] == u
#define IN_SAME_CLUSTER(u, v) C_i[u] == C_i[v] && IS_CLUSTERED(u)
#define IN_DIFF_CLUSTER(u, v) C_i[u] != C_i[v] && IS_CLUSTERED(u) && IS_CLUSTERED(v)
#define IN_V_PRIME(u) edges[u].size() > 0 || E_prev[u].size() > 0
#define WAS_IN_CLUSTER(v, x) C_prev[v] == x
#define WAS_CLUSTER_CENTER(u) WAS_IN_CLUSTER(u, u)

Graph Graph::spanner(int k, int joining_method) {
    if (k <= 0) {
        throw std::invalid_argument("Expected a positive value for argument k.");
    }
    if (k == 1) {
        return *this;
    }

    int num_iterations;
    if (joining_method == VERTEX_CLUSTER_JOINING) {
        num_iterations = k - 1;
    }
    else if (joining_method == CLUSTER_CLUSTER_JOINING) {
        num_iterations = k / 2;
    }
    else {
        throw std::invalid_argument("Joining method not recognized");
    }

    Graph S(num_vertices);
    std::vector<std::set<std::pair<int,int>>> edges = adj;
    std::vector<std::set<std::pair<int,double>>> E_prev(num_vertices);
    std::vector<int> C_prev(num_vertices);
    std::vector<int> C_prev_prev(num_vertices);
    for (int u = 0; u < num_vertices; u++) {
        C_prev[u] = u;
    }

#ifdef TIMING
    std::vector<std::string> steps({
        "Sampling cluster centres",
        "Finding neighbouring clusters",
        "Adding edges to the spanner",
        "Removing intra cluster edges",
        "Cluster joining"
    });
    std::vector<std::vector<clock_t>> timestamps(num_iterations);
    std::vector<int64_t> differences(steps.size(), 0);
#endif

    // I: Forming the clusters
    for (int i = 0; i < num_iterations; i++) {
        std::vector<std::set<std::pair<int,double>>> E_i(num_vertices);
        std::vector<int> C_i(num_vertices, UNCLUSTERED);
        std::vector<bool> removed(num_edges, false);

#ifdef TIMING
        timestamps[i].push_back(clock());
#endif

        // sample cluster centres
        srand(time(NULL));
        double prob = std::pow(num_vertices, - (float) 1 / k);
        for (int u = 0; u < num_vertices; u++) {
            if (WAS_CLUSTER_CENTER(u)) {
                double uni = random_number();
                if (uni < prob) {
                    C_i[u] = u;
                }
            }
        }
        for (int u = 0; u < num_vertices; u++) {
            int x = C_prev[u];
            if (IS_CLUSTER_CENTER(x)) {
                C_i[u] = x;
            }
        }
        for (int u = 0; u < num_vertices; u++) {
            for (auto [v, w]: E_prev[u]) {
                if (IS_CLUSTERED(u) && IS_CLUSTERED(v)) {
                    E_i[u].insert({v, w});
                    E_i[v].insert({u, w});
                }
            }
        }

#ifdef TIMING
        timestamps[i].push_back(clock());
#endif

        // find nearest cluster
        std::vector<int> V_min(num_vertices, -1);
        std::vector<double> W_min(num_vertices, std::numeric_limits<double>::max());
        for (int u = 0; u < num_vertices; u++) {
            if (IN_V_PRIME(u) && IS_UNCLUSTERED(u)) {
                for (auto [v, idx]: edges[u]) {
                    double w = weight[idx];
                    if (IS_CLUSTERED(v) && w < W_min[u]) {
                        V_min[u] = v;
                        W_min[u] = w;
                    }
                }
            }
        }

#ifdef TIMING
        timestamps[i].push_back(clock());
#endif

        // add edges to spanner
        for (int u = 0; u < num_vertices; u++) {
            if (IN_V_PRIME(u) && IS_UNCLUSTERED(u)) {
                double w_e_v;
                if (V_min[u] != -1) {
                    int v_min = V_min[u];
                    double w_min = W_min[u];
                    C_i[u] = C_i[v_min];
                    S.add_edge(u, v_min, w_min);
                    E_i[u].insert({v_min, w_min});
                    E_i[v_min].insert({u, w_min});
                    for (auto [v, idx]: edges[u]) {
                        if (IN_SAME_CLUSTER(v, v_min)) {
                            removed[idx] = true;
                        }
                    }
                    w_e_v = w_min;
                }
                else
                    w_e_v = std::numeric_limits<double>::max();

                std::unordered_map<int,std::pair<int,double>> A;
                for (auto [v, idx]: edges[u]) {
                    double w = weight[idx];
                    int x = C_prev[v];
                    auto it = A.find(x);
                    if (it == A.end() || w < it->second.second) {
                        if (w < w_e_v) {
                            A[x] = {v, w};
                        }
                    }
                }
                for (auto [key, value]: A) {
                    S.add_edge(u, value.first, value.second);
                }
                for (auto [v, idx]: edges[u]) {
                    int x = C_prev[v];
                    if (A.find(x) != A.end()) {
                        removed[idx] = true;
                    }
                }
            }
        }

#ifdef TIMING
        timestamps[i].push_back(clock());
#endif

        // remove intra-cluster edges
        for (int u = 0; u < num_vertices; u++) {
            if (IS_CLUSTERED(u)) {
                for (auto [v, idx]: edges[u]) {
                    if (IN_SAME_CLUSTER(u, v)) {
                        removed[idx] = true;
                    }
                }
            }
        }
        for (int u = 0; u < num_vertices; u++) {
            for (auto it = edges[u].begin(); it != edges[u].end(); ) {
                int idx = it->second;
                if (removed[idx]) {
                    edges[u].erase(it++);
                }
                else {
                    it++;
                }
            }
        }

#ifdef TIMING
        timestamps[i].push_back(clock());
#endif

        E_prev = E_i;
        C_prev_prev = C_prev;
        C_prev = C_i;
    }

#ifdef TIMING
    for (int i = 1; i < steps.size(); i++) {
        for (int j = 0; j < num_iterations; j++) {
            auto elapsed = timestamps[j][i] - timestamps[j][i - 1];
            auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
            differences[i - 1] += milliseconds.count();
        }
    }
#endif

#ifdef TIMING
    timestamps[0][0] = clock();
#endif

    // II: cluster joining
    if (joining_method == VERTEX_CLUSTER_JOINING) {
        for (int u = 0; u < num_vertices; u++) {
            if (IN_V_PRIME(u)) {
                for (int x = 0; x < num_vertices; x++) {
                    if (WAS_CLUSTER_CENTER(x)) {
                        int v_min = -1;
                        double w_min = std::numeric_limits<double>::max();
                        for (auto [v, idx]: edges[u]) {
                            double w = weight[idx];
                            if (WAS_IN_CLUSTER(v, x)) {
                                if (w < w_min) {
                                    v_min = v;
                                    w_min = w;
                                }
                            }
                        }
                        if (v_min != -1) {
                            S.add_edge(u, v_min, w_min);
                        }
                    }
                }
            }
        }
    }
    else if (joining_method == CLUSTER_CLUSTER_JOINING) {
        if (k % 2 == 1) {
            std::vector<int> centres;
            for (int x = 0; x < num_vertices; x++) {
                if (WAS_CLUSTER_CENTER(x)) {
                    centres.push_back(x);
                }
            }
            for (int i = 0; i < centres.size(); i++) {
                int x = centres[i];
                for (int j = i + 1; j < centres.size(); j++) {
                    int y = centres[j];
                    int u_min = -1;
                    int v_min = -1;
                    double w_min = std::numeric_limits<double>::max();
                    for (int u = 0; u < num_vertices; u++) {
                        if (WAS_IN_CLUSTER(u, x)) {
                            for (auto [v, idx]: edges[u]) {
                                double w = weight[idx];
                                if (WAS_IN_CLUSTER(v, y)) {
                                    if (w < w_min) {
                                        u_min = u;
                                        v_min = v;
                                        w_min = w;
                                    }
                                }
                            }
                        }
                    }
                    if (v_min != -1) {
                        S.add_edge(u_min, v_min, w_min);
                    }
                }
            }
        }
        else {
            std::vector<int> centres;
            std::vector<int> centres_old;
            for (int x = 0; x < num_vertices; x++) {
                if (WAS_CLUSTER_CENTER(x)) {
                    centres.push_back(x);
                }
                else if (C_prev_prev[x] = x) {
                    centres_old.push_back(x);
                }
            }
            for (int i = 0; i < centres.size(); i++) {
                int x = centres[i];
                for (int j = 0; j < centres_old.size(); j++) {
                    int y = centres_old[j];
                    int u_min = -1;
                    int v_min = -1;
                    double w_min = std::numeric_limits<double>::max();
                    for (int u = 0; u < num_vertices; u++) {
                        if (WAS_IN_CLUSTER(u, x)) {
                            for (auto [v, idx]: edges[u]) {
                                double w = weight[idx];
                                if (C_prev_prev[v] == y) {
                                    if (w < w_min) {
                                        u_min = u;
                                        v_min = v;
                                        w_min = w;
                                    }
                                }
                            }
                        }
                    }
                    if (v_min != -1) {
                        S.add_edge(u_min, v_min, w_min);
                    }
                }
            }
        }
    }

#ifdef TIMING
    timestamps[0][1] = clock();
    {
        auto elapsed = timestamps[0][1] - timestamps[0][0];
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
        differences[steps.size() - 1] = milliseconds.count();
    }
#endif

#ifdef TIMING
    int64_t total = 0;
    for (int i = 0; i < differences.size(); i++) {
        total += differences[i];
        std::cout << steps[i] << ": " << differences[i] << " ms" << std::endl;
    }
    std::cout << "Total execution time: " << total << " ms" << std::endl;
#endif

    return S;
}
