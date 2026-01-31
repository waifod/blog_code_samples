// Benchmark: Virtual dispatch with final classes
// 
// Tests whether marking classes as `final` allows the compiler to devirtualize.
//
// Build:
//   clang++ -O2 -std=c++20 -march=native -o bench_final bench_final.cpp -lbenchmark -lpthread
//
// Assembly:
//   clang++ -O2 -std=c++20 -march=native -S bench_final.cpp -o bench_final.s

#include <benchmark/benchmark.h>
#include <memory>
#include <random>
#include <vector>

constexpr std::size_t kNumShapes = 10000;

struct Shape {
    virtual ~Shape() = default;
    virtual double getArea() const = 0;
};

struct SquareFinal final : Shape {
    double side_;
    explicit SquareFinal(double s) : side_(s) {}
    double getArea() const override { return side_ * side_; }
};

struct TriangleFinal final : Shape {
    double base_, height_;
    TriangleFinal(double b, double h) : base_(b), height_(h) {}
    double getArea() const override { return 0.5 * base_ * height_; }
};

// Final class without pointer indirection
void bmFinalDirect(benchmark::State& state) {
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(1.0, 10.0);
    std::vector<SquareFinal> squares;
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
BENCHMARK(bmFinalDirect);

// Final class with unique_ptr indirection, homogeneous types
void bmFinalHomogeneous(benchmark::State& state) {
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(1.0, 10.0);
    std::vector<std::unique_ptr<Shape>> shapes;
    for (std::size_t i = 0; i < kNumShapes; ++i) {
        shapes.push_back(std::make_unique<SquareFinal>(dist(rng)));
    }

    for (auto _ : state) {
        double total = 0.0;
        for (const auto& shape : shapes) {
            total += shape->getArea();
        }
        benchmark::DoNotOptimize(total);
    }
}
BENCHMARK(bmFinalHomogeneous);

// Final classes with unique_ptr indirection, heterogeneous types
void bmFinalHeterogeneous(benchmark::State& state) {
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(1.0, 10.0);
    std::bernoulli_distribution coin(0.5);
    std::vector<std::unique_ptr<Shape>> shapes;

    for (std::size_t i = 0; i < kNumShapes; ++i) {
        if (coin(rng)) {
            shapes.push_back(std::make_unique<SquareFinal>(dist(rng)));
        } else {
            shapes.push_back(std::make_unique<TriangleFinal>(dist(rng), dist(rng)));
        }
    }

    for (auto _ : state) {
        double total = 0.0;
        for (const auto& shape : shapes) {
            total += shape->getArea();
        }
        benchmark::DoNotOptimize(total);
    }
}
BENCHMARK(bmFinalHeterogeneous);

BENCHMARK_MAIN();
