#ifndef MENU_HPP
#define MENU_HPP

#include "graph_list.hpp"
#include "graph_matrix.hpp"
#include "data_prepare.hpp"
#include "benchmark.hpp"
#include <string>
#include <memory>

class Menu {
private:
    std::unique_ptr<GraphList> graph_list;
    std::unique_ptr<GraphMatrix> graph_matrix;
    std::unique_ptr<DataPrepare> data_prepare;
    
    bool running;
    
    // Menu display methods
    void displayMainMenu();
    void displayGraphTypeMenu();
    void displayGraphOperationsMenu();
    void displayAlgorithmMenu();
    void displayBenchmarkMenu();
    
    // Graph operations
    void createGraph();
    void loadGraphFromFile();
    void addEdgeToGraph();
    void displayGraphInfo();
    void testAlgorithms();
    
    // Algorithm testing
    void testDijkstra();
    
    // Benchmarking
    void runBenchmark();
    void benchmarkDijkstra(const std::string& data_dir);
    
    // Data generation
    void generateTestData();
    
    // Helper methods
    int getValidInteger(const std::string& prompt, int min, int max);
    std::string getValidString(const std::string& prompt);
    bool confirmAction(const std::string& message);
    
    enum GraphType { LIST, MATRIX, NONE };
    GraphType current_graph_type;
    int current_vertices;

public:
    Menu();
    ~Menu();
    
    void run();
    void mainLoop();
    void handleMainMenuChoice(int choice);
    void handleGraphTypeChoice(int choice);
    void handleOperationChoice(int choice);
    void handleAlgorithmChoice(int choice);
};

#endif // MENU_HPP
