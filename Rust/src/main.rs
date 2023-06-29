mod function_library;

use std::f64::consts::PI;
use std::f64;


fn radians(degrees: f64) -> f64 {
    degrees * PI / 180.0
}

fn degrees(radians: f64) -> f64 {
    radians * 180.0 / PI
}

fn truncate_decimals(num: f64, digits: usize) -> f64 {
    let num_s = num.to_string();
    let dec_pos = num_s.find('.').unwrap_or(num_s.len());
    let substr_length = if dec_pos == num_s.len() {
        num_s.len()
    } else {
        1 + dec_pos + digits
    };
    let trimmed_result = &num_s[..substr_length];
    let final_result = trimmed_result.parse::<f64>().unwrap_or(0.0);

    final_result
}

struct TorpedoVorhaltRechner {
    m_torpedo_speed: f64,
    m_target_speed: f64,
    m_parallax_correction: f64,
    m_distance_to_target: f64,
    m_quotient: f64,
    m_angle_on_the_bow: f64,
    m_impact_angle: f64,
    m_deflection_angle: f64,
    m_gyro_angle: f64,
    m_target_bearing: f64,
    m_turn_ratio_correction: f64,
    m_parallax_forward: bool,
    m_target_length: f64,
    m_spread_angle: f64,
    delta1: f64,
    delta2: f64,
    m_enable_parallax_motor: bool,
    m_manual_parallax: f64,
}

impl TorpedoVorhaltRechner {
    fn new() -> Self {
        TorpedoVorhaltRechner {
            m_torpedo_speed: 0.0,
            m_target_speed: 0.0,
            m_parallax_correction: 0.0,
            m_distance_to_target: 300.0,
            m_quotient: 0.0,
            m_angle_on_the_bow: 0.0,
            m_impact_angle: 0.0,
            m_deflection_angle: 0.0,
            m_gyro_angle: 0.0,
            m_target_bearing: 0.0,
            m_turn_ratio_correction: 0.0,
            m_parallax_forward: true,
            m_target_length: 100.0,
            m_spread_angle: 0.0,
            delta1: 0.0,
            delta2: 0.0,
            m_enable_parallax_motor: true,
            m_manual_parallax: 0.0,
        }
    }

    fn set_impact_angle(&mut self, angle: f64) {
        self.m_impact_angle = angle;
        self.calculate_deflection_angle();
        self.m_angle_on_the_bow = 180.0
            - (self.m_impact_angle + self.m_deflection_angle + self.m_parallax_correction);
        self.calculate_spread();
    }

    fn set_distance_to_target(&mut self, distance: f64) {
        self.m_distance_to_target = distance;
        self.calculate_spread();
    }

    fn set_torpedo_speed(&mut self, speed: f64) {
        self.m_torpedo_speed = speed;
        self.m_quotient = self.m_target_speed / self.m_torpedo_speed;
        self.calculate_deflection_angle();
        self.calculate_spread();
    }

    fn set_target_speed(&mut self, speed: f64) {
        self.m_target_speed = speed;
        self.m_quotient = self.m_target_speed / self.m_torpedo_speed;
        self.calculate_deflection_angle();
        self.calculate_spread();
    }

    fn set_target_bearing(&mut self, bearing: f64) {
        self.m_target_bearing = bearing;
        self.calculate_deflection_angle();
        self.calculate_spread();
    }

    fn set_gyro_angle(&mut self, gyro_angle: f64) {
        self.m_gyro_angle = gyro_angle;
        self.calculate_deflection_angle();
        self.calculate_spread();
    }

    fn calculate_deflection_angle(&mut self) {
        let factor: f64 = if self.m_parallax_forward {
            1.0
        } else {
            -1.0
        };
        let deflection: f64 = factor
            * ((self.m_quotient.sin() * self.m_target_speed.sin())
                / (self.m_distance_to_target / self.m_torpedo_speed).sin())
            .asin();
        self.m_deflection_angle = degrees(deflection);
    }

    fn calculate_spread(&mut self) {
        let d: f64 = self.m_distance_to_target;
        let s: f64 = self.m_torpedo_speed;
        let t: f64 = self.m_target_speed;
        let q: f64 = self.m_quotient;
        let b: f64 = self.m_target_bearing;
        let a: f64 = self.m_angle_on_the_bow;
        let g: f64 = self.m_gyro_angle;
        let l: f64 = self.m_target_length;

        let e: f64 = ((q - 1.0) * d * (a - b).to_radians().sin()) / s;
        let f: f64 = (q * d * b.to_radians().sin()) / s;
        let i: f64 = (q * d * a.to_radians().sin()) / s;
        let j: f64 = (q - 1.0) * d * b.to_radians().sin() / s;
        let m: f64 = (2.0 * q - 1.0) * d * a.to_radians().sin() / s;
        let n: f64 = (2.0 * q - 1.0) * d * b.to_radians().sin() / s;

        let c: f64 = (f / (1.0 - e)).acos();
        let k: f64 = (i / (1.0 + j)).acos();
        let o: f64 = (m / (1.0 - n)).acos();

        let p: f64 = c + k + o;

        let spread: f64 = (l * p.to_degrees()) / (2.0 * d);

        self.m_spread_angle = degrees(spread);
    }
}

fn main() {
    let mut rechner = TorpedoVorhaltRechner::new();
    rechner.set_torpedo_speed(33.0);
    rechner.set_target_speed(9.0);
    rechner.set_distance_to_target(1200.0);
    rechner.set_target_bearing(45.0);
    rechner.set_gyro_angle(5.0);
    rechner.set_impact_angle(90.0);

    let Vt: f64 = 9.0;
    let Vg: f64 = 33.0;
    let u: f64 = Vt/Vg;
    let alpha: f64 = 30.0; // Value for α (alpha) in degrees
    let beta: f64 = function_library::torpedo_triangle(u, alpha);
    println!("Beta: {} degrees", beta);

    println!("Spread Angle: {} degrees", rechner.m_spread_angle);
}
