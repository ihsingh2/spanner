#include <graph.hpp>
#include <util.hpp>
#include <iostream>
#include <string>

#ifdef TIMING
#include <chrono>
#endif

int usage(char* argv[]) {
    std::cerr << "usage: " << argv[0] << " <graph.mtx> <algorithm> [<k>]" << std::endl;
    std::cerr << "algorithm:" << std::endl;
    std::cerr << "    0: 3-spanner" << std::endl;
    std::cerr << "    1: (2k - 1)-spanner (vertex-cluster joining)" << std::endl;
    std::cerr << "    2: (2k - 1)-spanner (cluster-cluster joining)" << std::endl;
    return 1;
}

int main(int argc, char* argv[])
{
    if (argc < 3)
        return usage(argv);

    std::string filepath = argv[1];
    int alg = atoi(argv[2]);
    int t = 2;
    if (alg > 0) {
        if (argc < 4)
            return usage(argv);
        t = atoi(argv[3]);
    }

    Graph G(filepath);
    Graph S;

#ifdef TIMING
    auto start = std::chrono::high_resolution_clock::now();
#endif

    switch(alg) {
        case 0:
            S = G.three_spanner();
            break;
        case 1:
            S = G.spanner(t, VERTEX_CLUSTER_JOINING);
            break;
        case 2:
            S = G.spanner(t, CLUSTER_CLUSTER_JOINING);
            break;
        default:
            throw std::invalid_argument("Invalid argument for algorithm");
    }

#ifdef TIMING
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = end - start;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
    std::cout << "Total execution time: " << milliseconds.count() << " ms" << std::endl;
#endif

    std::cout << "|V(S)| = " << S.order() << ", |E(S)| = " << S.size() << std::endl;

#ifdef VALIDATION
    std::vector<std::vector<double>> dist_original = G.floyd_warshall();
    std::vector<std::vector<double>> dist_stretch = S.floyd_warshall();
    double stretch = assert_stretch_bound(dist_original, dist_stretch, (double) 2 * t - 1);
    std::cout << "Stretch factor: " << stretch << std::endl;
#endif

    return 0;
}
