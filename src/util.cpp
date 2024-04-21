#include <util.hpp>
#include <iostream>
#include <cassert>
#include <limits>

double assert_stretch_bound(std::vector<std::vector<double>> dist1, std::vector<std::vector<double>> dist2, int max_stretch) {
    double stretch = std::numeric_limits<double>::min();
    assert(dist1.size() == dist2.size());
    for (int i = 0; i < dist1.size(); i++) {
        assert(dist1[i].size() == dist2[i].size());
        for (int j = 0; j < dist1[i].size(); j++) {
            assert(max_stretch * dist1[i][j] >= dist2[i][j]);
            stretch = std::max(stretch, dist2[i][j] / dist1[i][j]);
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
