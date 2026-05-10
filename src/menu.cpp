#include "menu.hpp"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

Menu::Menu() 
    : running(true), current_graph_type(NONE), current_vertices(0), 
      data_prepare(std::make_unique<DataPrepare>(12345)) {}

Menu::~Menu() {}

void Menu::run() {
    mainLoop();
}

void Menu::mainLoop() {
    while (running) {
        displayMainMenu();
        int choice = getValidInteger("Select option: ", 1, 7);
        handleMainMenuChoice(choice);
    }
}

void Menu::displayMainMenu() {
    std::cout << "\n========== MAIN MENU ==========\n";
    std::cout << "1. Create a new graph\n";
    std::cout << "2. Manage existing graph\n";
    std::cout << "3. Load graph from file\n";
    std::cout << "4. Test algorithms\n";
    std::cout << "5. Benchmark algorithms\n";
    std::cout << "6. Generate test data\n";
    std::cout << "7. Exit\n";
    std::cout << "===============================\n";
}

void Menu::displayGraphTypeMenu() {
    std::cout << "\n===== SELECT GRAPH TYPE =====\n";
    std::cout << "1. Adjacency List\n";
    std::cout << "2. Adjacency Matrix\n";
    std::cout << "=============================\n";
}

void Menu::displayGraphOperationsMenu() {
    std::cout << "\n===== GRAPH OPERATIONS =====\n";
    std::cout << "1. Add edge\n";
    std::cout << "2. Remove edge\n";
    std::cout << "3. Display graph\n";
    std::cout << "4. Show graph info\n";
    std::cout << "5. Test algorithms\n";
    std::cout << "6. Back to main menu\n";
    std::cout << "============================\n";
}

void Menu::displayAlgorithmMenu() {
    std::cout << "\n===== SELECT ALGORITHM =====\n";
    std::cout << "1. Dijkstra's algorithm\n";
    std::cout << "2. Dijkstra's algorithm with path\n";
    std::cout << "3. Back to graph operations\n";
    std::cout << "============================\n";
}

void Menu::handleMainMenuChoice(int choice) {
    switch (choice) {
        case 1:
            createGraph();
            break;
        case 2:
            manualGraphOperations();
            break;
        case 3:
            loadGraphFromFile();
            break;
        case 4:
            testAlgorithms();
            break;
        case 5:
            runBenchmark();
            break;
        case 6: 
            generateTestData();
            break;
        case 7:
            running = false;
            std::cout << "Exiting program. Goodbye!\n";
            break;
        default:
            std::cout << "Invalid choice!\n";
    }
}

void Menu::handleGraphTypeChoice(int choice) {
    displayGraphTypeMenu();
    int type_choice = getValidInteger("Select graph type: ", 1, 2);
    
    int vertices = getValidInteger("Enter number of vertices: ", 1, 10000);
    current_vertices = vertices;
    
    if (type_choice == 1) {
        graph_list = std::make_unique<GraphList>(vertices, false);
        current_graph_type = LIST;
        std::cout << "Adjacency List graph created with " << vertices << " vertices.\n";
    } else if (type_choice == 2) {
        graph_matrix = std::make_unique<GraphMatrix>(vertices, false);
        current_graph_type = MATRIX;
        std::cout << "Adjacency Matrix graph created with " << vertices << " vertices.\n";
    }
}

void Menu::manualGraphOperations() {
    if (current_graph_type == NONE) {
        std::cout << "No graph created. Please create a graph first.\n";
        return;
    }
    
    while (true) {
        displayGraphOperationsMenu();
        int op_choice = getValidInteger("Select operation: ", 1, 6);
        
        switch (op_choice) {
            case 1:
                addEdgeToGraph();
                break;
            case 2:
                removeEdgeFromGraph();
                break;
            case 3:
                displayGraphInfo();
                break;
            case 4: {
                if (current_graph_type == LIST && graph_list) {
                    std::cout << "Vertices: " << graph_list->getVertices() << "\n";
                    std::cout << "Edges: " << graph_list->getEdges() << "\n";
                } else if (current_graph_type == MATRIX && graph_matrix) {
                    std::cout << "Vertices: " << graph_matrix->getVertices() << "\n";
                    std::cout << "Edges: " << graph_matrix->getEdges() << "\n";
                }
                break;
            }
            case 5:
                testAlgorithms();
                break;
            case 6:
                return;
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void Menu::createGraph() {
    displayGraphTypeMenu();
    int choice = getValidInteger("Select graph type: ", 1, 2);
    
    int vertices = getValidInteger("Enter number of vertices: ", 1, 10000);
    current_vertices = vertices;
    
    if (choice == 1) {
        graph_list = std::make_unique<GraphList>(vertices, false);
        current_graph_type = LIST;
        std::cout << "Adjacency List graph created with " << vertices << " vertices.\n";
    } else if (choice == 2) {
        graph_matrix = std::make_unique<GraphMatrix>(vertices, false);
        current_graph_type = MATRIX;
        std::cout << "Adjacency Matrix graph created with " << vertices << " vertices.\n";
    }
}

void Menu::loadGraphFromFile() {
    std::string filename = getValidString("Enter filename (in data/ directory): ");
    std::string full_path = "data/" + filename;
    
    if (!fs::exists(full_path)) {
        std::cout << "File not found: " << full_path << "\n";
        return;
    }
    
    try {
        auto matrix = data_prepare->loadMatrixFromCSV(full_path);
        int vertices = matrix.size();
        current_vertices = vertices;
        
        displayGraphTypeMenu();
        int choice = getValidInteger("Load as which type: ", 1, 2);
        
        if (choice == 1) {
            graph_list = std::make_unique<GraphList>(vertices, false);
            graph_list->loadFromMatrix(matrix);
            current_graph_type = LIST;
            std::cout << "Graph loaded as Adjacency List.\n";
        } else if (choice == 2) {
            graph_matrix = std::make_unique<GraphMatrix>(vertices, false);
            graph_matrix->loadFromMatrix(matrix);
            current_graph_type = MATRIX;
            std::cout << "Graph loaded as Adjacency Matrix.\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Error loading file: " << e.what() << "\n";
    }
}

void Menu::addEdgeToGraph() {
    if (current_graph_type == NONE) {
        std::cout << "No graph created. Please create a graph first.\n";
        return;
    }
    
    int from = getValidInteger("Enter source vertex: ", 0, current_vertices - 1);
    int to = getValidInteger("Enter destination vertex: ", 0, current_vertices - 1);
    int weight = getValidInteger("Enter edge weight: ", 1, 1000);
    
    if (current_graph_type == LIST && graph_list) {
        graph_list->addEdge(from, to, weight);
        std::cout << "Edge added: " << from << " -> " << to << " (weight: " << weight << ")\n";
    } else if (current_graph_type == MATRIX && graph_matrix) {
        graph_matrix->addEdge(from, to, weight);
        std::cout << "Edge added: " << from << " -> " << to << " (weight: " << weight << ")\n";
    }
}

void Menu::removeEdgeFromGraph() {
    if (current_graph_type == NONE) {
        std::cout << "No graph created. Please create a graph first.\n";
        return;
    }
    
    int from = getValidInteger("Enter source vertex: ", 0, current_vertices - 1);
    int to = getValidInteger("Enter destination vertex: ", 0, current_vertices - 1);
    
    if (current_graph_type == LIST && graph_list) {
        graph_list->removeEdge(from, to);
        std::cout << "Edge removed: " << from << " -> " << to << "\n";
    } else if (current_graph_type == MATRIX && graph_matrix) {
        graph_matrix->removeEdge(from, to);
        std::cout << "Edge removed: " << from << " -> " << to << "\n";
    }
}
void Menu::displayGraphInfo() {
    if (current_graph_type == NONE) {
        std::cout << "No graph created.\n";
        return;
    }
    
    if (current_graph_type == LIST && graph_list) {
        graph_list->print();
    } else if (current_graph_type == MATRIX && graph_matrix) {
        graph_matrix->print();
    }
}

void Menu::testAlgorithms() {
    if (current_graph_type == NONE) {
        std::cout << "No graph created. Please create a graph first.\n";
        return;
    }
    
    displayAlgorithmMenu();
    int choice = getValidInteger("Select algorithm: ", 1, 3);
    
    switch(choice) {
        case 1:
            testDijkstra();
            break;
        case 2:
            testPathDijkstra();
            break;
        case 3:
            return;
        default:
            std::cout << "Invalid choice!\n";
    }
}

void Menu::testDijkstra() {
    int source = getValidInteger("Enter source vertex: ", 0, current_vertices - 1);
    
    if (current_graph_type == LIST && graph_list) {
        std::cout << graph_list->dijkstraVerbose(source);
    } else if (current_graph_type == MATRIX && graph_matrix) {
        std::cout << graph_matrix->dijkstraVerbose(source);
    }
}

void Menu::testPathDijkstra() {
    int source = getValidInteger("Enter source vertex: ", 0, current_vertices - 1);
    int destination = getValidInteger("Enter destination vertex: ", 0, current_vertices - 1);
    
    if (current_graph_type == LIST && graph_list) {
        std::cout << graph_list->dijkstraPathVerbose(source, destination);
    } else if (current_graph_type == MATRIX && graph_matrix) {
        std::cout << graph_matrix->dijkstraPathVerbose(source, destination);
    }
}


void Menu::generateTestData() {
    std::cout << "Generating test data files...\n";
    int num_seeds = getValidInteger("Enter number of seeds to generate (default 10): ", 1, 1000);
    std::string dir = getValidString("Enter directory to save data (default 'data/main_seed'): ");
    if (dir.empty()) {
        dir = "data/main_seed";
    }
    std::unique_ptr data_prepare = std::make_unique<DataPrepare>(12345, dir);
     // Create directory if it doesn't exist
    try {
        std::filesystem::create_directories(dir);
        data_prepare->generateTestData(num_seeds);
    } catch (const std::exception& e) {
        std::cout << "Error generating test data: " << e.what() << "\n";
    }
}

void Menu::displayBenchmarkMenu() {
    std::cout << "\n===== BENCHMARK MENU =====\n";
    std::cout << "1. Benchmark Dijkstra\n";
    std::cout << "2. Back\n";
    std::cout << "==========================\n";
}

void Menu::runBenchmark() {
    displayBenchmarkMenu();
    int choice = getValidInteger("Select benchmark: ", 1, 2);
    
    if (choice == 2) return;
    
    std::string data_dir = getValidString("Enter benchmark data folder (default 'data/main_seed'): ");
    if (data_dir.empty()) {
        data_dir = "data/main_seed";
    }
    
    if (!fs::exists(data_dir)) {
        std::cout << "Data folder not found: " << data_dir << "\n";
        return;
    }
    
    if (choice == 1) {
        benchmarkDijkstra(data_dir);
    }
}

void Menu::benchmarkDijkstra(const std::string& data_dir) {
    int vertices = getValidInteger("Enter number of vertices: ", 1, 1000);
    int density = getValidInteger("Enter density percentage (25/50/75/100): ", 1, 100);
    int num_files = getValidInteger("Enter number of files to test: ", 1, 1000);
    int source = getValidInteger("Enter source vertex: ", 0, vertices - 1);
    int rep_type = getValidInteger("Use List (1) or Matrix (2): ", 1, 2);
    
    bool use_matrix = (rep_type == 2);
    
    std::cout << "\nRunning Dijkstra benchmark...\n";
    
    try {
        auto benchmark_instance = std::make_unique<Benchmark>(data_dir);
        auto result = benchmark_instance->benchmarkDijkstra(vertices, density, num_files, source, use_matrix);
        benchmark_instance->printResults(result, "Dijkstra's Algorithm");
    } catch (const std::exception& e) {
        std::cout << "Error during benchmark: " << e.what() << "\n";
    }
}



int Menu::getValidInteger(const std::string& prompt, int min, int max) {
    int choice;
    while (true) {
        std::cout << prompt;
        if (std::cin >> choice && choice >= min && choice <= max) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a number between " << min << " and " << max << ".\n";
    }
}

std::string Menu::getValidString(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        if (std::getline(std::cin, input) && !input.empty()) {
            return input;
        }
        std::cout << "Invalid input. Please enter a non-empty string.\n";
    }
}

bool Menu::confirmAction(const std::string& message) {
    std::cout << message << " (y/n): ";
    char response;
    std::cin >> response;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return response == 'y' || response == 'Y';
}
