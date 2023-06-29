

pub fn torpedo_triangle(u: f64, alpha: f64) -> f64 {
    let beta: f64 = (1.0 / u * alpha.sin() - alpha.tan()).atan(); // β
    beta.to_degrees()
}
