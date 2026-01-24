use std::f64::consts::PI;

trait ShapeTrait {
    fn area(&self) -> f64;
}

struct Circle { radius: f64 }

impl ShapeTrait for Circle {
    fn area(&self) -> f64 { PI * self.radius * self.radius }
}

struct Square { side: f64 }

impl ShapeTrait for Square {
    fn area(&self) -> f64 { self.side * self.side }
}

struct Shape {
    inner: Box<dyn ShapeTrait>,
}

impl Shape {
    fn new<T: ShapeTrait + 'static>(shape: T) -> Self {
        Shape { inner: Box::new(shape) }
    }
    
    fn area(&self) -> f64 { self.inner.area() }
}

fn main() {
    let shapes: Vec<Shape> = vec![
        Shape::new(Circle { radius: 5.0 }),
        Shape::new(Square { side: 4.0 }),
    ];
    
    for shape in &shapes {
        println!("Area: {}", shape.area());
    }
}
