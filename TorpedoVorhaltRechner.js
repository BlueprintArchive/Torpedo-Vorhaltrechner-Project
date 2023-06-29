// Converts from degrees to radians.
Math.radians = function(degrees)  {
    return degrees * Math.PI / 180;
};

// Converts from radians to degrees
Math.degrees = function(radians)  {
    return radians * 180 / Math.PI;
};

// truncate decimal digits after point
Math.truncateDecimals = function(num, digits)  {
    var numS = num.toString(),
        decPos = numS.indexOf('.'),
        substrLength = decPos == -1 ? numS.length : 1 + decPos + digits,
        trimmedResult = numS.substr(0, substrLength),
        finalResult = isNaN(trimmedResult) ? 0 : trimmedResult;

    return parseFloat(finalResult);
};

function TorpedoVorhaltRechner()  {
    this.m_torpedoSpeed = 0;
    this.m_targetSpeed = 0;
    this.m_parallaxCorrection = 0;
    this.m_distanceToTarget = 300;   // 300 - 10000 m
    this.m_quotient = 0;
    this.m_angleOnTheBow = 0;
    this.m_impactAngle = 0;
    this.m_deflectionAngle = 0;
    this.m_gyroAngle = 0;
    this.m_targetBearing = 0;
    this.m_turnRatioCorrection = 0;
    this.m_parallaxForward = true;
    this.m_targetLength = 100;   // 100 - 200 m
    this.m_spreadAngle = 0;

    this.delta1 = 0;
    this.delta2 = 0;

    this.m_enableParallaxMotor = true;
    this.m_manualParallax = 0;

    this.setImpactAngle = function(angle)  {
        this.m_impactAngle = angle;
        this.calculateDeflectionAngle();
        this.m_angleOnTheBow = 180 - (this.m_impactAngle + this.m_deflectionAngle + this.m_parallaxCorrection);
        this.calculateSpread();
    };

    this.setDistanceToTarget = function(distance)  {
        this.m_distanceToTarget = distance;
        this.calculateSpread();
    };

    this.setTorpedoSpeed = function(speed)  {
        this.m_torpedoSpeed = speed;
        this.m_quotient = this.m_targetSpeed / this.m_torpedoSpeed;
        this.calculateDeflectionAngle();
        this.calculateSpread();
    };

    this.setTargetSpeed = function(speed)  {
        this.m_targetSpeed = speed;  
        this.m_quotient = this.m_targetSpeed / this.m_torpedoSpeed;
        this.calculateDeflectionAngle();
        this.calculateSpread();
    };

    this.setTargetBearing = function(angle)  {
        this.m_targetBearing = angle;
    };

    this.setTurnRatioCorrection = function(correction)  {
        this.m_turnRatioCorrection = correction;
    };

    this.calculateDeflectionAngle = function()  {
        var ctg_beta = (1 / (this.m_quotient * Math.sin(Math.radians(this.m_impactAngle))) - 1 / Math.tan(Math.radians(this.m_impactAngle)));
        if(isNaN(ctg_beta))
            this.m_deflectionAngle = 0;
        else
            this.m_deflectionAngle = Math.degrees(Math.atan(1 / (ctg_beta)));
    };

    this.setParallaxForward = function(newValue)  {
        this.m_parallaxForward = newValue;
    };

    this.setTargetLength = function(newValue)  {
        this.m_targetLength = newValue;
        this.calculateSpread();
    };

    this.calculateSpread = function()  {
        var angle = (this.m_targetLength / this.m_distanceToTarget) * Math.sin(Math.radians(this.m_angleOnTheBow)) *
                    ((Math.cos(Math.radians(this.m_angleOnTheBow)) / Math.sqrt(Math.pow(1/this.m_quotient, 2) -
                    Math.pow(Math.sin(Math.radians(this.m_angleOnTheBow)), 2) )) + 1);
        if(Math.degrees(angle) > 20)
            this.m_spreadAngle = 20;
        else
            this.m_spreadAngle = Math.degrees(angle);
    };

    var that = this;
    setInterval(function() {
        if(that.m_enableParallaxMotor)  {
            var step = 0.0005;
            that.delta1 = 0;
            while(1)  {
                // turning speed correction is taken into account externally - while setting target bearing
                that.m_gyroAngle = that.m_deflectionAngle + that.m_targetBearing /*- that.m_turnRatioCorrection * 0.4*/ + that.m_parallaxCorrection;
                that.m_gyroAngle = that.m_gyroAngle % 360;
                if(that.m_gyroAngle < 0)
                    that.m_gyroAngle = 360 + that.m_gyroAngle;

                var result1 = that.calculateCartesianCoordinateOfIdealTorpedoStartPoint(that.m_gyroAngle);
                var result2 = that.calculatePolarCoordinateOfIdealTorpedoStartPoint(result1[0], result1[1]);
                var X = result2[0];
                var O = result2[1];
                if(that.m_parallaxForward === true)
                    // turning speed correction is taken into account externally - while setting target bearing
                    that.delta2 = X * Math.sin(Math.radians(that.m_targetBearing /*- that.m_turnRatioCorrection * 0.4*/) + that.delta1 - O) /
                                  that.m_distanceToTarget;
                else
                    // turning speed correction is taken into account externally - while setting target bearing
                    that.delta2 = X * Math.sin(Math.radians(that.m_targetBearing /*- that.m_turnRatioCorrection * 0.4*/) - that.delta1 - O) /
                    that.m_distanceToTarget;

                if(Math.degrees(that.delta1) > 25)
                    that.m_parallaxCorrection = 25;
                else if(Math.degrees(that.delta1) < -25)
                    that.m_parallaxCorrection = -25;
                else
                    that.m_parallaxCorrection = Math.degrees(that.delta1);

                if(that.m_parallaxForward === false)
                    that.m_parallaxCorrection = -that.m_parallaxCorrection;

                if(Math.abs(that.delta1 - that.delta2) < 0.001)
                    break;

                if(that.delta1 >= Math.PI)  {
                    that.delta1 = 0;
                    step = -step;
                    continue;
                }
                if(that.delta1 <= -Math.PI)
                    break;

                that.delta1 += step;
            }
        }
        else  {
            // turning speed correction is taken into account externally - while setting target bearing
            that.m_gyroAngle = that.m_deflectionAngle + that.m_targetBearing /*- that.m_turnRatioCorrection * 0.4*/ + that.m_parallaxCorrection;
            that.m_gyroAngle = that.m_gyroAngle % 360;
            if(that.m_gyroAngle < 0)
                that.m_gyroAngle = 360 + that.m_gyroAngle;

            var result1 = that.calculateCartesianCoordinateOfIdealTorpedoStartPoint(that.m_gyroAngle);
            var result2 = that.calculatePolarCoordinateOfIdealTorpedoStartPoint(result1[0], result1[1]);
            var X = result2[0];
            var O = result2[1];
            if(that.m_parallaxForward === true)
                // turning speed correction is taken into account externally - while setting target bearing
                that.delta2 = X * Math.sin(Math.radians(that.m_targetBearing /*- that.m_turnRatioCorrection * 0.4*/) + that.delta1 - O) /
                              that.m_distanceToTarget;
            else
                // turning speed correction is taken into account externally - while setting target bearing
                that.delta2 = X * Math.sin(Math.radians(that.m_targetBearing /*- that.m_turnRatioCorrection * 0.4*/) - that.delta1 - O) /
                              that.m_distanceToTarget;

            if(Math.degrees(that.delta1) > 25)
                that.m_parallaxCorrection = 25;
            else if(Math.degrees(that.delta1) < -25)
                that.m_parallaxCorrection = -25;
            else
                that.m_parallaxCorrection = Math.degrees(that.delta1);

            if(that.m_parallaxForward === false)
                that.m_parallaxCorrection = -that.m_parallaxCorrection;

            that.delta1 = Math.radians(that.m_manualParallax);
        }

        that.m_angleOnTheBow = 180 - (that.m_impactAngle + that.m_deflectionAngle + that.m_parallaxCorrection);
    }, 100);

    this.calculateCartesianCoordinateOfIdealTorpedoStartPoint = function(angle)  {
        if(that.m_parallaxForward === true)  {
            if(angle >= 0 && angle <= 180)  {
                var x = 27 + 9.5 + 95 * Math.sin(Math.radians(angle)) - (Math.radians(angle) * 95 + 9.5) * Math.cos(Math.radians(angle));
                var y = 95 * (1 - Math.cos(Math.radians(angle))) - (Math.radians(angle) * 95 + 9.5) * Math.sin(Math.radians(angle));
                return [x, y];
            }
            else  {
                var x = 27 + 9.5 + 95 * Math.sin(Math.radians(360 - angle)) - (Math.radians(360 - angle) * 95 + 9.5) *
                        Math.cos(Math.radians(360 - angle));
                var y = -(95 * (1 - Math.cos(Math.radians(360 - angle))) - (Math.radians(360 - angle) * 95 + 9.5) *
                        Math.sin(Math.radians(360 - angle)));
                return [x, y];
            }
        }
        else  {
            if(angle >= 0 && angle <= 180)  {
                var x = -(27 + 9.5 + 95 * Math.sin(Math.radians(180 - angle)) - (Math.radians(180 - angle) * 95 + 9.5) *
                        Math.cos(Math.radians(180 - angle)));
                var y = 95 * (1 - Math.cos(Math.radians(180 - angle))) - (Math.radians(180 - angle) * 95 + 9.5) *
                        Math.sin(Math.radians(180 - angle));
                return [x, y];
            }
            else  {
                var x = -(27 + 9.5 + 95 * Math.sin(Math.radians(angle - 180)) - (Math.radians(angle - 180) * 95 + 9.5) *
                        Math.cos(Math.radians(angle - 180)));
                var y = -(95 * (1 - Math.cos(Math.radians(angle - 180))) - (Math.radians(angle -180) * 95 + 9.5) *
                        Math.sin(Math.radians(angle - 180)));
                return [x, y];
            }
        }
    };

    this.calculatePolarCoordinateOfIdealTorpedoStartPoint = function(x, y)  {
        var X = Math.sqrt(Math.pow(x, 2) + Math.pow(y, 2));
        var O = Math.atan(y / x);
        return [X, O];
    };

    this.getDelta1 = function()  {
        return this.m_parallaxCorrection;
    };

    this.getDelta2 = function()  {
        if(this.m_parallaxForward)
            return Math.degrees(this.delta2);
        else
            return -Math.degrees(this.delta2);
    };

    this.getMaximumDistanceToTarget = function()  {
        var d = Math.sin(Math.radians(this.m_impactAngle)) / Math.sin(Math.radians(this.m_impactAngle + this.m_deflectionAngle));
        if(isNaN(d))
            d = 1;

        return d;
    };

    this.enableParallaxMotor = function(enable)  {
        this.m_enableParallaxMotor = enable;
    };
};
