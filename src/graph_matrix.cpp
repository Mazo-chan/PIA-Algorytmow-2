#include "graph_matrix.hpp"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <queue>
#include <utility>

struct HeapNode {
    int distance;
    int vertex;
    std::vector<int> path;
    
    bool operator>(const HeapNode& other) const {
        return distance > other.distance;
    }
};

GraphMatrix::GraphMatrix(int v, bool directed)
    : vertices(v), adjacency_matrix(v, std::vector<int>(v, INF)), is_directed(directed) {
    // Initialize diagonal to 0 (distance to itself)
    for (int i = 0; i < v; i++) {
        adjacency_matrix[i][i] = 0;
    }
}

GraphMatrix::~GraphMatrix() {}

void GraphMatrix::setDirected(bool directed) {
    is_directed = directed;
}

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

DijkstraResult GraphMatrix::dijkstra(int source) {
    std::vector<std::pair<int, std::vector<int>>> results(vertices);
    std::vector<bool> visited(vertices, false);
    
    std::priority_queue<HeapNode, std::vector<HeapNode>, std::greater<HeapNode>> pq;
    
    // Initialize all distances to INF
    for (int i = 0; i < vertices; i++) {
        results[i] = {INF, {}};
    }
    
    results[source] = {0, {source}};
    pq.push({0, source, {source}});
    
    while (!pq.empty()) {
        HeapNode current = pq.top();
        pq.pop();
        
        int u = current.vertex;
        
        if (visited[u]) {
            continue;
        }
        visited[u] = true;
        
        for (int v = 0; v < vertices; v++) {
            if (!visited[v] && adjacency_matrix[u][v] != INF) {
                int newDistance = current.distance + adjacency_matrix[u][v];
                
                if (newDistance < results[v].first) {
                    results[v].first = newDistance;
                    results[v].second = current.path;
                    results[v].second.push_back(v);
                    
                    pq.push({newDistance, v, results[v].second});
                }
            }
        }
    }
    
    return {results};
}

std::string GraphMatrix::dijkstraVerbose(int source) {
    std::ostringstream oss;
    auto result = dijkstra(source);
    
    oss << "Dijkstra from vertex " << source << ":\n";
    for (int i = 0; i < vertices; i++) {
        oss << "  To " << i << ": ";
        if (result.results[i].first == INF) {
            oss << "INF (unreachable)\n";
        } else {
            oss << result.results[i].first << "\n";
        }
    }
    
    return oss.str();
}

std::string GraphMatrix::dijkstraPathVerbose(int source, int destination) {
    std::ostringstream oss;
    auto result = dijkstra(source);
    
    if (result.results[destination].first == INF) {
        oss << "No path exists from " << source << " to " << destination << "\n";
    } else {
        oss << "Shortest path from " << source << " to " << destination << ":\n";
        oss << "  ";
        for (size_t i = 0; i < result.results[destination].second.size(); i++) {
            oss << result.results[destination].second[i];
            if (i < result.results[destination].second.size() - 1) {
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
                std::cout << std::setw(8) << "INF ";
            } else {
                std::cout << std::setw(8) << adjacency_matrix[i][j] << " ";
            }
        }
        std::cout << "\n";
    }
}

void GraphMatrix::loadFromMatrix(const std::vector<std::vector<int>>& matrix) {
    setDirected(true);
    for (size_t i = 0; i < matrix.size() && i < static_cast<size_t>(vertices); i++) {
        for (size_t j = 0; j < matrix[i].size() && j < static_cast<size_t>(vertices); j++) {
            if (matrix[i][j] != 0 && matrix[i][j] != INF) {
                addEdge(i, j, matrix[i][j]);
            }
        }
    }
    setDirected(false);
}
