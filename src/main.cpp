#include <graph.hpp>
#include <util.hpp>
#include <iostream>
#include <chrono>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <graph.mtx>" << std::endl;
        return 0;
    }

    Graph G(argv[1]);

    auto start = std::chrono::high_resolution_clock::now();

    Graph S = G.spanner(3);

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = end - start;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
    std::cout << "Execution time: " << milliseconds.count() << " ms" << std::endl;

    std::vector<std::vector<double>> dist_original = G.floyd_warshall();
    std::vector<std::vector<double>> dist_stretch = S.floyd_warshall();
    double t = assert_stretch_bound(dist_original, dist_stretch, 3);
    std::cout << "Stretch factor: " << t << std::endl;

    return 0;
}
