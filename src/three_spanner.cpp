#include <cmath>
#include <ctime>
#include <limits>
#include <unordered_map>
#include <iostream>
#include <graph.hpp>
#include <util.hpp>

#ifdef TIMING
#include <chrono>
#define clock() std::chrono::high_resolution_clock::now()
#define clock_t std::chrono::time_point<std::chrono::high_resolution_clock>
#endif

#define UNCLUSTERED -1
#define IS_CLUSTERED(u) C[u] != UNCLUSTERED
#define IS_UNCLUSTERED(u) C[u] == UNCLUSTERED
#define IS_CLUSTER_CENTER(u) C[u] == u
#define IN_SAME_CLUSTER(u, v) C[u] == C[v] && IS_CLUSTERED(u)
#define IN_DIFF_CLUSTER(u, v) C[u] != C[v] && IS_CLUSTERED(u) && IS_CLUSTERED(v)

Graph Graph::three_spanner() {
    Graph S(num_vertices);
    std::vector<int> C(num_vertices, UNCLUSTERED);
    std::vector<std::list<std::pair<int,double>>> edges(num_vertices);

#ifdef TIMING
    std::vector<std::string> steps({
        "Sampling cluster centres",
        "Formation of clusters",
        "Segregating inter-cluster edges",
        "Joining vertices with neighboring clusters"
    });
    std::vector<clock_t> timestamps;
    std::vector<int64_t> differences(steps.size(), 0);
    timestamps.push_back(clock());
#endif

    // sample cluster centres
    srand(time(NULL));
    double prob = 1 / std::sqrt(num_vertices);
    for (int u = 0; u < num_vertices; u++) {
        double uni = random_number();
        if (uni < prob) {
            C[u] = u;
        }
    }

#ifdef TIMING
    timestamps.push_back(clock());
#endif

    // form clusters
    for (int u = 0; u < num_vertices; u++) {
        if (IS_UNCLUSTERED(u)) {
            double w_min = std::numeric_limits<double>::max();
            for (auto [v, idx]: adj[u]) {
                double w = weight[idx];
                if (IS_CLUSTER_CENTER(v) && w < w_min) {
                    C[u] = v;
                    w_min = w;
                }
            }
            if (C[u] != -1) {
                S.add_edge(u, C[u], w_min);
                for (auto [v, idx]: adj[u]) {
                    double w = weight[idx];
                    if (w < w_min) {
                        S.add_edge(u, v, w);
                    }
                }
            }
            else {
                for (auto [v, idx]: adj[u]) {
                    double w = weight[idx];
                    S.add_edge(u, v, w);
                }
            }
        }
    }

#ifdef TIMING
    timestamps.push_back(clock());
#endif

    // segregate inter-cluster edges
    for (int u = 0; u < num_vertices; u++) {
        for (auto [v, idx]: adj[u]) {
            double w = weight[idx];
            if (IN_DIFF_CLUSTER(u, v)) {
                edges[u].push_back({v, w});
                edges[v].push_back({u, w});
            }
        }
    }

#ifdef TIMING
    timestamps.push_back(clock());
#endif

    // join vertices with neighboring clusters
    for (int u = 0; u < num_vertices; u++) {
        std::unordered_map<int,std::pair<int,double>> A;
        for (auto [v, w]: edges[u]) {
            int x = C[v];
            auto it = A.find(x);
            if (it == A.end() || it->second.second > w) {
                A[x] = {v, w};
            }
        }
        for (auto [key, value]: A) {
            S.add_edge(u, value.first, value.second);
        }
    }

#ifdef TIMING
    timestamps.push_back(clock());
#endif

#ifdef TIMING
    for (int i = 1; i < timestamps.size(); i++) {
        auto elapsed = timestamps[i] - timestamps[i - 1];
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
        differences[i - 1] = milliseconds.count();
    }
    int64_t total = 0;
    for (int i = 0; i < differences.size(); i++) {
        total += differences[i];
        std::cout << steps[i] << ": " << differences[i] << " ms" << std::endl;
    }
    std::cout << "Total execution time: " << total << " ms" << std::endl;
#endif

    return S;
}
