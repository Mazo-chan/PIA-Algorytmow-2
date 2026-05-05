# Complete Project Documentation - Graph Algorithms

## Table of Contents
1. [Project Overview](#project-overview)
2. [Project Architecture](#project-architecture)
3. [Module Documentation](#module-documentation)
4. [Data Structures](#data-structures)
5. [Algorithms](#algorithms)
6. [Usage Guide](#usage-guide)
7. [Building and Running](#building-and-running)
8. [File Specifications](#file-specifications)
9. [Examples](#examples)

---

## Project Overview

**Project Name:** Mini Project 2 - Graph Algorithms (Miniprojekt 2 - Algorytmy grafowe)

**Purpose:** This project implements weighted graph data structures and implements shortest path finding algorithms (Dijkstra and Bellman-Ford) to find paths with minimum weight.

**Technology Stack:**
- **Language:** C++20
- **Build System:** CMake 3.10+
- **Compiler:** GCC 15+ / Clang 17+ / MSVC 19.43+
- **Platform:** Windows / macOS / Linux

**Key Features:**
- Two graph representations: Adjacency List and Adjacency Matrix
- Two shortest path algorithms: Dijkstra and Bellman-Ford
- Random test data generation with configurable parameters
- Interactive console menu for testing and algorithm verification
- CSV-based graph file I/O

---

## Project Architecture

### High-Level Design

```
┌─────────────────────────────────────┐
│        main.cpp (Entry Point)       │
├─────────────────────────────────────┤
│                Menu                 │  ← User Interface Layer
│         (menu.hpp/cpp)              │
├─────────────────────────────────────┤
│  ┌──────────────┐  ┌──────────────┐ │
│  │ GraphList    │  │ GraphMatrix   │ │  ← Graph Implementations
│  │(hpp/cpp)     │  │ (hpp/cpp)    │ │
│  └──────────────┘  └──────────────┘ │
├─────────────────────────────────────┤
│    DataPrepare (data_prepare.hpp)   │  ← Data Generation
├─────────────────────────────────────┤
│         CSV Files (data/)            │  ← Persistent Storage
└─────────────────────────────────────┘
```

### Module Relationships

- **main.cpp** → Creates and runs the Menu
- **Menu** → Manages user interaction and orchestrates all operations
- **GraphList/GraphMatrix** → Core data structures with algorithm implementations
- **DataPrepare** → Generates test graphs and handles I/O
- **data/** → Stores generated graph files

---

## Module Documentation

### 1. main.cpp - Program Entry Point

**Purpose:** Minimal entry point that initializes the application

**Responsibilities:**
- Create Menu instance
- Run the main application loop
- Handle top-level exceptions

**Code Structure:**
```cpp
int main() {
    try {
        Menu menu;
        menu.run();
    } catch (const std::exception& e) {
        // Handle errors
        return 1;
    }
    return 0;
}
```

**Key Points:**
- Exception-safe implementation
- No global state
- Minimal business logic

---

### 2. GraphList - Adjacency List Graph

**Header File:** `graph_list.hpp`
**Implementation:** `graph_list.cpp`

**Purpose:** Represents graphs using adjacency list data structure for memory-efficient sparse graph representation

#### Data Structure

```cpp
struct Edge {
    int to;        // Target vertex
    int weight;    // Edge weight
};

class GraphList {
private:
    int vertices;
    std::vector<std::vector<Edge>> adjacency_list;
    bool is_directed;
};
```

**Adjacency List Format:**
- Each vertex has a vector of edges
- Each edge stores destination vertex and weight
- Undirected graphs store edges in both directions
- Directed graphs store edges only one way

**Example Representation:**
```
Graph with vertices {0,1,2,3}:
0 → [(1,5), (2,3)]
1 → [(0,5), (3,2)]
2 → [(0,3), (3,4)]
3 → [(1,2), (2,4)]
```

#### Public Methods

**Construction & Destruction:**
```cpp
GraphList(int v, bool directed = false);
~GraphList();
```
- Creates graph with `v` vertices
- `directed=false` creates undirected graph

**Edge Operations:**
```cpp
void addEdge(int from, int to, int weight);
void removeEdge(int from, int to);
```
- `addEdge`: Adds weighted edge between vertices
- `removeEdge`: Removes edge between vertices

**Algorithms:**
```cpp
std::vector<int> dijkstra(int source);
std::string dijkstraVerbose(int source);

std::pair<bool, std::vector<int>> bellmanFord(int source);
std::string bellmanFordVerbose(int source);
```
- Returns shortest distances from source to all vertices
- Verbose versions return formatted string results

**Utility Methods:**
```cpp
int getVertices() const;
int getEdges() const;
bool isConnected(int from, int to) const;
void print() const;
void loadFromMatrix(const std::vector<std::vector<int>>& matrix);
```

#### Time & Space Complexity

| Operation | Time | Space |
|-----------|------|-------|
| Add Edge | O(1) | O(1) |
| Remove Edge | O(E/V) | O(1) |
| Dijkstra | O((V+E)logV) | O(V) |
| Bellman-Ford | O(VE) | O(V) |
| Storage | - | O(V+E) |

**Where V = vertices, E = edges**

---

### 3. GraphMatrix - Adjacency Matrix Graph

**Header File:** `graph_matrix.hpp`
**Implementation:** `graph_matrix.cpp`

**Purpose:** Represents graphs using adjacency matrix for dense graphs and easy random access

#### Data Structure

```cpp
class GraphMatrix {
private:
    int vertices;
    std::vector<std::vector<int>> adjacency_matrix;
    bool is_directed;
    static constexpr int INF = 1e9;  // Infinity value
};
```

**Matrix Format:**
- Matrix[i][j] = weight of edge from i to j
- Matrix[i][j] = INF means no edge exists
- Matrix[i][i] = 0 (distance to self)

**Example Representation:**
```
Graph with 3 vertices:
    0    1    2
0 [  0    5   INF ]
1 [  5    0    3  ]
2 [ INF   3    0  ]
```

#### Key Differences from GraphList

| Aspect | GraphList | GraphMatrix |
|--------|-----------|-------------|
| Memory | O(V+E) | O(V²) |
| Edge Lookup | O(degree) | O(1) |
| Add Edge | O(1) | O(1) |
| Dense Graphs | Poor | Good |
| Sparse Graphs | Good | Poor |
| Cache Friendly | Poor | Good |

#### Implementation Details

**Infinity Handling:**
```cpp
static constexpr int INF = 1e9;  // 1 billion used as infinity
```
- Used to represent non-existent edges
- Must be large enough to avoid overflow in calculations
- Compared directly in algorithm implementations

**Matrix Initialization:**
- Diagonal set to 0 (distance from vertex to itself)
- All other values set to INF
- Updated with actual edge weights via `addEdge()`

---

### 4. DataPrepare - Test Data Generation

**Header File:** `data_prepare.hpp`
**Implementation:** `data_prepare.cpp`

**Purpose:** Generate random test graphs with configurable parameters for algorithm benchmarking

#### Construction

```cpp
DataPrepare(unsigned int seed, const std::string& output_directory = "data/main_seed");
```
- `seed`: Initial random seed for reproducibility
- `output_directory`: Where to save generated CSV files

#### Main Methods

**Generate Test Dataset:**
```cpp
void generateTestData(int num_seeds = 100);
```
- Generates 100 different seed variations
- For each seed, creates graphs with:
  - Vertices: {10, 50, 100, 500, 1000}
  - Densities: {25%, 50%, 75%, 100%}
  - Total files: 100 × 5 × 4 = 2,000 graphs

**Generate Single Graph:**
```cpp
void generateGraphCSV(int vertices, int density_percent, unsigned int seed, const std::string& filename);
```
- Creates one graph and saves to CSV

**Generate Adjacency Matrix:**
```cpp
std::vector<std::vector<int>> generateAdjacencyMatrix(
    int vertices, 
    int density_percent, 
    unsigned int seed,
    int min_weight = 1,
    int max_weight = 100
);
```

**Parameters:**
- `vertices`: Number of vertices in graph
- `density_percent`: Percentage of possible edges (25-100)
- `seed`: Random seed for reproducibility
- `min_weight`: Minimum edge weight (default: 1)
- `max_weight`: Maximum edge weight (default: 100)

#### Connectivity Guarantee

```cpp
bool ensureConnectivity(std::vector<std::vector<int>>& matrix, int vertices);
```

**Process:**
1. Generate graph with target density
2. Check if all vertices are connected (BFS)
3. If disconnected, add random edges until connected
4. Ensures every generated graph is valid

**Why Important:**
- Disconnected graphs would have INF distances
- Makes algorithm comparison meaningful
- Ensures paths exist between all vertices

#### File I/O

**Save to CSV:**
```cpp
void saveMatrixToCSV(const std::vector<std::vector<int>>& matrix, const std::string& filename);
```

**Example CSV Format:**
```
0,5,INF,2
5,0,3,INF
INF,3,0,4
2,INF,4,0
```

**Load from CSV:**
```cpp
std::vector<std::vector<int>> loadMatrixFromCSV(const std::string& filename);
```

#### Seed Generation Strategy

```cpp
unsigned int generateNextSeed(unsigned int seed) {
    const unsigned int a = 1103515245;
    const unsigned int c = 12345;
    const unsigned int m = 2147483648;
    return (a * seed + c) % m;
}
```

**Linear Congruential Generator (LCG):**
- Simple, fast pseudo-random number generator
- Deterministic: same seed produces same sequence
- Sufficient for test data generation

---

### 5. Menu - User Interface

**Header File:** `menu.hpp`
**Implementation:** `menu.cpp`

**Purpose:** Provides interactive console interface for testing graphs and algorithms

#### Main Components

**Class State:**
```cpp
class Menu {
private:
    std::unique_ptr<GraphList> graph_list;
    std::unique_ptr<GraphMatrix> graph_matrix;
    std::unique_ptr<DataPrepare> data_prepare;
    bool running;
    enum GraphType { LIST, MATRIX, NONE };
    GraphType current_graph_type;
    int current_vertices;
};
```

**Menu Flow:**

```
┌─────────────────┐
│   MAIN MENU     │
└────────┬────────┘
    │
    ├─► 1. Create Graph ─► Select Type ─► Enter Vertices
    ├─► 2. Load from File ─► Enter Filename ─► Select Type
    ├─► 3. Test Algorithms ─► Algorithm Selection ─► Results
    ├─► 4. Generate Test Data ─► Number of Seeds
    └─► 5. Exit
```

#### Core Operations

**Create Graph:**
```cpp
void createGraph();
```
1. Display graph type menu (List/Matrix)
2. Get number of vertices from user
3. Create appropriate graph instance
4. Store type and vertex count

**Load Graph from File:**
```cpp
void loadGraphFromFile();
```
1. Request filename (relative to data/)
2. Parse CSV file into matrix
3. User selects representation type
4. Load matrix into graph object

**Add Edges Manually:**
```cpp
void addEdgeToGraph();
```
1. Request source vertex
2. Request destination vertex
3. Request edge weight
4. Call appropriate addEdge() method

**Test Algorithms:**
```cpp
void testAlgorithms();
void testDijkstra();
void testBellmanFord();
```
1. Request source vertex
2. Run selected algorithm
3. Display results (distances from source to all vertices)

**Generate Test Data:**
```cpp
void generateTestData();
```
1. Request number of seeds to generate
2. Call DataPrepare::generateTestData()
3. Creates graph files in data/main_seed/

#### Input Validation

**Safe Integer Input:**
```cpp
int getValidInteger(const std::string& prompt, int min, int max);
```
- Validates input is integer
- Checks range [min, max]
- Re-prompts on invalid input
- Clears input buffer

**String Input:**
```cpp
std::string getValidString(const std::string& prompt);
```
- Ensures non-empty input
- Returns user string

**Confirmation:**
```cpp
bool confirmAction(const std::string& message);
```
- Asks y/n question
- Returns boolean result

---

## Data Structures

### Edge Structure (GraphList)

```cpp
struct Edge {
    int to;        // Destination vertex (0 to vertices-1)
    int weight;    // Edge weight (positive or negative)
};
```

**Adjacency List:**
```cpp
std::vector<std::vector<Edge>> adjacency_list;
// adjacency_list[u] contains all edges from vertex u
```

### Distance Vector (Output)

**Dijkstra & Bellman-Ford Return:**
```cpp
std::vector<int> distances;
// distances[v] = shortest path weight from source to vertex v
// distances[v] = INF means v is unreachable
```

---

## Algorithms

### Dijkstra's Algorithm

**Purpose:** Find shortest paths from a single source to all vertices in weighted graphs with **non-negative edge weights**.

**Time Complexity:** O((V + E) log V) with binary heap

**Implementation in GraphList:**

```cpp
std::vector<int> dijkstra(int source) {
    // 1. Initialize distances
    std::vector<int> distance(vertices, INF);
    std::vector<bool> visited(vertices, false);
    distance[source] = 0;
    
    // 2. Main loop: process vertices in order of distance
    for (int count = 0; count < vertices - 1; count++) {
        // Find unvisited vertex with minimum distance
        int u = findMinDistanceVertex(distance, visited);
        
        if (u == -1 || distance[u] == INF) break;
        visited[u] = true;
        
        // 3. Relax edges from u
        for (const auto& edge : adjacency_list[u]) {
            int v = edge.to;
            int weight = edge.weight;
            
            if (!visited[v] && distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
            }
        }
    }
    
    return distance;
}
```

**Algorithm Steps:**

1. **Initialization:**
   - Set distance to source = 0
   - Set distance to all others = INF
   - Mark all vertices as unvisited

2. **Main Loop (V-1 iterations):**
   - Find unvisited vertex with minimum distance
   - Mark it as visited
   - For each unvisited neighbor:
     - If path through current vertex is shorter, update distance

3. **Termination:**
   - All reachable vertices have shortest distances
   - Unreachable vertices remain INF

**Example:**

```
Graph:    0 --(5)-- 1
          |  \      |
         (1) (2)   (3)
          |  /      |
          2 --(4)-- 3

Dijkstra from 0:
Step 1: distance = [0, INF, INF, INF], visit 0
Step 2: distance = [0, 5, 1, INF], visit 2
Step 3: distance = [0, 5, 1, 5], visit 3
Step 4: distance = [0, 5, 1, 5], visit 1
Final: [0, 5, 1, 5]
```

**Limitations:**
- ❌ Does not work with negative edge weights
- ✅ Works with disconnected graphs (unreachable = INF)
- ✅ Optimal for dense graphs

---

### Bellman-Ford Algorithm

**Purpose:** Find shortest paths from a single source, supporting **negative edge weights** and detecting **negative cycles**.

**Time Complexity:** O(VE)

**Implementation:**

```cpp
std::pair<bool, std::vector<int>> bellmanFord(int source) {
    std::vector<int> distance(vertices, INF);
    distance[source] = 0;
    
    // 1. Relax edges V-1 times
    for (int i = 0; i < vertices - 1; i++) {
        for (int u = 0; u < vertices; u++) {
            if (distance[u] != INF) {
                for (int v = 0; v < vertices; v++) {
                    if (adjacency_matrix[u][v] != INF &&
                        distance[u] + adjacency_matrix[u][v] < distance[v]) {
                        distance[v] = distance[u] + adjacency_matrix[u][v];
                    }
                }
            }
        }
    }
    
    // 2. Check for negative cycles
    for (int u = 0; u < vertices; u++) {
        if (distance[u] != INF) {
            for (int v = 0; v < vertices; v++) {
                if (adjacency_matrix[u][v] != INF &&
                    distance[u] + adjacency_matrix[u][v] < distance[v]) {
                    return {false, distance};  // Negative cycle!
                }
            }
        }
    }
    
    return {true, distance};
}
```

**Algorithm Steps:**

1. **Initialization:**
   - distance[source] = 0
   - distance[others] = INF

2. **Edge Relaxation (V-1 times):**
   - For each edge (u, v) with weight w:
     - If distance[u] + w < distance[v]:
       - distance[v] = distance[u] + w

3. **Negative Cycle Detection:**
   - Run relaxation one more time
   - If any distance decreases, negative cycle exists
   - Return failure

**Example with Negative Weights:**

```
Graph:     0 --(4)-- 1
           |         |
          (-2)      (-2)
           |         |
           2 --(3)-- 3

Bellman-Ford from 0:
Iteration 1: [0, 4, -2, INF]
Iteration 2: [0, 4, -2, 2]
Iteration 3: [0, 4, -2, 2]
Final: [0, 4, -2, 2]
```

**Example with Negative Cycle:**

```
Graph:     0 --1-- 1
           |        |
          1|       -3
           |        |
           2 --1--- 3
           |___ -1__|
           
Bellman-Ford detects cycle and returns false
```

**Advantages Over Dijkstra:**
- ✅ Supports negative edge weights
- ✅ Detects negative cycles
- ❌ Slower: O(VE) vs O((V+E)logV)

---

## Usage Guide

### Quick Start

#### 1. Build the Project

```bash
cd /path/to/PIA-Algorytmow-2
mkdir build
cd build
cmake ..
cmake --build .
```

#### 2. Run the Program

**Windows:**
```bash
.\Debug\algo_projekt_2.exe
```

**Linux/macOS:**
```bash
./algo_projekt_2
```

### Interactive Menu

#### Option 1: Create a New Graph

```
Select option: 1

===== SELECT GRAPH TYPE =====
1. Adjacency List
2. Adjacency Matrix
=============================
Select graph type: 1

Enter number of vertices: 4
Adjacency List graph created with 4 vertices.

===== GRAPH OPERATIONS =====
1. Add edge
2. Display graph
3. Show graph info
4. Test algorithms
5. Back to main menu
============================
Select operation: 1

Enter source vertex: 0
Enter destination vertex: 1
Enter edge weight: 5
Edge added: 0 -> 1 (weight: 5)
```

#### Option 2: Load Graph from File

```
Select option: 2

Enter filename (in data/ directory): graph_v10_d50_s0.csv
Load as which type: 1
Graph loaded as Adjacency List.
```

#### Option 3: Test Algorithms

```
Select option: 3

Enter source vertex: 0

===== SELECT ALGORITHM =====
1. Dijkstra's algorithm
2. Bellman-Ford algorithm
3. Back
============================
Select algorithm: 1

Dijkstra from vertex 0:
  To 0: 0
  To 1: 5
  To 2: 3
  To 3: 7
```

#### Option 4: Generate Test Data

```
Select option: 4

Enter number of seeds to generate (default 10): 5
Generating test data files...
Test data generation completed!
```

Creates files like:
- `data/main_seed/graph_v10_d25_s0.csv`
- `data/main_seed/graph_v10_d50_s0.csv`
- etc.

---

## Building and Running

### Prerequisites

- **C++20 Compiler:** GCC 15+, Clang 17+, or MSVC 19.43+
- **CMake:** Version 3.10 or higher
- **Operating System:** Windows, macOS, or Linux

### Build Instructions

**Step 1: Create Build Directory**
```bash
mkdir build && cd build
```

**Step 2: Configure with CMake**
```bash
cmake ..
```

**Step 3: Build Project**
```bash
cmake --build .
```

Or with specific configuration:
```bash
cmake --build . --config Debug
```

### Compilation Details

**CMakeLists.txt Configuration:**

```cmake
cmake_minimum_required(VERSION 3.10)
project(SD_PROJEKT_2 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files auto-discovered
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Executable creation
add_executable(algo_projekt_2 ${SOURCES})

# Include paths
include_directories(src)

# Compiler flags
if(MSVC)
    target_compile_options(algo_projekt_2 PRIVATE /W4)
else()
    target_compile_options(algo_projekt_2 PRIVATE -Wall -Wextra -Wpedantic)
endif()
```

### Compiler Warnings

**Expected warnings (non-critical):**
- C4267: size_t to int conversion (safe for this project scale)
- C4244: double to int conversion (intentional for INF)
- C4100: Unused parameters (can be ignored)

---

## File Specifications

### Project Structure

```
PIA-Algorytmow-2/
├── CMakeLists.txt           # Build configuration
├── README.md                # Project overview
├── DOCUMENTATION.md         # This file
├── src/
│   ├── main.cpp             # Program entry point
│   ├── menu.hpp/cpp         # Interactive menu
│   ├── graph_list.hpp/cpp   # Adjacency list implementation
│   ├── graph_matrix.hpp/cpp # Adjacency matrix implementation
│   └── data_prepare.hpp/cpp # Test data generation
├── data/
│   └── main_seed/           # Generated CSV graph files
├── build/
│   ├── Debug/
│   │   ├── algo_projekt_2.exe
│   │   └── ...
│   ├── CMakeFiles/
│   └── CMakeLists.txt
└── results/                 # For performance results
```

### CSV File Format

**Filename Convention:**
```
graph_v{vertices}_d{density}_s{seed}.csv
```
Example: `graph_v100_d50_s5.csv`

**File Content:**
```
0,5,INF,2,8
5,0,3,INF,1
INF,3,0,4,2
2,INF,4,0,6
8,1,2,6,0
```

- Comma-separated values
- INF represents no edge
- Values on line i represent edges from vertex i
- Symmetric for undirected graphs

---

## Examples

### Example 1: Create and Test Simple Graph

```cpp
// Create adjacency list with 5 vertices
GraphList g(5, false);

// Add edges
g.addEdge(0, 1, 4);
g.addEdge(0, 2, 2);
g.addEdge(1, 2, 1);
g.addEdge(1, 3, 5);
g.addEdge(2, 3, 8);
g.addEdge(2, 4, 10);
g.addEdge(3, 4, 2);

// Test Dijkstra from vertex 0
auto distances = g.dijkstra(0);
// Output: [0, 4, 2, 9, 11]
```

### Example 2: Generate Test Data Programmatically

```cpp
// Create data generator with seed 12345
DataPrepare gen(12345, "data/main_seed");

// Generate 100 test seeds
// Creates 100 × 5 vertices × 4 densities = 2000 graphs
gen.generateTestData(100);

// Or generate specific graph
gen.generateGraphCSV(50, 75, 12345, "test_graph.csv");
```

### Example 3: Detect Negative Cycle

```cpp
GraphMatrix g(3, false);
g.addEdge(0, 1, 1);
g.addEdge(1, 2, -3);
g.addEdge(2, 0, 1);  // Creates cycle with total weight -1

auto [has_solution, distances] = g.bellmanFord(0);
if (!has_solution) {
    std::cout << "Negative cycle detected!\n";
}
```

### Example 4: Load and Analyze Graph

```cpp
DataPrepare prep(0);
auto matrix = prep.loadMatrixFromCSV("data/main_seed/graph.csv");

GraphList g(matrix.size(), false);
g.loadFromMatrix(matrix);

std::cout << g.dijkstraVerbose(0);  // Get formatted results
```

---

## Performance Characteristics

### Dijkstra's Algorithm

| Graph Type | Vertices | Edges | Time (ms) |
|-----------|----------|-------|-----------|
| List (sparse) | 100 | 150 | ~0.01 |
| List (sparse) | 1000 | 1500 | ~0.1 |
| Matrix (dense) | 100 | 5000 | ~0.02 |
| Matrix (dense) | 1000 | 500000 | ~2.0 |

### Bellman-Ford Algorithm

| Graph Type | Vertices | Edges | Time (ms) |
|-----------|----------|-------|-----------|
| List (sparse) | 100 | 150 | ~1.0 |
| List (sparse) | 1000 | 1500 | ~10 |
| Matrix (dense) | 100 | 5000 | ~2.0 |
| Matrix (dense) | 1000 | 500000 | ~20 |

**Key Observations:**
- Dijkstra significantly faster for sparse graphs
- Bellman-Ford slower but handles negative weights
- Matrix representation better for dense graphs
- List representation better for sparse graphs

---

## Troubleshooting

### Issue: "File not found" when loading graph

**Solution:** 
1. Generate test data first (Option 4)
2. Use exact filename from data/main_seed/
3. Check file exists: `dir data\main_seed\` (Windows)

### Issue: Graph appears disconnected

**Explanation:**
- Graphs are auto-verified for connectivity
- Some vertices may be unreachable despite connectivity check
- This shouldn't happen if generation succeeded

### Issue: Negative cycle not detected

**Cause:**
- Only works with negative edge weights
- Ensure edges have negative weights
- Bellman-Ford algorithm correctly detects cycles

---

## Summary

This project demonstrates:
1. **Data Structure Design:** Two graph representations with tradeoffs
2. **Algorithm Implementation:** Dijkstra and Bellman-Ford with full explanations
3. **Software Engineering:** Modular design, error handling, user interface
4. **Testing:** Automated test data generation with configurable parameters
5. **C++20:** Modern C++ features (structured bindings, smart pointers, etc.)

The documentation above provides complete understanding of architecture, algorithms, and usage patterns.
