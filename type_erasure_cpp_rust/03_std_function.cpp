#include <functional>
#include <iostream>

void print_result(std::function<int(int, int)> op) {
    std::cout << op(3, 4) << "\n";
}

int main() {
    // Different types, same interface
    print_result([](int a, int b) { return a + b; });     // Lambda
    print_result(std::multiplies<int>{});                 // Functor
    print_result([](int a, int b) { return a * b; });     // Different lambda
}
