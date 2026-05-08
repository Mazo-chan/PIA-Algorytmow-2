#ifndef GRAPH_MATRIX_HPP
#define GRAPH_MATRIX_HPP

#include <vector>
#include <climits>
#include <string>

class GraphMatrix {
private:
    int vertices;
    std::vector<std::vector<int>> adjacency_matrix;
    bool is_directed;
    static constexpr int INF = INT_MAX;

public:
    GraphMatrix(int v, bool directed = false);
    ~GraphMatrix();
    
    void addEdge(int from, int to, int weight);
    void removeEdge(int from, int to);
    
    // Dijkstra's algorithm - for non-negative weights
    std::vector<int> dijkstra(int source);
    std::string dijkstraVerbose(int source);
    std::vector<int> dijkstraPath(int source, int destination);
    std::string dijkstraPathVerbose(int source, int destination);
    
    int getVertices() const;
    int getEdges() const;
    bool isConnected(int from, int to) const;
    void print() const;
    
    // Load from adjacency matrix format
    void loadFromMatrix(const std::vector<std::vector<int>>& matrix);
};

#endif // GRAPH_MATRIX_HPP
