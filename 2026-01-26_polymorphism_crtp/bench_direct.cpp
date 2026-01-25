// Benchmark: Direct call (baseline, no polymorphism)
// 
// Build:
//   clang++ -O2 -std=c++20 -march=native -o bench_direct bench_direct.cpp -lbenchmark -lpthread
//
// Assembly:
//   clang++ -O2 -std=c++20 -march=native -S bench_direct.cpp -o bench_direct.s

#include <benchmark/benchmark.h>
#include <random>
#include <vector>

constexpr std::size_t kNumShapes = 10000;

struct Square {
    double side;
    explicit Square(double s) : side(s) {}
    double getArea() const { return side * side; }
};

void bmDirect(benchmark::State& state) {
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(1.0, 10.0);
    std::vector<Square> squares;
    for (std::size_t i = 0; i < kNumShapes; ++i) {
        squares.emplace_back(dist(rng));
    }

    for (auto _ : state) {
        double total = 0.0;
        for (const auto& s : squares) {
            total += s.getArea();
        }
        benchmark::DoNotOptimize(total);
    }
}
BENCHMARK(bmDirect);

BENCHMARK_MAIN();
