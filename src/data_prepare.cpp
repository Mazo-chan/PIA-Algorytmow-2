#include "data_prepare.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cmath>
#include <algorithm>
#include <queue>

namespace fs = std::filesystem;

DataPrepare::DataPrepare(unsigned int seed, const std::string& output_directory)
    : main_seed(seed), output_dir(output_directory) {
    // Create output directory if it doesn't exist
    fs::create_directories(output_dir);
}

DataPrepare::~DataPrepare() {}

unsigned int DataPrepare::generateNextSeed(unsigned int seed) {
    // Deterministic seed generator
    const unsigned int a = 1103515245;
    const unsigned int c = 12345;
    const unsigned int m = 2147483648;
    return (a * seed + c) % m;
}

void DataPrepare::generateTestData(int num_seeds) {
    std::vector<int> vertices = {10, 50, 100, 500, 1000};
    std::vector<int> densities = {25, 50, 75, 100}; // percentages
    
    unsigned int current_seed = main_seed;
    
    std::cout << "Generating test data...\n";
    
    for (int seed_idx = 0; seed_idx < num_seeds; seed_idx++) {
        current_seed = generateNextSeed(current_seed);
        
        for (int v : vertices) {
            for (int d : densities) {
                std::string filename = output_dir + "/graph_v" + std::to_string(v) +
                                      "_d" + std::to_string(d) +
                                      "_s" + std::to_string(seed_idx) + ".csv";
                
                generateGraphCSV(v, d, current_seed, filename);
            }
        }
    }
    
    std::cout << "Test data generation complete!\n";
}

void DataPrepare::generateGraphCSV(int vertices, int density_percent, unsigned int seed, const std::string& filename) {
    auto matrix = generateAdjacencyMatrix(vertices, density_percent, seed);
    saveMatrixToCSV(matrix, filename);
}

std::vector<std::vector<int>> DataPrepare::generateAdjacencyMatrix(
    int vertices, 
    int density_percent, 
    unsigned int seed,
    int min_weight,
    int max_weight) {
    
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> weight_dist(min_weight, max_weight);
    std::uniform_real_distribution<double> probability_dist(0.0, 1.0);
    
    // Initialize an empty adjacency matrix
    // 0 means no edge, positive weight means edge exists
    std::vector<std::vector<int>> matrix(vertices, std::vector<int>(vertices, 0));
    
    // Calculate target number of edges
    int max_edges = (vertices * (vertices - 1)) / 2;
    int target_edges = (max_edges * density_percent) / 100;
    
    // Add edges randomly
    // As long as we didn't reach the target number of edges, keep adding
    int added_edges = 0;
    for (int i = 0; i < vertices && added_edges < target_edges; i++) {
        for (int j = i + 1; j < vertices && added_edges < target_edges; j++) {
            // Randomly decide whether to add an edge based on the desired density
            double prob = probability_dist(rng);
            double edge_probability = static_cast<double>(density_percent) / 100.0;
            
            // If the random probability is less than the edge probability, add an edge
            if (prob < edge_probability) {
                int weight = weight_dist(rng);
                matrix[i][j] = weight;
                matrix[j][i] = weight; // undirected
                added_edges++;
            }
        }
    }
    
    // Ensure connectivity
    ensureConnectivity(matrix, vertices);
    
    return matrix;
}

bool DataPrepare::ensureConnectivity(std::vector<std::vector<int>>& matrix, int vertices) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> weight_dist(1, 100);
    
    // BFS to check connectivity
    // We start from 0 and simply traverse the graph to see if we can reach all vertices
    auto isConnected = [&]() {
        std::vector<bool> visited(vertices, false);
        std::queue<int> q;
        q.push(0);
        visited[0] = true;
        int count = 1;
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            
            for (int v = 0; v < vertices; v++) {
                if (!visited[v] && matrix[u][v] != 0) {
                    visited[v] = true;
                    q.push(v);
                    count++;
                }
            }
        }
        
        return count == vertices;
    };
    
    // Add edges to connect components if necessary
    while (!isConnected()) {
        int u = rng() % vertices;
        int v = rng() % vertices;
        
        if (u != v && matrix[u][v] == 0) {
            int weight = weight_dist(rng);
            matrix[u][v] = weight;
            matrix[v][u] = weight;
        }
    }
    
    return true;
}

void DataPrepare::saveMatrixToCSV(const std::vector<std::vector<int>>& matrix, const std::string& filename) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return;
    }
    
    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix[i].size(); j++) {
            file << matrix[i][j];
            if (j < matrix[i].size() - 1) {
                file << ",";
            }
        }
        file << "\n";
    }
    
    file.close();
}

std::vector<std::vector<int>> DataPrepare::loadMatrixFromCSV(const std::string& filename) {
    std::vector<std::vector<int>> matrix;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return matrix;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        std::stringstream ss(line);
        std::string value;
        
        while (std::getline(ss, value, ',')) {
            row.push_back(std::stoi(value));
        }
        
        if (!row.empty()) {
            matrix.push_back(row);
        }
    }
    
    file.close();
    return matrix;
}
