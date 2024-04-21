#pragma once
#include <vector>
#include <cstdlib>

double assert_stretch_bound(std::vector<std::vector<double>> dist_original, std::vector<std::vector<double>> dist_stretch, int stretch_factor);
void print_matrix(std::vector<std::vector<double>> matrix);

inline double random_number() {
    return (double) (rand() / (RAND_MAX + 1.0));
}
