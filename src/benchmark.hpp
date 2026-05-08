#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include "graph_list.hpp"
#include "graph_matrix.hpp"
#include "data_prepare.hpp"
#include <string>
#include <vector>
#include <chrono>

struct BenchmarkResult {
    std::vector<long long> times_ns;  // Individual run times in nanoseconds
    long long mean_time_ns;           // Mean time in nanoseconds
    long long min_time_ns;            // Minimum time
    long long max_time_ns;            // Maximum time
    int num_runs;                     // Number of test runs
    int num_vertices;
    int density_percent;
};

class Benchmark {
public:
    Benchmark(const std::string& data_dir = "data/main_seed");
    
    // Benchmark Dijkstra on multiple files
    BenchmarkResult benchmarkDijkstra(
        int num_vertices,
        int density_percent,
        int num_files,
        int source_vertex,
        bool use_matrix = false
    );
    
    // Print benchmark results
    void printResults(const BenchmarkResult& result, const std::string& algorithm_name) const;
    
private:
    std::string data_dir;
    
    // Helper to load file
    std::vector<std::vector<int>> loadGraphFile(int vertices, int density, int seed_idx) const;
    
    // Helper to format time
    std::string formatTime(long long ns) const;
};

#endif // BENCHMARK_HPP
