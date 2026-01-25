#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

// Existing types - they don't inherit from anything
struct Circle {
    double radius;
    double area() const { return M_PI * radius * radius; }
};

struct Square {
    double side;
    double area() const { return side * side; }
};

// External polymorphism: wrap types in a polymorphic interface
struct ShapeWrapper {
    virtual ~ShapeWrapper() = default;
    virtual double area() const = 0;
};

template<typename T>
struct ShapeWrapperImpl : ShapeWrapper {
    T shape_;
    ShapeWrapperImpl(T s) : shape_(std::move(s)) {}
    double area() const override { return shape_.area(); }
};

int main() {
    std::vector<std::unique_ptr<ShapeWrapper>> shapes;
    shapes.emplace_back(std::make_unique<ShapeWrapperImpl<Circle>>(Circle{5.0}));
    shapes.emplace_back(std::make_unique<ShapeWrapperImpl<Square>>(Square{4.0}));
    
    for (const auto& shape : shapes) {
        std::cout << "Area: " << shape->area() << "\n";
    }
}
