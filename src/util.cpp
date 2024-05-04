#include <util.hpp>
#include <iostream>
#include <limits>
#include <string>

void assert(std::string name, bool cond) {
    if (!cond) {
        std::cout << "Assertion Failed: " << name << std::endl;
        exit(1);
    }
}

double assert_stretch_bound(std::vector<std::vector<double>> dist1, std::vector<std::vector<double>> dist2, double max_stretch) {
    double stretch = std::numeric_limits<double>::min();
    assert("num_vertices", dist1.size() == dist2.size());
    for (int i = 0; i < dist1.size(); i++) {
        assert("num_vertices", dist1[i].size() == dist2[i].size());
        for (int j = 0; j < dist1[i].size(); j++) {
            if (dist1[i][j] != 0) {
                stretch = std::max(stretch, dist2[i][j] / dist1[i][j]);
            }
            assert("stretch_factor", stretch <= max_stretch);
        }
    }
    return stretch;
}

void print_matrix(std::vector<std::vector<double>> matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
