#include <cmath>
#include <iostream>
#include <vector>
#include <memory>

struct Shape {
    virtual ~Shape() = default;
    virtual double area() const = 0;
};

struct Circle : Shape {
    double radius_;
    Circle(double r) : radius_(r) {}
    double area() const override { return M_PI * radius_ * radius_; }
};

struct Square : Shape {
    double side_;
    Square(double s) : side_(s) {}
    double area() const override { return side_ * side_; }
};

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.emplace_back(std::make_unique<Circle>(5.0));
    shapes.emplace_back(std::make_unique<Square>(4.0));
    
    for (const auto& shape : shapes) {
        std::cout << "Area: " << shape->area() << "\n";
    }
}
