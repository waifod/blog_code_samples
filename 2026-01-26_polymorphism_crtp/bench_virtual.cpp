// Benchmark: Virtual dispatch (runtime polymorphism)
// 
// Build:
//   clang++ -O2 -std=c++20 -march=native -o bench_virtual bench_virtual.cpp -lbenchmark -lpthread
//
// Assembly:
//   clang++ -O2 -std=c++20 -march=native -S bench_virtual.cpp -o bench_virtual.s

#include <benchmark/benchmark.h>
#include <memory>
#include <random>
#include <vector>

constexpr std::size_t kNumShapes = 10000;

struct Shape {
    virtual ~Shape() = default;
    virtual double getArea() const = 0;
};

struct Square : Shape {
    double side;
    explicit Square(double s) : side(s) {}
    double getArea() const override { return side * side; }
};

struct Triangle : Shape {
    double base, height;
    Triangle(double b, double h) : base(b), height(h) {}
    double getArea() const override { return 0.5 * base * height; }
};

// Virtual dispatch without pointer indirection
void bmVirtualDirect(benchmark::State& state) {
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
BENCHMARK(bmVirtualDirect);

// Virtual dispatch with unique_ptr indirection, homogeneous types
void bmVirtualHomogeneous(benchmark::State& state) {
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(1.0, 10.0);
    std::vector<std::unique_ptr<Shape>> shapes;
    for (std::size_t i = 0; i < kNumShapes; ++i) {
        shapes.push_back(std::make_unique<Square>(dist(rng)));
    }

    for (auto _ : state) {
        double total = 0.0;
        for (const auto& shape : shapes) {
            total += shape->getArea();
        }
        benchmark::DoNotOptimize(total);
    }
}
BENCHMARK(bmVirtualHomogeneous);

// Virtual dispatch with unique_ptr indirection, heterogeneous types
void bmVirtualHeterogeneous(benchmark::State& state) {
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(1.0, 10.0);
    std::bernoulli_distribution coin(0.5);
    std::vector<std::unique_ptr<Shape>> shapes;

    for (std::size_t i = 0; i < kNumShapes; ++i) {
        if (coin(rng)) {
            shapes.push_back(std::make_unique<Square>(dist(rng)));
        } else {
            shapes.push_back(std::make_unique<Triangle>(dist(rng), dist(rng)));
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
BENCHMARK(bmVirtualHeterogeneous);

BENCHMARK_MAIN();
