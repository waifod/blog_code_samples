#include <cmath>
#include <memory>
#include <iostream>
#include <vector>

class Shape {
    struct Concept {
        virtual ~Concept() = default;
        virtual double area() const = 0;
    };
    
    template<typename T>
    struct Model : Concept {
        T data;
        Model(T value) : data(std::move(value)) {}
        double area() const override { return data.area(); }
    };
    
    std::unique_ptr<Concept> object;

public:
    template<typename T>
    Shape(T obj) : object(std::make_unique<Model<T>>(std::move(obj))) {}
    
    double area() const { return object->area(); }
};

// Concrete types - no inheritance required
struct Circle {
    double radius;
    double area() const { return M_PI * radius * radius; }
};

struct Square {
    double side;
    double area() const { return side * side; }
};

int main() {
    std::vector<Shape> shapes;  // Value semantics!
    shapes.emplace_back(Circle{5.0});
    shapes.emplace_back(Square{4.0});
    
    for (const auto& shape : shapes) {
        std::cout << "Area: " << shape.area() << "\n";
    }
}
