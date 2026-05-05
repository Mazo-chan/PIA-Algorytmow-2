# Quick Reference Guide

## One-Minute Overview

This C++ project implements **shortest path algorithms** for weighted graphs using two different data structures.

### What It Does
- Finds shortest paths between vertices in a graph (Dijkstra, Bellman-Ford)
- Generates random test graphs with configurable sizes and densities
- Provides an interactive menu for manual testing

### Technologies
- **Language:** C++20
- **Build:** CMake 3.10+
- **Compilers:** GCC 15+, Clang 17+, MSVC 19.43+

---

## Quick Start

### Build (90 seconds)
```bash
cd PIA-Algorytmow-2
mkdir build && cd build
cmake ..
cmake --build .
```

### Run
```bash
# Windows
./Debug/algo_projekt_2.exe

# Linux/macOS
./algo_projekt_2
```

### Menu Options
1. **Create Graph** - Build graph manually, select representation (list/matrix)
2. **Load from File** - Load pre-generated CSV graph
3. **Test Algorithms** - Run Dijkstra or Bellman-Ford
4. **Generate Test Data** - Create random graphs for testing
5. **Exit** - Quit program

---

## Key Concepts

### Two Graph Representations

| Feature | Adjacency List | Adjacency Matrix |
|---------|---------------|------------------|
| **Memory** | O(V+E) - Efficient for sparse | O(V²) - Efficient for dense |
| **Edge Access** | O(E/V) average | O(1) - Direct lookup |
| **Add Edge** | O(1) | O(1) |
| **Best for** | Sparse graphs | Dense graphs |

### Two Shortest Path Algorithms

| Algorithm | Edge Weights | Negative Weights | Cycles | Time |
|-----------|-------------|------------------|--------|------|
| **Dijkstra** | Positive only | ❌ No | ❌ No | O((V+E)logV) |
| **Bellman-Ford** | Any | ✅ Yes | ✅ Detects | O(VE) |

---

## File Organization

```
src/
├── main.cpp               # Program start
├── menu.hpp/cpp           # User interface
├── graph_list.hpp/cpp     # Adjacency list + algorithms
├── graph_matrix.hpp/cpp   # Adjacency matrix + algorithms
└── data_prepare.hpp/cpp   # Test data generation

data/
└── main_seed/             # Generated CSV files

build/
└── Debug/
    └── algo_projekt_2.exe # Compiled program
```

---

## Common Tasks

### Task 1: Create and Test a Graph
```cpp
GraphList g(5, false);  // 5 vertices, undirected
g.addEdge(0, 1, 5);
g.addEdge(1, 2, 3);
g.addEdge(0, 2, 9);

auto distances = g.dijkstra(0);
// Result: [0, 5, 8, INF, INF]
```

### Task 2: Generate Test Graphs
```cpp
DataPrepare gen(12345);
gen.generateTestData(10);  // 10 seeds × 5 sizes × 4 densities
```

### Task 3: Load Graph from CSV
```cpp
DataPrepare prep(0);
auto matrix = prep.loadMatrixFromCSV("data/graph.csv");
GraphList g(matrix.size(), false);
g.loadFromMatrix(matrix);
```

### Task 4: Detect Negative Cycle
```cpp
auto [success, distances] = g.bellmanFord(0);
if (!success) {
    cout << "Negative cycle detected!";
}
```

---

## Algorithm Reference

### Dijkstra's Algorithm
- **Purpose:** Find shortest paths (non-negative weights only)
- **Approach:** Greedy - always visit nearest unvisited vertex
- **Time:** O((V+E)logV) with binary heap
- **Use When:** Fast solution needed, no negative weights

**Step-by-step:**
1. Set distance[source]=0, all others=INF
2. Repeat V-1 times:
   - Find nearest unvisited vertex
   - Mark as visited
   - Relax all outgoing edges

### Bellman-Ford Algorithm
- **Purpose:** Find shortest paths (any weights, detects negatives)
- **Approach:** Relaxation - gradually improve distances
- **Time:** O(VE)
- **Use When:** Negative weights or cycle detection needed

**Step-by-step:**
1. Set distance[source]=0, all others=INF
2. Repeat V-1 times:
   - For each edge: relax if shorter path found
3. Check one more time:
   - If any distance improves, negative cycle exists

---

## Data Structures

### Edge (for Adjacency List)
```cpp
struct Edge {
    int to;      // Destination vertex
    int weight;  // Edge weight
};
```

### Graph Input/Output
```cpp
// Input: Adjacency matrix (vector of vectors)
std::vector<std::vector<int>> matrix = {
    {0, 5, INF},
    {5, 0, 3},
    {INF, 3, 0}
};

// Output: Distance vector
std::vector<int> distances = {0, 5, 8};  // From source 0
```

### CSV Format
```
0,5,INF,2
5,0,3,INF
INF,3,0,4
2,INF,4,0
```
- Row i: edges from vertex i
- Column j: edge to vertex j
- INF (1e9): no edge exists
- Symmetric for undirected graphs

---

## Performance Tips

### Choose Right Representation
- **Adjacency List** → If edge count << V²
- **Adjacency Matrix** → If edge count ≈ V²

### Choose Right Algorithm
- **Dijkstra** → All positive weights, need speed
- **Bellman-Ford** → Negative weights or cycle detection

### Input Limits
- **Max Vertices:** 10,000+ (depends on representation)
- **Edge Weight Range:** 1-100 (Dijkstra), any int (Bellman-Ford)
- **Test Data:** 100 seeds × 5 sizes × 4 densities ≈ 2,000 graphs

---

## Compilation Warnings

**Safe to ignore:**
- `C4267` - size_t to int conversion (no overflow risk)
- `C4244` - double to int (intentional for INF constant)
- `C4100` - Unused parameter (not referenced in function)

**Actual errors:**
- `C4251` - Requires DLL export (not in this project)
- Linker errors - Missing .cpp files (should not occur)

---

## Troubleshooting

| Problem | Solution |
|---------|----------|
| Build fails | Run `cmake ..` first, then `cmake --build .` |
| File not found | Generate data with menu option 4 first |
| All distances INF | Graph may be disconnected, graph ensures it's not |
| Program hangs | Press Ctrl+C to exit menu |
| Negative cycle not detected | Verify edges have negative weights |

---

## Code Examples

### Example 1: Basic Usage
```cpp
// Create 3-vertex graph
GraphMatrix g(3);

// Add edges
g.addEdge(0, 1, 7);
g.addEdge(0, 2, 5);
g.addEdge(1, 2, 2);

// Find shortest paths from vertex 0
auto dist = g.dijkstra(0);
// dist = [0, 7, 5]

// Verbose output
cout << g.dijkstraVerbose(0);
// Output:
// Dijkstra from vertex 0:
//   To 0: 0
//   To 1: 7
//   To 2: 5
```

### Example 2: Test Data Generation
```cpp
DataPrepare gen(42, "my_graphs");

// Generate adjacency matrix
auto matrix = gen.generateAdjacencyMatrix(
    20,      // vertices
    50,      // 50% density
    42,      // seed
    1, 100   // weights 1-100
);

// Save to CSV
gen.saveMatrixToCSV(matrix, "my_graphs/test.csv");

// Load from CSV
auto loaded = gen.loadMatrixFromCSV("my_graphs/test.csv");
```

### Example 3: Negative Weight Handling
```cpp
// Bellman-Ford handles negative weights
GraphList g(4);
g.addEdge(0, 1, 6);
g.addEdge(0, 2, 7);
g.addEdge(1, 2, 8);
g.addEdge(1, 3, -4);   // Negative edge
g.addEdge(2, 3, -5);   // Negative edge

auto [valid, dist] = g.bellmanFord(0);
// valid = true (no cycle)
// dist = [0, 6, 7, 2]
```

---

## Next Steps

1. **Build** the project (`cmake --build .`)
2. **Run** the program
3. **Generate** test data (menu option 4)
4. **Test** algorithms on generated graphs (menu option 3)
5. **Review** [DOCUMENTATION.md](DOCUMENTATION.md) for in-depth explanations
6. **Examine** source code in `src/` directory

---

## Key Classes & Methods

### GraphList
```cpp
GraphList g(vertices, directed=false);
g.addEdge(from, to, weight);
g.removeEdge(from, to);
std::vector<int> distances = g.dijkstra(source);
auto [valid, distances] = g.bellmanFord(source);
g.print();
```

### GraphMatrix
```cpp
GraphMatrix g(vertices, directed=false);
g.addEdge(from, to, weight);
g.removeEdge(from, to);
std::vector<int> distances = g.dijkstra(source);
auto [valid, distances] = g.bellmanFord(source);
g.print();
```

### DataPrepare
```cpp
DataPrepare gen(seed, output_dir);
gen.generateTestData(num_seeds);
gen.generateGraphCSV(vertices, density, seed, filename);
auto matrix = gen.loadMatrixFromCSV(filename);
gen.saveMatrixToCSV(matrix, filename);
```

### Menu
```cpp
Menu menu;
menu.run();  // Starts interactive menu loop
```

---

## References

- **Dijkstra's Algorithm:** Classic greedy shortest path algorithm
- **Bellman-Ford Algorithm:** Dynamic programming approach for shortest paths
- **Adjacency List:** Space-efficient graph representation
- **Adjacency Matrix:** Time-efficient graph representation
- **C++20 Features:** Structured bindings, smart pointers, concepts

---

For detailed explanations, see [DOCUMENTATION.md](DOCUMENTATION.md)
