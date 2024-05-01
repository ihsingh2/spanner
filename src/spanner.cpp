#include <graph.hpp>
#include <util.hpp>
#include <cmath>
#include <ctime>
#include <limits>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

#define UNCLUSTERED -1
#define IS_CLUSTERED(u) C_i[u] != UNCLUSTERED
#define IS_UNCLUSTERED(u) C_i[u] == UNCLUSTERED
#define IS_CLUSTER_CENTER(u) C_i[u] == u
#define IN_SAME_CLUSTER(u, v) C_i[u] == C_i[v] && IS_CLUSTERED(u)
#define IN_DIFF_CLUSTER(u, v) C_i[u] != C_i[v] && IS_CLUSTERED(u) && IS_CLUSTERED(v)
#define IN_V_PRIME(u) edges[u].size() > 0 || E_prev[u].size() > 0
#define WAS_IN_CLUSTER(v, x) C_prev[v] == x
#define WAS_CLUSTER_CENTER(u) WAS_IN_CLUSTER(u, u)

Graph Graph::spanner(int k) {
    if (k <= 0) {
        throw std::invalid_argument("Expected a positive value for argument k.");
    }
    if (k == 1) {
        return *this;
    }

    Graph S(num_vertices);
    std::vector<std::set<std::pair<int,int>>> edges = adj;
    std::vector<std::set<std::pair<int,double>>> E_prev(num_vertices);
    std::vector<int> C_prev(num_vertices);
    for (int u = 0; u < num_vertices; u++) {
        C_prev[u] = u;
    }

    // I: Forming the clusters
    for (int i = 0; i < k - 1; i++) {
        std::vector<std::set<std::pair<int,double>>> E_i(num_vertices);
        std::vector<int> C_i(num_vertices, UNCLUSTERED);
        std::vector<bool> removed(num_edges, false);

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

        // add edges to spanner
        for (int u = 0; u < num_vertices; u++) {
            if (IN_V_PRIME(u) && IS_UNCLUSTERED(u)) {
                if (V_min[u] == -1) {
                    for (int x = 0; x < num_vertices; x++) {
                        if (WAS_CLUSTER_CENTER(x)) {
                            int v_min = -1;
                            double w_min = std::numeric_limits<double>::max();
                            for (auto [v, idx]: edges[u]) {
                                double w = weight[idx];
                                if (WAS_IN_CLUSTER(v, x)) {
                                    removed[idx] = true;
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
                else {
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
                    double w_e_v = w_min;
                    for (int x = 0; x < num_vertices; x++) {
                        if (WAS_CLUSTER_CENTER(x)) {
                            v_min = -1;
                            w_min = std::numeric_limits<double>::max();
                            for (auto [v, idx]: edges[u]) {
                                double w = weight[idx];
                                if (WAS_IN_CLUSTER(v, x)) {
                                    if (w < w_min) {
                                        v_min = v;
                                        w_min = w;
                                    }
                                }
                            }
                            if (v_min != -1 && w_min < w_e_v) {
                                S.add_edge(u, v_min, w_min);
                                for (auto [v, idx]: edges[u]) {
                                    if (WAS_IN_CLUSTER(v, x)) {
                                        removed[idx] = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // remove intra-cluster edges
        for (int u = 0; u < num_vertices; u++) {
            if (IS_CLUSTERED(u)) {
                for (auto [v, idx]: edges[u]) {
                    if (C_i[u] == C_i[v]) {
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

        E_prev = E_i;
        C_prev = C_i;
    }

    // II: Vertex cluster joining
    for (int u = 0; u < num_vertices; u++) {
        if (edges[u].size() > 0) {
            for (int x = 0; x < num_vertices; x++) {
                if (C_prev[x] == x) {
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

    return S;
}