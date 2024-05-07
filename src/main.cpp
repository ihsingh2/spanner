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
    int k = 2;
    if (alg > 0) {
        if (argc < 4)
            return usage(argv);
        k = atoi(argv[3]);
    }

    Graph G(filepath);
    Graph S;

    switch(alg) {
        case 0:
            S = G.three_spanner();
            break;
        case 1:
            S = G.spanner(k, VERTEX_CLUSTER_JOINING);
            break;
        case 2:
            S = G.spanner(k, CLUSTER_CLUSTER_JOINING);
            break;
        default:
            throw std::invalid_argument("Invalid argument for algorithm");
    }

    std::cout << "|V| = " << S.order() << std::endl;
    std::cout << "|E(G)| = " << G.size() << std::endl;
    std::cout << "|E(S)| = " << S.size() << std::endl;

#ifdef VALIDATION
    std::vector<std::vector<double>> dist_original = G.floyd_warshall();
    std::vector<std::vector<double>> dist_stretch = S.floyd_warshall();
    double stretch = assert_stretch_bound(dist_original, dist_stretch, (double) 2 * k - 1);
    std::cout << "Stretch factor: " << stretch << std::endl;
#endif

    return 0;
}
