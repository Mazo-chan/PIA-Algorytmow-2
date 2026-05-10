# Quick Reference Guide

## One-Minute Overview

This C++ project implements **shortest path algorithm** for weighted graphs using two different data structures.

### What It Does
- Finds shortest paths between vertices in a graph using Dijkstra's algorithm
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
3. **Test Algorithms** - Run Dijkstra shortest path algorithm
4. **Generate Test Data** - Create random graphs for testing
5. **Exit** - Quit program

---

## Key Concepts

### Two Graph Representations

| Feature | Adjacency List | Adjacency Matrix |
|---------|---------------|------------------|
| **Memory** | O(V+E) - Only stores edges | O(V²) - Stores all pairs |
| **Edge Access** | O(degree) average | O(1) - Direct lookup |
| **Add Edge** | O(1) | O(1) |
| **Dijkstra** | O((V+E)logV) - FAST | O(V²logV) - SLOW |
| **Best for** | Sparse graphs (E << V²) | Dense graphs (E ≈ V²) |

**Why GraphMatrix is O(V²logV):**
```cpp
// GraphList: iterate only over existing edges
for (const auto& edge : adjacency_list[u]) {  // O(E) total iterations
    // ... process edge
}

// GraphMatrix: iterate over ALL vertices
for (int v = 0; v < vertices; v++) {  // O(V²) total iterations!
    if (adjacency_matrix[u][v] != INF) {  // Check all pairs
        // ... process edge
    }
}
```

### Two Shortest Path Algorithms

**Dijkstra's Algorithm:**

| Feature | Details |
|---------|---------|
| **Purpose** | Find shortest paths with non-negative weights |
| **Time Complexity** | O((V+E)logV) with binary heap |
| **Negative Weights** | ❌ Not supported |
| **Cycle Detection** | ❌ No |
| **Best For** | Sparse graphs, fast solutions |



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

### Task 1: Understand Priority Queue in Dijkstra

**What is the min-heap?**
```cpp
// Priority queue with std::greater creates a MIN-heap
std::priority_queue<HeapNode, ..., std::greater<HeapNode>> pq;

// Comparison: HeapNode::operator>
bool operator>(const HeapNode& other) const {
    return distance > other.distance;
}
// Returns true if "this" needs to be ordered AFTER "other"
// This creates min-heap: smallest distance at top

// Example:
pq.push({10, vertex5, path});
pq.push({3, vertex2, path});
pq.push({7, vertex1, path});

pq.top();  // Returns {3, vertex2, ...} - SMALLEST distance!
```

**How it improves performance:**
- Without heap: O(V²) - search all vertices for minimum each time
- With heap: O(V log V) - extract minimum in O(log V) time
- Saves: O(V²) - O(V log V) = significant speedup for sparse graphs!

### Task 2: Create and Test a Graph
```cpp
GraphList g(5, false);  // 5 vertices, undirected
g.addEdge(0, 1, 5);
g.addEdge(1, 2, 3);
g.addEdge(0, 2, 9);

auto result = g.dijkstra(0);  // From vertex 0
// result.results[0] = {0, [0]}       - distance 0, path [0]
// result.results[1] = {5, [0,1]}     - distance 5, path [0,1]
// result.results[2] = {8, [0,1,2]}   - distance 8, path [0,1,2]
```

### Task 3: Compare Representations
```cpp
// SPARSE GRAPH: 100 vertices, 150 edges (1.5% of possible)
GraphList sparse(100, false);     // ✅ Use list: O((V+E)logV) ≈ O(1.25K log V)
GraphMatrix sparse_wrong(100);    // ❌ Use matrix: O(V²logV) ≈ O(10K log V) - 8x slower!

// DENSE GRAPH: 100 vertices, 5000 edges (50% of possible)
GraphList dense(100, false);      // ❌ Use list: still works but slower
GraphMatrix dense(100);           // ✅ Use matrix: better cache locality, O(1) lookup
```

---

## Algorithm Reference

### Dijkstra's Algorithm (Priority Queue Implementation)

**Purpose:** Find shortest paths (non-negative weights only)
**Time Complexity:** 
- GraphList: O((V+E)logV) 
- GraphMatrix: O(V²logV)
- Without heap: O(V²)

**How It Works:**
1. Start at source vertex (distance = 0)
2. Repeat: Extract closest unvisited vertex from min-heap
3. For each neighbor: if new path is shorter, update and add to heap
4. Continue until all reachable vertices processed

**Key Insight - Min-Heap Priority Queue:**
```cpp
std::priority_queue<HeapNode, std::vector<HeapNode>, std::greater<HeapNode>> pq;
// std::greater makes it a MIN-heap (smallest at top)
// pq.top() returns HeapNode with smallest distance
// Each push/pop is O(log V)
```

**Step-by-Step Example:**
```
Graph:    0 --(5)-- 1      Dijkstra from 0:
          |  \      |
         (1) (2)   (3)    Initial:  pq = [(dist:0, vertex:0)]
          |  /      |
          2 --(4)-- 3      Step 1: Extract (0,0)
                           Add (1,2) and (5,1) to pq
                           pq = [(1,2), (5,1)]
                           
                           Step 2: Extract (1,2)
                           Add (5,3) to pq
                           pq = [(5,1), (5,3)]
                           
                           Step 3-4: Extract (5,1), (5,3)
                           No improvements
                           
                           Final distances: [0, 5, 1, 5]
```

**Complexity Why:**
- **With heap:** V iterations of `top()/pop()` = O(V log V)
                 E edge updates with `push()` = O(E log V)
                 **Total:** O((V+E) log V)
  
- **Without heap:** V iterations of linear min-find = O(V²) ✗ Slower!

**Choose Dijkstra When:**
- ✅ All edge weights are positive
- ✅ Need fast shortest path (esp. sparse graphs)
- ✅ Single-source to all destinations

**Avoid When:**
- ❌ Negative edge weights present
- ❌ Need cycle detection

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
- **Dijkstra** → Optimal for shortest paths with non-negative weights

### Input Limits
- **Max Vertices:** 10,000+ (depends on representation)
- **Edge Weight Range:** 1-100 (positive integers)
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

## Decision Guide

### Which Graph Representation?

```
Do you know the number of edges?
├─ YES, E << V² (sparse)
│  └─→ Use GraphList: O(V+E) memory, faster Dijkstra
│
└─ NO or E ≈ V² (dense)
   └─→ Use GraphMatrix: O(V²) memory, O(1) edge lookup
```

**Sparse Graph Examples:**
- Social networks: 1M users, 10M friendships (0.001% dense)
- Road networks: 1K cities, 10K roads (1% dense)
- → Use **GraphList**

**Dense Graph Examples:**
- Complete graphs: All vertices connected
- Flight routes: 100 airports, most have direct flights (>50%)
- → Use **GraphMatrix**

### Which Algorithm?

**Dijkstra's Algorithm is the primary choice for this project.**

It provides:
- ✅ Fast shortest paths: O((V+E)logV)
- ✅ Optimal for sparse graphs
- ✅ Priority queue-based for efficiency
- ⚠️ Requires non-negative edge weights only

### Performance Comparison

| Scenario | GraphList + Dijkstra | GraphMatrix + Dijkstra | Notes |
|----------|---------------------|----------------------|-------|
| Sparse (V=1000, E=1500) | ⚡ 0.01ms | 🐢 10ms | List is 1000x faster |
| Dense (V=100, E=5000) | 🐢 1ms | ⚡ 0.5ms | Matrix better for dense |
| Large sparse (V=10K, E=50K) | ⚡ ~1ms | 🐌 ~100ms | List scales well |
| Complete graph (V=100) | 🐢 ~2ms | ⚡ ~0.5ms | Matrix optimal |

---

## Troubleshooting Quick Tips

| Problem | Root Cause | Solution |
|---------|-----------|----------|
| "All distances INF" | Graph is disconnected | Use provided data generator (auto-connects) |
| Wrong distances returned | Non-negative weight requirement violated | Verify all edge weights are >= 0 |
| Dijkstra times out | Wrong representation for graph | Use GraphList for sparse graphs |
| Memory error | Graph too large for matrix | Use GraphList for large sparse graphs |
| Slow performance | Using wrong representation | Use GraphMatrix for dense graphs |
6. **Examine** source code in `src/` directory

---

## Key Classes & Methods

### GraphList
```cpp
GraphList g(vertices, directed=false);
g.addEdge(from, to, weight);
g.removeEdge(from, to);
DijkstraResult result = g.dijkstra(source);
std::string verbose = g.dijkstraVerbose(source);
std::string pathInfo = g.dijkstraPathVerbose(source, destination);
g.print();
```

### GraphMatrix
```cpp
GraphMatrix g(vertices, directed=false);
g.addEdge(from, to, weight);
g.removeEdge(from, to);
DijkstraResult result = g.dijkstra(source);
std::string verbose = g.dijkstraVerbose(source);
std::string pathInfo = g.dijkstraPathVerbose(source, destination);
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

- **Dijkstra's Algorithm:** Greedy shortest path algorithm with priority queue optimization
- **Priority Queue / Min-Heap:** O(log V) operations for efficient vertex extraction
- **Adjacency List:** Space-efficient O(V+E) graph representation for sparse graphs
- **Adjacency Matrix:** Time-efficient O(1) edge lookup for dense graphs
- **C++20 Features:** Structured bindings, smart pointers, auto keyword

---

For detailed explanations, see [DOCUMENTATION.md](DOCUMENTATION.md)
