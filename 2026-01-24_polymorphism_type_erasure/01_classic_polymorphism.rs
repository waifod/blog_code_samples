use std::f64::consts::PI;

trait Shape {
    fn area(&self) -> f64;
}

struct Circle { radius: f64 }

impl Shape for Circle {
    fn area(&self) -> f64 { PI * self.radius * self.radius }
}

struct Square { side: f64 }

impl Shape for Square {
    fn area(&self) -> f64 { self.side * self.side }
}

fn main() {
    let shapes: Vec<Box<dyn Shape>> = vec![
        Box::new(Circle { radius: 5.0 }),
        Box::new(Square { side: 4.0 }),
    ];
    
    for shape in &shapes {
        println!("Area: {}", shape.area());
    }
}
