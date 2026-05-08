#include "graph_list.hpp"
#include <iostream>
#include <climits>
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
    std::vector<int> distance(vertices, INT_MAX);
    std::vector<bool> visited(vertices, false);
    
    distance[source] = 0;
    
    for (int count = 0; count < vertices - 1; count++) {
        int u = -1;
        int min_dist = INT_MAX;
        
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
            
        if (!visited[v] && distance[u] != INT_MAX &&
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
        if (distances[i] == INT_MAX) {
            oss << "INF (unreachable)\n";
        } else {
            oss << distances[i] << "\n";
        }
    }
    
    return oss.str();
}

std::vector<int> GraphList::dijkstraPath(int source, int destination) {
    std::vector<int> distance(vertices, INT_MAX);
    std::vector<int> parent(vertices, -1);
    std::vector<bool> visited(vertices, false);
    
    distance[source] = 0;
    
    for (int count = 0; count < vertices - 1; count++) {
        int u = -1;
        int min_dist = INT_MAX;
        
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
            
            if (!visited[v] && distance[u] != INT_MAX &&
                distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
                parent[v] = u;
            }
        }
    }
    
    // Reconstruct path
    std::vector<int> path;
    if (distance[destination] == INT_MAX) {
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

std::string GraphList::dijkstraPathVerbose(int source, int destination) {
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
            if (matrix[i][j] != 0 && matrix[i][j] != INT_MAX) {
                addEdge(i, j, matrix[i][j]);
            }
        }
    }
}
