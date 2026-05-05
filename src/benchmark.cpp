#include "benchmark.hpp"
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <algorithm>
#include <numeric>

namespace fs = std::filesystem;

Benchmark::Benchmark(const std::string& data_dir)
    : data_dir(data_dir) {}

std::vector<std::vector<int>> Benchmark::loadGraphFile(int vertices, int density, int seed_idx) const {
    std::string filename = data_dir + "/graph_v" + std::to_string(vertices) +
                          "_d" + std::to_string(density) +
                          "_s" + std::to_string(seed_idx) + ".csv";
    
    DataPrepare prep(0, data_dir);
    return prep.loadMatrixFromCSV(filename);
}

BenchmarkResult Benchmark::benchmarkDijkstra(
    int num_vertices,
    int density_percent,
    int num_files,
    int source_vertex,
    bool use_matrix)
{
    BenchmarkResult result;
    result.num_vertices = num_vertices;
    result.density_percent = density_percent;
    result.num_runs = 0;
    result.mean_time_ns = 0;
    result.min_time_ns = LLONG_MAX;
    result.max_time_ns = 0;
    
    if (source_vertex < 0 || source_vertex >= num_vertices) {
        std::cerr << "Invalid source vertex\n";
        return result;
    }
    
    // Run benchmarks on each file
    for (int i = 0; i < num_files; i++) {
        try {
            auto matrix = loadGraphFile(num_vertices, density_percent, i);
            
            if (use_matrix) {
                GraphMatrix g(num_vertices, false);
                g.loadFromMatrix(matrix);
                
                auto start = std::chrono::high_resolution_clock::now();
                auto distances = g.dijkstra(source_vertex);
                auto end = std::chrono::high_resolution_clock::now();
                
                long long elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                result.times_ns.push_back(elapsed_ns);
                result.min_time_ns = std::min(result.min_time_ns, elapsed_ns);
                result.max_time_ns = std::max(result.max_time_ns, elapsed_ns);
                result.num_runs++;
            } else {
                GraphList g(num_vertices, false);
                g.loadFromMatrix(matrix);
                
                auto start = std::chrono::high_resolution_clock::now();
                auto distances = g.dijkstra(source_vertex);
                auto end = std::chrono::high_resolution_clock::now();
                
                long long elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                result.times_ns.push_back(elapsed_ns);
                result.min_time_ns = std::min(result.min_time_ns, elapsed_ns);
                result.max_time_ns = std::max(result.max_time_ns, elapsed_ns);
                result.num_runs++;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error loading file " << i << ": " << e.what() << "\n";
        }
    }
    
    // Calculate mean time
    if (result.num_runs > 0) {
        long long sum = std::accumulate(result.times_ns.begin(), result.times_ns.end(), 0LL);
        result.mean_time_ns = sum / result.num_runs;
    }
    
    return result;
}

BenchmarkResult Benchmark::benchmarkBellmanFord(
    int num_vertices,
    int density_percent,
    int num_files,
    int source_vertex,
    bool use_matrix)
{
    BenchmarkResult result;
    result.num_vertices = num_vertices;
    result.density_percent = density_percent;
    result.num_runs = 0;
    result.mean_time_ns = 0;
    result.min_time_ns = LLONG_MAX;
    result.max_time_ns = 0;
    
    if (source_vertex < 0 || source_vertex >= num_vertices) {
        std::cerr << "Invalid source vertex\n";
        return result;
    }
    
    // Run benchmarks on each file
    for (int i = 0; i < num_files; i++) {
        try {
            auto matrix = loadGraphFile(num_vertices, density_percent, i);
            
            if (use_matrix) {
                GraphMatrix g(num_vertices, false);
                g.loadFromMatrix(matrix);
                
                auto start = std::chrono::high_resolution_clock::now();
                auto [valid, distances] = g.bellmanFord(source_vertex);
                auto end = std::chrono::high_resolution_clock::now();
                
                long long elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                result.times_ns.push_back(elapsed_ns);
                result.min_time_ns = std::min(result.min_time_ns, elapsed_ns);
                result.max_time_ns = std::max(result.max_time_ns, elapsed_ns);
                result.num_runs++;
            } else {
                GraphList g(num_vertices, false);
                g.loadFromMatrix(matrix);
                
                auto start = std::chrono::high_resolution_clock::now();
                auto [valid, distances] = g.bellmanFord(source_vertex);
                auto end = std::chrono::high_resolution_clock::now();
                
                long long elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                result.times_ns.push_back(elapsed_ns);
                result.min_time_ns = std::min(result.min_time_ns, elapsed_ns);
                result.max_time_ns = std::max(result.max_time_ns, elapsed_ns);
                result.num_runs++;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error loading file " << i << ": " << e.what() << "\n";
        }
    }
    
    // Calculate mean time
    if (result.num_runs > 0) {
        long long sum = std::accumulate(result.times_ns.begin(), result.times_ns.end(), 0LL);
        result.mean_time_ns = sum / result.num_runs;
    }
    
    return result;
}

std::string Benchmark::formatTime(long long ns) const {
    if (ns < 1000) {
        return std::to_string(ns) + " ns";
    } else if (ns < 1000000) {
        return std::to_string(ns / 1000) + " us (" + std::to_string(ns) + " ns)";
    } else if (ns < 1000000000) {
        return std::to_string(ns / 1000000) + " ms (" + std::to_string(ns) + " ns)";
    } else {
        return std::to_string(ns / 1000000000) + " s (" + std::to_string(ns) + " ns)";
    }
}

void Benchmark::printResults(const BenchmarkResult& result, const std::string& algorithm_name) const {
    std::cout << "\n";
    std::cout << "[----------------------------------------------------------------]\n";
    std::cout << "[              BENCHMARK RESULTS - " << std::left << std::setw(38) << algorithm_name << "]\n";
    std::cout << "[----------------------------------------------------------------]\n";
    
    std::cout << "Configuration:\n";
    std::cout << "  Vertices:        " << result.num_vertices << "\n";
    std::cout << "  Density:         " << result.density_percent << "%\n";
    std::cout << "  Test Runs:       " << result.num_runs << " files\n";
    
    if (result.num_runs == 0) {
        std::cout << "\nNo files found to benchmark!\n";
        return;
    }
    
    std::cout << "\nTiming Results:\n";
    std::cout << "  Mean Time:       " << formatTime(result.mean_time_ns) << "\n";
    std::cout << "  Min Time:        " << formatTime(result.min_time_ns) << "\n";
    std::cout << "  Max Time:        " << formatTime(result.max_time_ns) << "\n";
    std::cout << "  Difference:      " << formatTime(result.max_time_ns - result.min_time_ns) << "\n";
    
    std::cout << "\nIndividual Run Times (in nanoseconds):\n";
    for (int i = 0; i < result.num_runs; i++) {
        std::cout << "  Run " << std::setw(3) << (i + 1) << ": " 
                  << std::setw(12) << result.times_ns[i] << " ns";
        
        // Show relative to mean
        if (result.mean_time_ns > 0) {
            double percent = ((double)result.times_ns[i] / result.mean_time_ns - 1.0) * 100.0;
            if (percent >= 0) {
                std::cout << " (+";
            } else {
                std::cout << " (";
            }
            std::cout << std::fixed << std::setprecision(1) << percent << "%)";
        }
        std::cout << "\n";
    }
    
    std::cout << "\n";
}
