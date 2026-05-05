#include "graph_list.hpp"
#include <iostream>
#include <queue>
#include <algorithm>
#include <sstream>

GraphList::GraphList(int v, bool directed)
    : vertices(v), adjacency_list(v), is_directed(directed) {}

GraphList::~GraphList() {}

void GraphList::addEdge(int from, int to, int weight) {
    if (from < 0 || from >= vertices || to < 0 || to >= vertices) {
        return;
    }
    adjacency_list[from].push_back({to, weight});
    if (!is_directed) {
        adjacency_list[to].push_back({from, weight});
    }
}

void GraphList::removeEdge(int from, int to) {
    if (from < 0 || from >= vertices || to < 0 || to >= vertices) {
        return;
    }
    auto& edges = adjacency_list[from];
    edges.erase(std::remove_if(edges.begin(), edges.end(),
        [to](const Edge& e) { return e.to == to; }), edges.end());
    
    if (!is_directed) {
        auto& edges_to = adjacency_list[to];
        edges_to.erase(std::remove_if(edges_to.begin(), edges_to.end(),
            [from](const Edge& e) { return e.to == from; }), edges_to.end());
    }
}

std::vector<int> GraphList::dijkstra(int source) {
    std::vector<int> distance(vertices, std::numeric_limits<int>::max());
    std::vector<bool> visited(vertices, false);
    
    distance[source] = 0;
    
    for (int count = 0; count < vertices - 1; count++) {
        int u = -1;
        int min_dist = std::numeric_limits<int>::max();
        
        for (int v = 0; v < vertices; v++) {
            if (!visited[v] && distance[v] < min_dist) {
                min_dist = distance[v];
                u = v;
            }
        }
        
        if (u == -1) break;
        visited[u] = true;
        
        for (const auto& edge : adjacency_list[u]) {
            int v = edge.to;
            int weight = edge.weight;
            
            if (!visited[v] && distance[u] != std::numeric_limits<int>::max() &&
                distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
            }
        }
    }
    
    return distance;
}

std::string GraphList::dijkstraVerbose(int source) {
    std::ostringstream oss;
    auto distances = dijkstra(source);
    
    oss << "Dijkstra from vertex " << source << ":\n";
    for (int i = 0; i < vertices; i++) {
        oss << "  To " << i << ": ";
        if (distances[i] == std::numeric_limits<int>::max()) {
            oss << "INF (unreachable)\n";
        } else {
            oss << distances[i] << "\n";
        }
    }
    
    return oss.str();
}

std::pair<bool, std::vector<int>> GraphList::bellmanFord(int source) {
    std::vector<int> distance(vertices, std::numeric_limits<int>::max());
    distance[source] = 0;
    
    // Relax edges vertices - 1 times
    for (int i = 0; i < vertices - 1; i++) {
        for (int u = 0; u < vertices; u++) {
            if (distance[u] != std::numeric_limits<int>::max()) {
                for (const auto& edge : adjacency_list[u]) {
                    int v = edge.to;
                    int weight = edge.weight;
                    
                    if (distance[u] + weight < distance[v]) {
                        distance[v] = distance[u] + weight;
                    }
                }
            }
        }
    }
    
    // Check for negative cycles
    for (int u = 0; u < vertices; u++) {
        if (distance[u] != std::numeric_limits<int>::max()) {
            for (const auto& edge : adjacency_list[u]) {
                int v = edge.to;
                int weight = edge.weight;
                
                if (distance[u] + weight < distance[v]) {
                    return {false, distance}; // Negative cycle detected
                }
            }
        }
    }
    
    return {true, distance};
}

std::string GraphList::bellmanFordVerbose(int source) {
    std::ostringstream oss;
    auto [has_solution, distances] = bellmanFord(source);
    
    if (!has_solution) {
        oss << "Bellman-Ford from vertex " << source << ":\n";
        oss << "  NEGATIVE CYCLE DETECTED - algorithm failed\n";
    } else {
        oss << "Bellman-Ford from vertex " << source << ":\n";
        for (int i = 0; i < vertices; i++) {
            oss << "  To " << i << ": ";
            if (distances[i] == std::numeric_limits<int>::max()) {
                oss << "INF (unreachable)\n";
            } else {
                oss << distances[i] << "\n";
            }
        }
    }
    
    return oss.str();
}

int GraphList::getVertices() const {
    return vertices;
}

int GraphList::getEdges() const {
    int count = 0;
    for (const auto& list : adjacency_list) {
        count += list.size();
    }
    return is_directed ? count : count / 2;
}

bool GraphList::isConnected(int from, int to) const {
    if (from < 0 || from >= vertices || to < 0 || to >= vertices) {
        return false;
    }
    
    for (const auto& edge : adjacency_list[from]) {
        if (edge.to == to) {
            return true;
        }
    }
    return false;
}

void GraphList::print() const {
    std::cout << "Graph (Adjacency List):\n";
    for (int i = 0; i < vertices; i++) {
        std::cout << "  " << i << " -> ";
        for (const auto& edge : adjacency_list[i]) {
            std::cout << edge.to << "(" << edge.weight << ") ";
        }
        std::cout << "\n";
    }
}

void GraphList::loadFromMatrix(const std::vector<std::vector<int>>& matrix) {
    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] != 0 && matrix[i][j] != std::numeric_limits<int>::max()) {
                addEdge(i, j, matrix[i][j]);
            }
        }
    }
}
