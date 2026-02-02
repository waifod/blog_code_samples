// Standalone version of bench_direct to examine assembly without benchmark
// framework
//
// Build:
//   clang++ -O2 -std=c++20 -march=native -o direct_standalone
//   direct_standalone.cpp
//
// Assembly:
//   clang++ -O2 -std=c++20 -march=native -S direct_standalone.cpp -o
//   direct_standalone.s

#include <cstddef>
#include <random>
#include <vector>

constexpr std::size_t kNumShapes = 10000;

struct Square {
  double side_;
  Square(double s) : side_(s) {}
  double getArea() const { return side_ * side_; }
};

// Prevent inlining so we can see the loop in isolation
__attribute__((noinline)) double sumAreas(const std::vector<Square> &squares) {
  double total = 0.0;
  for (const auto &s : squares) {
    total += s.getArea();
  }
  return total;
}

int main() {
  std::mt19937 rng(42);
  std::uniform_real_distribution<double> dist(1.0, 10.0);

  std::vector<Square> squares;
  for (std::size_t i = 0; i < kNumShapes; ++i) {
    squares.emplace_back(dist(rng));
  }

  // Use volatile to prevent the call from being optimized away
  volatile double result = sumAreas(squares);
}
