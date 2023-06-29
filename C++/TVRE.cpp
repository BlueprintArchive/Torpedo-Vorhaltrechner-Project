#include <iostream>
#include <cmath>

double radians(double degrees) {
    return degrees * M_PI / 180;
}

double degrees(double radians) {
    return radians * 180 / M_PI;
}

double truncateDecimals(double num, int digits) {
    std::string numS = std::to_string(num);
    size_t decPos = numS.find('.');
    size_t substrLength = (decPos == std::string::npos) ? numS.length() : 1 + decPos + digits;
    std::string trimmedResult = numS.substr(0, substrLength);
    double finalResult = std::stod(trimmedResult);
    return finalResult;
}

class TorpedoVorhaltRechner {
private:
    double m_torpedoSpeed;
    double m_targetSpeed;
    double m_parallaxCorrection;
    double m_distanceToTarget;   // 300 - 10000 m
    double m_quotient;
    double m_angleOnTheBow;
    double m_impactAngle;
    double m_deflectionAngle;
    double m_gyroAngle;
    double m_targetBearing;
    double m_turnRatioCorrection;
    bool m_parallaxForward;
    double m_targetLength;   // 100 - 200 m
    double m_spreadAngle;

    double delta1;
    double delta2;

    bool m_enableParallaxMotor;
    double m_manualParallax;

    void calculateDeflectionAngle() {
        double ctg_beta = (1 / (m_quotient * std::sin(radians(m_impactAngle))) - 1 / std::tan(radians(m_impactAngle)));
        if (std::isnan(ctg_beta))
            m_deflectionAngle = 0;
        else
            m_deflectionAngle = degrees(std::atan(1 / ctg_beta));
    }

    void calculateSpread() {
        double angle = (m_targetLength / m_distanceToTarget) * std::sin(radians(m_angleOnTheBow)) *
            ((std::cos(radians(m_angleOnTheBow)) / std::sqrt(std::pow(1 / m_quotient, 2) -
            std::pow(std::sin(radians(m_angleOnTheBow)), 2))) + 1);
        if (degrees(angle) > 20)
            m_spreadAngle = 20;
        else
            m_spreadAngle = degrees(angle);
    }

    std::pair<double, double> calculateCartesianCoordinateOfIdealTorpedoStartPoint(double angle) {
        double x, y;
        if (m_parallaxForward) {
            if (angle >= 0 && angle <= 180) {
                x = 27 + 9.5 + 95 * std::sin(radians(angle)) - (radians(angle) * 95 + 9.5) * std::cos(radians(angle));
                y = 95 * (1 - std::cos(radians(angle))) - (radians(angle) * 95 + 9.5) * std::sin(radians(angle));
            } else {
                x = 27 + 9.5 + 95 * std::sin(radians(360 - angle)) - (radians(360 - angle) * 95 + 9.5) *
                    std::cos(radians(360 - angle));
                y = -(95 * (1 - std::cos(radians(360 - angle))) - (radians(360 - angle) * 95 + 9.5) *
                    std::sin(radians(360 - angle)));
            }
        } else {
            if            (angle >= 0 && angle <= 180) {
                x = 27 + 9.5 + 95 * std::sin(radians(angle)) - (radians(angle) * 95 + 9.5) * std::cos(radians(angle));
                y = -(95 * (1 - std::cos(radians(angle))) - (radians(angle) * 95 + 9.5) * std::sin(radians(angle)));
            } else {
                x = 27 + 9.5 + 95 * std::sin(radians(360 - angle)) - (radians(360 - angle) * 95 + 9.5) *
                    std::cos(radians(360 - angle));
                y = 95 * (1 - std::cos(radians(360 - angle))) - (radians(360 - angle) * 95 + 9.5) *
                    std::sin(radians(360 - angle));
            }
        }
        return std::make_pair(x, y);
    }

public:
    TorpedoVorhaltRechner(double torpedoSpeed, double targetSpeed, double parallaxCorrection,
        double distanceToTarget, double quotient, double angleOnTheBow, double impactAngle,
        double gyroAngle, double targetBearing, double turnRatioCorrection, bool parallaxForward,
        double targetLength, double manualParallax) :
        m_torpedoSpeed(torpedoSpeed), m_targetSpeed(targetSpeed), m_parallaxCorrection(parallaxCorrection),
        m_distanceToTarget(distanceToTarget), m_quotient(quotient), m_angleOnTheBow(angleOnTheBow),
        m_impactAngle(impactAngle), m_gyroAngle(gyroAngle), m_targetBearing(targetBearing),
        m_turnRatioCorrection(turnRatioCorrection), m_parallaxForward(parallaxForward),
        m_targetLength(targetLength), m_enableParallaxMotor(false), m_manualParallax(manualParallax) {
        calculateDeflectionAngle();
        calculateSpread();
    }

    void calculate() {
        delta1 = m_deflectionAngle + m_targetBearing;
        delta2 = delta1 - m_gyroAngle;
        std::pair<double, double> startPoint = calculateCartesianCoordinateOfIdealTorpedoStartPoint(delta2);
        double x = startPoint.first;
        double y = startPoint.second;
        double p = std::sqrt(std::pow(x, 2) + std::pow(y, 2));
        double a = (x == 0) ? 0 : degrees(std::atan(y / x));
        double delta_a = a - m_gyroAngle;
        double delta_v = m_torpedoSpeed - m_targetSpeed;
        double omega = m_targetSpeed * std::sin(radians(m_angleOnTheBow)) / p;
        double vorhalt = (p / (m_torpedoSpeed * std::cos(radians(delta_a)) + omega)) * delta_v;
        double correctedVorhalt = vorhalt + m_parallaxCorrection;
        double finalVorhalt = (m_enableParallaxMotor) ? correctedVorhalt + m_manualParallax : correctedVorhalt;
        double roundedVorhalt = truncateDecimals(finalVorhalt, 1);
        std::cout << "Vorhalt: " << roundedVorhalt << " m" << std::endl;
    }
};

int main() {
    double torpedoSpeed = 40;
    double targetSpeed = 30;
    double parallaxCorrection = 0;
    double distanceToTarget = 500;
    double quotient = 1.2;
    double angleOnTheBow = 15;
    double impactAngle = 30;
    double gyroAngle = 20;
    double targetBearing = 45;
    double turnRatioCorrection = 0;
    bool parallaxForward = true;
    double targetLength = 150;
    double manualParallax = 0;

    TorpedoVorhaltRechner calculator(torpedoSpeed, targetSpeed, parallaxCorrection, distanceToTarget,
        quotient, angleOnTheBow, impactAngle, gyroAngle, targetBearing, turnRatioCorrection,
        parallaxForward, targetLength, manualParallax);
    calculator.calculate();

    return 0;
}

