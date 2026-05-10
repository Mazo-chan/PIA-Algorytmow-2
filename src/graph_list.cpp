#include "graph_list.hpp"
#include <iostream>
#include <climits>
#include <queue>
#include <algorithm>
#include <sstream>

struct HeapNode {
    int distance;
    int vertex;
    std::vector<int> path;
    
    // For priority queue ordering (min-heap based on distance)
    bool operator>(const HeapNode& other) const {
        return distance > other.distance;
    }
};

GraphList::GraphList(int v, bool directed)
    : vertices(v), adjacency_list(v), is_directed(directed) {}

GraphList::~GraphList() {}

void GraphList::setDirected(bool directed) {
    is_directed = directed;
}

void GraphList::addEdge(int from, int to, int weight) {
    if (from < 0 || from >= vertices || to < 0 || to >= vertices) {
        return;
    }
    // Add edge to the adjacency list
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
    // Remove edge from 'from' to 'to' using remove-erase idiom
    // remove_if will move the matching edge to the end of the vector, and then erase will remove it
    edges.erase(std::remove_if(edges.begin(), edges.end(),
        [to](const Edge& e) { return e.to == to; }), edges.end());
    
    // If the graph is undirected, also remove the edge from 'to' to 'from'
    if (!is_directed) {
        auto& edges_to = adjacency_list[to];
        edges_to.erase(std::remove_if(edges_to.begin(), edges_to.end(),
            [from](const Edge& e) { return e.to == from; }), edges_to.end());
    }
}

DijkstraResult GraphList::dijkstra(int source) {
    std::vector<std::pair<int, std::vector<int>>> results(vertices);
    std::vector<bool> visited(vertices, false);
    
    // true - we need to reshuffle, false - child is in the right place (obviously)
    // normally std::less is used for max-heap
    std::priority_queue<HeapNode, std::vector<HeapNode>, std::greater<HeapNode>> pq;
    
    // Initialize all distances to INT_MAX
    for (int i = 0; i < vertices; i++) {
        results[i] = {INT_MAX, {}};
    }
    
    // Distance to source is 0
    results[source] = {0, {source}};
    pq.push({0, source, {source}});
    
    // Body of Dijkstra's algorithm
    while (!pq.empty()) {
        // Get the vertex with the smallest distance
        HeapNode current = pq.top();
        pq.pop();
        
        int u = current.vertex;
        
        if (visited[u]) {
            continue;
        }
        visited[u] = true;
        
        // Explore neighbors
        for (const auto& edge : adjacency_list[u]) {
            int v = edge.to;
            int weight = edge.weight;
            int newDistance = current.distance + weight;
            
            // If a shorter path to v is found
            if (!visited[v] && newDistance < results[v].first) {
                // Update the distance and path for vertex v
                results[v].first = newDistance;
                results[v].second = current.path;
                results[v].second.push_back(v);
                
                // Add the neighbor to the priority queue
                pq.push({newDistance, v, results[v].second});
            }
        }
    }
    
    return {results};
}

// Verbose output for Dijkstra's algorithm
std::string GraphList::dijkstraVerbose(int source) {
    std::ostringstream oss;
    auto result = dijkstra(source);
    
    oss << "Dijkstra from vertex " << source << ":\n";
    for (int i = 0; i < vertices; i++) {
        oss << "  To " << i << ": ";
        if (result.results[i].first == INT_MAX) {
            oss << "INF (unreachable)\n";
        } else {
            oss << result.results[i].first <<  "\n";
        }
    }
    
    return oss.str();
}

// Verbose output for Dijkstra's algorithm with path
std::string GraphList::dijkstraPathVerbose(int source, int destination) {
    std::ostringstream oss;
    // Get the Dijkstra result for the source vertex
    auto result = dijkstra(source);
    
    if (result.results[destination].first == INT_MAX) {
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

int GraphList::getVertices() const {
    return vertices;
}

int GraphList::getEdges() const {
    int count = 0;
    for (const auto& list : adjacency_list) {
        count += list.size();
    }
    // Each edge is counted twice in an undirected graph
    return is_directed ? count : count / 2;
}

bool GraphList::isConnected(int from, int to) const {
    // Check if vertices are valid
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
    // Load edges from the adjacency matrix
    // setDirected(true) to avoid adding duplicate edges for undirected graphs
    // After loading, setDirected(false) to allow adding edges in both directions if needed
    setDirected(true);
    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] != 0 && matrix[i][j] != INT_MAX) {
                addEdge(i, j, matrix[i][j]);
            }
        }
    }
    setDirected(false);
}
