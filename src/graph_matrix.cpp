#include "graph_matrix.hpp"
#include <iostream>
#include <algorithm>
#include <sstream>

GraphMatrix::GraphMatrix(int v, bool directed)
    : vertices(v), adjacency_matrix(v, std::vector<int>(v, INF)), is_directed(directed) {
    // Initialize diagonal to 0 (distance to itself)
    for (int i = 0; i < v; i++) {
        adjacency_matrix[i][i] = 0;
    }
}

GraphMatrix::~GraphMatrix() {}

void GraphMatrix::addEdge(int from, int to, int weight) {
    if (from < 0 || from >= vertices || to < 0 || to >= vertices) {
        return;
    }
    adjacency_matrix[from][to] = weight;
    if (!is_directed) {
        adjacency_matrix[to][from] = weight;
    }
}

void GraphMatrix::removeEdge(int from, int to) {
    if (from < 0 || from >= vertices || to < 0 || to >= vertices) {
        return;
    }
    adjacency_matrix[from][to] = INF;
    if (!is_directed) {
        adjacency_matrix[to][from] = INF;
    }
}

std::vector<int> GraphMatrix::dijkstra(int source) {
    std::vector<int> distance(vertices, INF);
    std::vector<bool> visited(vertices, false);
    
    distance[source] = 0;
    
    for (int count = 0; count < vertices - 1; count++) {
        int u = -1;
        int min_dist = INF;
        
        for (int v = 0; v < vertices; v++) {
            if (!visited[v] && distance[v] < min_dist) {
                min_dist = distance[v];
                u = v;
            }
        }
        
        if (u == -1 || distance[u] == INF) break;
        visited[u] = true;
        
        for (int v = 0; v < vertices; v++) {
            if (!visited[v] && adjacency_matrix[u][v] != INF &&
                distance[u] + adjacency_matrix[u][v] < distance[v]) {
                distance[v] = distance[u] + adjacency_matrix[u][v];
            }
        }
    }
    
    return distance;
}

std::string GraphMatrix::dijkstraVerbose(int source) {
    std::ostringstream oss;
    auto distances = dijkstra(source);
    
    oss << "Dijkstra from vertex " << source << ":\n";
    for (int i = 0; i < vertices; i++) {
        oss << "  To " << i << ": ";
        if (distances[i] == INF) {
            oss << "INF (unreachable)\n";
        } else {
            oss << distances[i] << "\n";
        }
    }
    
    return oss.str();
}

std::pair<bool, std::vector<int>> GraphMatrix::bellmanFord(int source) {
    std::vector<int> distance(vertices, INF);
    distance[source] = 0;
    
    // Relax edges vertices - 1 times
    for (int i = 0; i < vertices - 1; i++) {
        for (int u = 0; u < vertices; u++) {
            if (distance[u] != INF) {
                for (int v = 0; v < vertices; v++) {
                    if (adjacency_matrix[u][v] != INF &&
                        distance[u] + adjacency_matrix[u][v] < distance[v]) {
                        distance[v] = distance[u] + adjacency_matrix[u][v];
                    }
                }
            }
        }
    }
    
    // Check for negative cycles
    for (int u = 0; u < vertices; u++) {
        if (distance[u] != INF) {
            for (int v = 0; v < vertices; v++) {
                if (adjacency_matrix[u][v] != INF &&
                    distance[u] + adjacency_matrix[u][v] < distance[v]) {
                    return {false, distance}; // Negative cycle detected
                }
            }
        }
    }
    
    return {true, distance};
}

std::string GraphMatrix::bellmanFordVerbose(int source) {
    std::ostringstream oss;
    auto [has_solution, distances] = bellmanFord(source);
    
    if (!has_solution) {
        oss << "Bellman-Ford from vertex " << source << ":\n";
        oss << "  NEGATIVE CYCLE DETECTED - algorithm failed\n";
    } else {
        oss << "Bellman-Ford from vertex " << source << ":\n";
        for (int i = 0; i < vertices; i++) {
            oss << "  To " << i << ": ";
            if (distances[i] == INF) {
                oss << "INF (unreachable)\n";
            } else {
                oss << distances[i] << "\n";
            }
        }
    }
    
    return oss.str();
}

int GraphMatrix::getVertices() const {
    return vertices;
}

int GraphMatrix::getEdges() const {
    int count = 0;
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            if (adjacency_matrix[i][j] != INF && i != j) {
                count++;
            }
        }
    }
    return is_directed ? count : count / 2;
}

bool GraphMatrix::isConnected(int from, int to) const {
    if (from < 0 || from >= vertices || to < 0 || to >= vertices) {
        return false;
    }
    return adjacency_matrix[from][to] != INF;
}

void GraphMatrix::print() const {
    std::cout << "Graph (Adjacency Matrix):\n";
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            if (adjacency_matrix[i][j] == INF) {
                std::cout << "INF ";
            } else {
                std::cout << adjacency_matrix[i][j] << " ";
            }
        }
        std::cout << "\n";
    }
}

void GraphMatrix::loadFromMatrix(const std::vector<std::vector<int>>& matrix) {
    for (size_t i = 0; i < matrix.size() && i < static_cast<size_t>(vertices); i++) {
        for (size_t j = 0; j < matrix[i].size() && j < static_cast<size_t>(vertices); j++) {
            if (matrix[i][j] != 0 && matrix[i][j] != INF) {
                addEdge(i, j, matrix[i][j]);
            }
        }
    }
}
