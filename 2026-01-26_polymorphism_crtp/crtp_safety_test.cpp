// CRTP safety idiom test
// 
// Demonstrates that the private constructor + friend pattern catches
// the common mistake of inheriting from the wrong CRTP instantiation.
//
// Build:
//   clang++ -std=c++20 crtp_safety_test.cpp
//
// Expected: compilation error on Wrong

template<typename Derived>
struct Shape {
private:
    Shape() = default;
    friend Derived;
    
public:
    double getArea() const {
        return static_cast<const Derived*>(this)->area();
    }
};

struct Right : Shape<Right> {
    double area() const { return 0.0; }
};

struct Wrong : Shape<Right> {  // Oops, meant Shape<Wrong>
    double area() const { return 0.0; }
};

int main() {
    Right r;   // OK
    Wrong w;   // Error: default constructor of 'Wrong' is implicitly deleted because
               //        base class 'Shape<Right>' has an inaccessible default constructor
}
