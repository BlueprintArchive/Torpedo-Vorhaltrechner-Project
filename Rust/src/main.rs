struct TorpedoVorhaltRechner {
    torpedo_speed: f64,
    target_speed: f64,
    parallax_correction: f64,
    distance_to_target: f64,
    quotient: f64,
    angle_on_the_bow: f64,
    impact_angle: f64,
    deflection_angle: f64,
    gyro_angle: f64,
    target_bearing: f64,
    turn_ratio_correction: f64,
    parallax_forward: bool,
    target_length: f64,
    spread_angle: f64,
    polar_coordinate: f64,
    cartesan_coordinate: f64,
    delta1: f64,
    delta2: f64,
    enable_parallax_motor: bool,
    manual_parallax: f64,
}

impl TorpedoVorhaltRechner {
    fn new() -> TorpedoVorhaltRechner {
        TorpedoVorhaltRechner {
            torpedo_speed: 0.0,
            target_speed: 0.0,
            parallax_correction: 0.0,
            distance_to_target: 300.0,
            quotient: 0.0,
            angle_on_the_bow: 0.0,
            impact_angle: 0.0,
            deflection_angle: 0.0,
            gyro_angle: 0.0,
            target_bearing: 0.0,
            turn_ratio_correction: 0.0,
            parallax_forward: true,
            target_length: 100.0,
            spread_angle: 0.0,
            polar_coordinate: 0.0,
            cartesan_coordinate: 0.0,
            delta1: 0.0,
            delta2: 0.0,
            enable_parallax_motor: true,
            manual_parallax: 0.0,
        }
    }

    //inputs -----------------------------------------------------
    fn set_impact_angle(&mut self, angle: f64) {
        self.impact_angle = angle;
        self.calculate_deflection_angle();
        self.angle_on_the_bow = 180.0 - (self.impact_angle + self.deflection_angle + self.parallax_correction);
        self.calculate_spread();
    }

    fn set_distance_to_target(&mut self, distance: f64) {
        self.distance_to_target = distance;
        self.calculate_spread();
    }

    fn set_torpedo_speed(&mut self, speed: f64) {
        self.torpedo_speed = speed;
        self.quotient = self.target_speed / self.torpedo_speed;
        self.calculate_deflection_angle();
        self.calculate_spread();
    }

    fn set_target_speed(&mut self, speed: f64) {
        self.target_speed = speed;
        self.quotient = self.target_speed / self.torpedo_speed;
        self.calculate_deflection_angle();
        self.calculate_spread();
    }

    fn set_target_bearing(&mut self, angle: f64) {
        self.target_bearing = angle;
    }

    fn set_turn_ratio_correction(&mut self, correction: f64) {
        self.turn_ratio_correction = correction;
    }

    fn enable_parallax_motor(&mut self, enable: bool) {
        self.enable_parallax_motor = enable;
    }
    
    fn set_parallax_forward(&mut self, new_value: bool) {
        self.parallax_forward = new_value;
    }

    fn set_target_length(&mut self, new_value: f64) {
        self.target_length = new_value;
        self.calculate_spread();
    }
    
    //torpedo triangle
    fn calculate_deflection_angle(&mut self) {
        
        let beta = 
        
        1.0 / 
        (self.quotient * (self.impact_angle.to_radians().sin()))
        - 1.0 / (self.impact_angle.to_radians().tan());
        
        
        if beta.is_nan() {
            self.deflection_angle = 0.0;
        } else {
            self.deflection_angle = (1.0 / beta).atan().to_degrees();
        }
    }

    fn calculate_spread(&mut self) {
        let angle = (self.target_length / self.distance_to_target)
            * (self.angle_on_the_bow.to_radians().sin())
            * ((self.angle_on_the_bow.to_radians().cos()
                / ((1.0 / self.quotient).powi(2)
                    - (self.angle_on_the_bow.to_radians().sin()).powi(2))).sqrt()
                + 1.0);

        if angle.to_degrees() > 20.0 {
            self.spread_angle = 20.0;
        } else {
            self.spread_angle = angle.to_degrees();
        }
    }

    fn calculate_cartesian_coordinate_of_ideal_torpedo_start_point(&self, angle: f64) -> (f64, f64) {
        if self.parallax_forward {
            if angle >= 0.0 && angle <= 180.0 {
                let x = 27.0 + 9.5 + 95.0 * angle.to_radians().sin()
                    - (angle.to_radians() * 95.0 + 9.5) * angle.to_radians().cos();
                let y = 95.0 * (1.0 - angle.to_radians().cos())
                    - (angle.to_radians() * 95.0 + 9.5) * angle.to_radians().sin();
                (x, y)
            } else {
                let x = 27.0 + 9.5 + 95.0 * (360.0 - angle).to_radians().sin()
                    - ((360.0 - angle).to_radians() * 95.0 + 9.5)
                        * (360.0 - angle).to_radians().cos();
                let y = -(95.0 * (1.0 - (360.0 - angle).to_radians().cos())
                    - ((360.0 - angle).to_radians() * 95.0 + 9.5)
                        * (360.0 - angle).to_radians().sin());
                (x, y)
            }
        } else {
            if angle >= 0.0 && angle <= 180.0 {
                let x = -(27.0 + 9.5 + 95.0 * (180.0 - angle).to_radians().sin()
                    - ((180.0 - angle).to_radians() * 95.0 + 9.5)
                        * (180.0 - angle).to_radians().cos());
                let y = 95.0 * (1.0 - (180.0 - angle).to_radians().cos())
                    - ((180.0 - angle).to_radians() * 95.0 + 9.5)
                        * (180.0 - angle).to_radians().sin();
                (x, y)
            } else {
                let x = -(27.0 + 9.5 + 95.0 * (angle - 180.0).to_radians().sin()
                    - ((angle - 180.0).to_radians() * 95.0 + 9.5)
                        * (angle - 180.0).to_radians().cos());
                let y = -(95.0 * (1.0 - (angle - 180.0).to_radians().cos())
                    - ((angle - 180.0).to_radians() * 95.0 + 9.5)
                        * (angle - 180.0).to_radians().sin());
                (x, y)
            }
        }
    }

    fn calculate_polar_coordinate_of_ideal_torpedo_start_point(&self, x: f64, y: f64,) -> (f64, f64) {
        let X = (x.powi(2) + y.powi(2)).sqrt();
        let O = (y / x).atan();
        (X, O)
    }

    fn get_delta1(&self) -> f64 {
        self.parallax_correction
    }

    fn calculate_parallax_correction(&mut self, target: f64, parallax: f64, theta: f64) {

        self.parallax_correction = (self.target_bearing + parallax).to_radians().sin() / self.distance_to_target

    }

    fn get_delta2(&self) -> f64 {
        if self.parallax_forward {
            self.delta2.to_degrees()
        } else {
            -self.delta2.to_degrees()
        }
    }

    fn get_maximum_distance_to_target(&self) -> f64 {
        let d = (self.impact_angle.to_radians().sin()
            / (self.impact_angle.to_radians() + self.deflection_angle.to_radians()).sin())
            .to_degrees();
        if d.is_nan() {
            1.0
        } else {
            d
        }
    }

    //finally, output of Gyro Angle 
    fn set_gyro_angle(&mut self) {
        self.gyro_angle = self.target_bearing + self.parallax_correction - self.deflection_angle;

    }
}

//made a function outside of struct to test

fn calculate_deflection_angle() {
    
    let a: f64 = 5.0;
    let u: f64 = 2.0;
    
    let numerator: f64 = 1.0 / (u * a.sin());
    let denominator: f64 = a.cos() / a.sin();
    let arcctg_arg: f64 = numerator - denominator;
    let beta: f64 = arcctg_arg.atan();

    if beta.is_nan() {
        let beta: f64 = 0.0;
    } else {
        println!("{beta}")
    }
    

}

fn main() {

    fn TVRE() {
        let mut TVRE = TorpedoVorhaltRechner::new();

        TVRE.set_target_bearing(58.30);
        TVRE.set_impact_angle(54.4);
        TVRE.set_distance_to_target(2000.0);
        TVRE.set_torpedo_speed(44.0);
        TVRE.set_target_speed(35.2);
        TVRE.set_turn_ratio_correction(2.0);
        TVRE.set_parallax_forward(true);
        TVRE.set_target_length(130.0);
        TVRE.set_gyro_angle();
    
        println!("Target Bearing: {}", TVRE.target_bearing);
        println!("Target Speed: {}", TVRE.target_speed);
        println!("Torpedo Speed: {}", TVRE.torpedo_speed);
        println!("Quotient: {}", TVRE.quotient);
        println!("Torpedo Impact Angle: {}", TVRE.impact_angle);
        println!("AOB: {}", TVRE.angle_on_the_bow);
        println!("Deflection Angle: {}", TVRE.deflection_angle);
        println!("Gyro Angle: {}", TVRE.gyro_angle);
    }
}
