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

std::vector<int> GraphMatrix::dijkstraPath(int source, int destination) {
    std::vector<int> distance(vertices, INF);
    std::vector<int> parent(vertices, -1);
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
                parent[v] = u;
            }
        }
    }
    
    // Reconstruct path
    std::vector<int> path;
    if (distance[destination] == INF) {
        return path;  // No path exists
    }
    
    int current = destination;
    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }
    
    std::reverse(path.begin(), path.end());
    return path;
}

std::string GraphMatrix::dijkstraPathVerbose(int source, int destination) {
    std::ostringstream oss;
    auto path = dijkstraPath(source, destination);
    
    if (path.empty()) {
        oss << "No path exists from " << source << " to " << destination << "\n";
    } else {
        oss << "Shortest path from " << source << " to " << destination << ":\n";
        oss << "  ";
        for (size_t i = 0; i < path.size(); i++) {
            oss << path[i];
            if (i < path.size() - 1) {
                oss << " -> ";
            }
        }
        oss << "\n";
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
