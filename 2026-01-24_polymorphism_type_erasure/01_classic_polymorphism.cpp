#include <cmath>
#include <iostream>
#include <vector>
#include <memory>

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
};

class Circle : public Shape {
    double radius_;
public:
    Circle(double r) : radius_(r) {}
    double area() const override { return M_PI * radius_ * radius_; }
};

class Square : public Shape {
    double side_;
public:
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
