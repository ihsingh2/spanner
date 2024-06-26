#include <algorithm>
#include <limits>
// #include <omp.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <graph.hpp>
#include <minheap.hpp>

Graph::Graph() {
    num_vertices = 0;
    num_edges = 0;
    adj.resize(0);
    weight.resize(0);
}

Graph::Graph(int __num_vertices) {
    num_vertices = __num_vertices;
    num_edges = 0;
    adj.resize(num_vertices);
    weight.resize(0);
}

Graph::Graph(int __num_vertices, std::vector<std::tuple<int,int,double>> __edges) {
    num_vertices = __num_vertices;
    num_edges = 0;
    adj.resize(num_vertices);
    weight.resize(0);
    for (auto edge: __edges) {
        int u = std::get<0>(edge);
        int v = std::get<1>(edge);
        double w = std::get<2>(edge);
        process_edge(u, v, w);
    }
}

Graph::Graph(std::string filepath) {
    std::ifstream file(filepath);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line[0] == '%')
                continue;
            else {
                std::stringstream ss(line);
                ss >> num_vertices;
                num_edges = 0;
                adj.resize(num_vertices);
                weight.resize(0);
                break;
            }
        }
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            int u, v;
            double w;
            ss >> u >> v >> w;
            u--; v--;
            process_edge(u, v, w);
        }
        file.close();
    }
    else {
        throw std::invalid_argument("File not found");
    }
}

void Graph::add_edge(int u, int v, double w) {
    int idx = num_edges;
    auto pos = std::find_if(adj[u].begin(), adj[u].end(),
                            [v](std::pair<int,int> const &b) { 
                                return b.first == v; 
                            });

    if (pos == adj[u].end()) {
        adj[u].push_back({v, idx});
        adj[v].push_back({u, idx});
        weight.push_back(w);
        num_edges++;
    }
}

void Graph::process_edge(int u, int v, double w) {
    if (w < 0) {
        throw std::invalid_argument("Found negative weight edge in the graph");
    }
    if (u < 0 || num_vertices <= u || v < 0 || num_vertices <= v) {
        throw std::invalid_argument("Vertex index out of range");
    }
    if (u != v) {
        add_edge(u, v, w);
    }
}

int Graph::order() {
    return num_vertices;
}

int Graph::size() {
    return num_edges;
}

void Graph::print_adjacency_list() {
    for (int u = 0; u < num_vertices; u++) {
        std::cout << u << ": ";
        if (adj[u].size() > 0) {
            auto it = adj[u].begin();
            while (true) {
                int v = it->first;
                int idx = it->second;
                double w = weight[idx];
                std::cout << "(" << v << "," << w << ")";
                if (++it == adj[u].end())
                    break;
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<double>> Graph::floyd_warshall() {
    std::vector<std::vector<double>> dist(num_vertices, std::vector<double> (num_vertices, std::numeric_limits<double>::max()));
    for (int u = 0; u < num_vertices; u++) {
        dist[u][u] = 0;
        for (auto [v, idx]: adj[u]) {
            double w = weight[idx];
            dist[u][v] = w;
            dist[v][u] = w;
        }
    }

    /* for (int k = 0; k < num_vertices; k++) {
        for (int i = 0; i < num_vertices; i++) {
            for (int j = 0; j < num_vertices; j++) {
                dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    return dist; */

    double size = sizeof(double) * num_vertices * num_vertices;
    double *graph = (double *) malloc(size);
    double *output = (double *) malloc(size);

    for (int u = 0; u < num_vertices; u++) {
        for (int v = 0; v < num_vertices; v++) {
            graph[u * num_vertices + v] = dist[u][v];
        }
    }

    floyd_warshall_gpu(graph, num_vertices, output);

    for (int u = 0; u < num_vertices; u++) {
        for (int v = 0; v < num_vertices; v++) {
            dist[u][v] = output[u * num_vertices + v];
        }
    }

    return dist;
}

std::vector<std::vector<double>> Graph::dijkstra_apsp() {
    std::vector<std::vector<double>> distance(num_vertices);
    // #pragma omp parallel for
    for (int u = 0; u < num_vertices; u++) {
        distance[u] = dijkstra_sssp(u);
    }
    return distance;
}

std::vector<double> Graph::dijkstra_sssp(int u) {
    std::vector<double> distance(num_vertices, std::numeric_limits<double>::max());
    std::vector<bool> inqueue(num_vertices, true);

    std::vector<std::pair<double,int>> keys(num_vertices);
    for (int i = 0; i < num_vertices; i++)
        keys[i] = std::make_pair(std::numeric_limits<double>::max(), i);
    keys[u].first = 0;

    MinHeap Q(keys);
    while (!Q.empty()) {
        std::pair<double,int> m = Q.pop();
        int u = m.second;
        distance[u] = m.first;
        inqueue[u] = false;
        if (distance[u] == std::numeric_limits<double>::max())
            break;

        for (auto [v, idx]: adj[u]) {
            double w = weight[idx];
            if (inqueue[v]) {
                if (Q.peek(v) > distance[u] + w) {
                    Q.update(v, distance[u] + w);
                }
            }
        }
    }
    return distance;
}
