#include <graph.hpp>
#include <util.hpp>
#include <cmath>
#include <ctime>
#include <limits>
#include <unordered_map>

#define UNCLUSTERED -1
#define IS_CLUSTERED(u) C[u] != UNCLUSTERED
#define IS_UNCLUSTERED(u) C[u] == UNCLUSTERED
#define IS_CLUSTER_CENTER(u) C[u] == u
#define IN_SAME_CLUSTER(u, v) C[u] == C[v] && IS_CLUSTERED(u)
#define IN_DIFF_CLUSTER(u, v) C[u] != C[v] && IS_CLUSTERED(u) && IS_CLUSTERED(v)

Graph Graph::three_spanner() {
    Graph S(num_vertices);
    std::vector<int> C(num_vertices, UNCLUSTERED);
    std::vector<std::set<std::pair<int,double>>> edges(num_vertices);

    // sample cluster centres
    srand(time(NULL));
    double prob = 1 / std::sqrt(num_vertices);
    for (int u = 0; u < num_vertices; u++) {
        double uni = random_number();
        if (uni < prob) {
            C[u] = u;
        }
    }

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

    // segregate inter-cluster edges
    for (int u = 0; u < num_vertices; u++) {
        for (auto [v, idx]: adj[u]) {
            double w = weight[idx];
            if (IN_DIFF_CLUSTER(u, v)) {
                edges[u].insert({v, w});
                edges[v].insert({u, w});
            }
        }
    }

    // join vertices with neighbouring clusters
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

    return S;
}
