#pragma once
#include <list>
#include <vector>

#define VERTEX_CLUSTER_JOINING 0
#define CLUSTER_CLUSTER_JOINING 1

class Graph {
    public:
        Graph();
        Graph(int __num_vertices);
        Graph(int __num_vertices, std::vector<std::tuple<int,int,double>> __edges);
        Graph(std::string filepath);

        void add_edge(int u, int v, double w);
        void process_edge(int u, int v, double w);

        int order();
        int size();
        void print_adjacency_list();

        Graph spanner(int k, int joining_method);
        Graph three_spanner();
        std::vector<std::vector<double>> floyd_warshall();
        std::vector<std::vector<double>> dijkstra_apsp();
        std::vector<double> dijkstra_sssp(int u);

    private:
        int num_vertices;
        int num_edges;
        std::vector<std::list<std::pair<int,int>>> adj;
        std::vector<double> weight;
};

void floyd_warshall_gpu(const double *graph, int num_vertices, double *output);
