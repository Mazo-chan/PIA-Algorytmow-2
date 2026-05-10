#ifndef DATA_PREPARE_HPP
#define DATA_PREPARE_HPP

#include <vector>
#include <string>
#include <random>

class DataPrepare {
private:
    unsigned int main_seed;
    std::string output_dir;
    
    unsigned int generateNextSeed(unsigned int seed);
    
public:
    DataPrepare(unsigned int seed, const std::string& output_directory = "data/main_seed");
    ~DataPrepare();
    
    // Generate test data files
    void generateTestData(int num_seeds = 100);
    
    // Generate a single graph CSV file
    void generateGraphCSV(int vertices, int density_percent, unsigned int seed, const std::string& filename);
    
    // Generate and save adjacency matrix
    // NOTE: Default weights are positive (1-100) to prevent negative cycles
    // For Dijkstra testing, use default positive weights
    std::vector<std::vector<int>> generateAdjacencyMatrix(
        int vertices, 
        int density_percent, 
        unsigned int seed,
        int min_weight = 1,           // Positive minimum by default (prevents cycles)
        int max_weight = 100
    );
    
    // Ensure graph connectivity
    bool ensureConnectivity(std::vector<std::vector<int>>& matrix, int vertices);
    
    // Save matrix to CSV
    void saveMatrixToCSV(const std::vector<std::vector<int>>& matrix, const std::string& filename);
    
    // Load matrix from CSV
    std::vector<std::vector<int>> loadMatrixFromCSV(const std::string& filename);
};

#endif // DATA_PREPARE_HPP
