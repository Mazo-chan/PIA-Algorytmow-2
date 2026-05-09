#ifndef DIJKSTRA_RESULT_HPP
#define DIJKSTRA_RESULT_HPP

#include <vector>
#include <utility>

struct DijkstraResult {
    std::vector<std::pair<int, std::vector<int>>> results;  // (weight, path) for each vertex
};

#endif // DIJKSTRA_RESULT_HPP
