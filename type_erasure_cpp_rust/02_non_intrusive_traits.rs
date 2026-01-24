use std::f64::consts::PI;

struct ThirdPartyCircle { radius: f64 }

trait Shape {
    fn area(&self) -> f64;
}

impl Shape for ThirdPartyCircle {
    fn area(&self) -> f64 { PI * self.radius * self.radius }
}

fn main() {
    let circle = ThirdPartyCircle { radius: 5.0 };
    println!("Area: {}", circle.area());
}
