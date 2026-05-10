#ifndef GRAPH_LIST_HPP
#define GRAPH_LIST_HPP

#include <vector>
#include <string>
#include <utility>
#include "dijkstra_result.hpp"

struct Edge {
    int to;
    int weight;
};

class GraphList {
private:
    int vertices;
    std::vector<std::vector<Edge>> adjacency_list;
    bool is_directed;

public:
    GraphList(int v, bool directed = false);
    ~GraphList();
    
    void setDirected(bool directed);
    void addEdge(int from, int to, int weight);
    void removeEdge(int from, int to);
    
    // Dijkstra's algorithm - returns (weight, path) pairs for all vertices
    DijkstraResult dijkstra(int source);
    std::string dijkstraVerbose(int source);
    std::string dijkstraPathVerbose(int source, int destination);
    
    int getVertices() const;
    int getEdges() const;
    bool isConnected(int from, int to) const;
    void print() const;
    
    // Load from adjacency matrix format
    void loadFromMatrix(const std::vector<std::vector<int>>& matrix);
};

#endif // GRAPH_LIST_HPP
