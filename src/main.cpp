#include <graph.hpp>
#include <util.hpp>
#include <chrono>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr << "usage: " << argv[0] << " <graph.mtx> <t>" << std::endl;
        return 0;
    }

    std::string filepath = argv[1];
    int t = atoi(argv[2]);

    Graph G(filepath);

    auto start = std::chrono::high_resolution_clock::now();

    Graph S = G.spanner(t);
    std::cout << S.order() << " " << S.size() << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = end - start;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
    std::cout << "Execution time: " << milliseconds.count() << " ms" << std::endl;

    std::vector<std::vector<double>> dist_original = G.floyd_warshall();
    std::vector<std::vector<double>> dist_stretch = S.floyd_warshall();
    double stretch = assert_stretch_bound(dist_original, dist_stretch, (double) 2 * t - 1);
    std::cout << "Stretch factor: " << stretch << std::endl;

    return 0;
}
