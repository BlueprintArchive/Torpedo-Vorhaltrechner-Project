

pub fn set_impact_angle(angle: f64) {
    impact_angle = angle;
    calculate_deflection_angle();
    angle_on_the_bow = 180.0 - (impact_angle + deflection_angle + parallax_correction);
    calculate_spread();
}

//Distance to target, 300m to 10,000m
pub fn set_distance_to_target( distance: f64) {
    distance_to_target = distance;
    calculate_spread();
}


//Set Torpedo Speed, 10kn to 50kn, calculate quotient
pub fn set_torpedo_speed(speed: f64) {
    torpedo_speed = speed;
    quotient = target_speed / torpedo_speed;
    calculate_deflection_angle();
    calculate_spread();
}

// Set Target Speed, calculated from observing two points and calculating difference in angle and time. 
pub fn set_target_speed(speed: f64) {
    target_speed = speed;
    quotient = target_speed / torpedo_speed;
    calculate_deflection_angle();
    calculate_spread();
}

//taken from periscope or added manually
pub fn set_target_bearing(angle: f64) {
    target_bearing = angle;
}



pub fn set_turn_ratio_correction(correction: f64) {
    turn_ratio_correction = correction;
}


pub fn calculate_deflection_angle() {
    let beta = 1.0 / (quotient * impact_angle.to_radians().sin()) - impact_angle.to_radians().cotan();
    
    if beta.is_nan() {
        deflection_angle = 0.0;
    } else {
        deflection_angle = (1.0 / beta).atan().to_degrees();
    }
}


pub fn set_parallax_forward( new_value: bool) {
    parallax_forward = new_value;
}


pub fn set_target_length( new_value: f64) {
    target_length = new_value;
    calculate_spread();
}


pub fn calculate_spread() {
    let angle = (target_length / distance_to_target)
        * (angle_on_the_bow.to_radians().sin())
        * ((angle_on_the_bow.to_radians().cos()
            / ((1.0 / quotient).powi(2)
                - (angle_on_the_bow.to_radians().sin()).powi(2))).sqrt()
            + 1.0);

    if angle.to_degrees() > 20.0 {
        spread_angle = 20.0;
    } else {
        spread_angle = angle.to_degrees();
    }
}


pub fn calculate_cartesian_coordinate_of_ideal_torpedo_start_point(& angle: f64) -> (f64, f64) {
    if parallax_forward {
        
        
        if angle >= 0.0 && angle <= 180.0 {
            let x = 27.0 + 9.5 + 95.0 * angle.to_radians().sin()
                - (angle.to_radians() * 95.0 + 9.5) * angle.to_radians().cos();
            let y = 95.0 * (1.0 - angle.to_radians().cos())
                - (angle.to_radians() * 95.0 + 9.5) * angle.to_radians().sin();
            (x, y)
        } 
        
        else {
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


pub fn calculate_polar_coordinate_of_ideal_torpedo_start_point(& x: f64, y: f64,) -> (f64, f64) {
    let X = (x.powi(2) + y.powi(2)).sqrt();
    let O = (y / x).atan();
    (X, O)
}


pub fn get_delta1(){
    parallax_correction
}


pub fn get_delta2() {
    if parallax_forward {
        delta2.to_degrees()
    } else {
        -delta2.to_degrees()
    }
}


pub fn get_maximum_distance_to_target(){
    let d = (impact_angle.to_radians().sin()
        / (impact_angle.to_radians() + deflection_angle.to_radians()).sin())
        .to_degrees();
    if d.is_nan() {
        1.0
    } else {
        d
    }
}


pub fn enable_parallax_motor( enable: bool) {
    enable_parallax_motor = enable;
}

fn calculate_deflection_angle() {
        
    let beta =  (1/(quotient * impact_angle.to_radians.sin())) - (1/impact_angle.to_radians.tan()).tan();
    
    
    if beta.is_nan() {
        deflection_angle = 0.0;
    } else {
        deflection_angle = (1.0 / beta).atan().to_degrees();
    }
}