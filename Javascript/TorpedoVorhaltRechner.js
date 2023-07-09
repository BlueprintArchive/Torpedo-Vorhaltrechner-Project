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
    this.torpedoSpeed = 0;
    this.targetSpeed = 0;
    this.parallaxCorrection = 0;
    this.distanceToTarget = 300;   // 300 - 10000 m
    this.quotient = 0;
    this.angleOnTheBow = 0;
    this.impactAngle = 0;
    this.deflectionAngle = 0;
    this.gyroAngle = 0;
    this.targetBearing = 0;
    this.turnRatioCorrection = 0;
    this.parallaxForward = true;
    this.targetLength = 100;   // 100 - 200 m
    this.spreadAngle = 0;

    this.delta1 = 0;
    this.delta2 = 0;

    this.enableParallaxMotor = true;
    this.manualParallax = 0;

    this.setImpactAngle = function(angle)  {
        this.impactAngle = angle;
        this.calculateDeflectionAngle();
        this.angleOnTheBow = 180 - (this.impactAngle + this.deflectionAngle + this.parallaxCorrection);
        this.calculateSpread();
    };

    this.setDistanceToTarget = function(distance)  {
        this.distanceToTarget = distance;
        this.calculateSpread();
    };

    this.setTorpedoSpeed = function(speed)  {
        this.torpedoSpeed = speed;
        this.quotient = this.targetSpeed / this.torpedoSpeed;
        this.calculateDeflectionAngle();
        this.calculateSpread();
    };

    this.setTargetSpeed = function(speed)  {
        this.targetSpeed = speed;  
        this.quotient = this.targetSpeed / this.torpedoSpeed;
        this.calculateDeflectionAngle();
        this.calculateSpread();
    };

    this.setTargetBearing = function(angle)  {
        this.targetBearing = angle;
    };

    this.setTurnRatioCorrection = function(correction)  {
        this.turnRatioCorrection = correction;
    };

    this.calculateDeflectionAngle = function()  {
        var ctg_beta = (1 / (this.quotient * Math.sin(Math.radians(this.impactAngle))) - 1 / Math.tan(Math.radians(this.impactAngle)));
        if(isNaN(ctg_beta))
            this.deflectionAngle = 0;
        else
            this.deflectionAngle = Math.degrees(Math.atan(1 / (ctg_beta)));
    };

    this.setParallaxForward = function(newValue)  {
        this.parallaxForward = newValue;
    };

    this.setTargetLength = function(newValue)  {
        this.targetLength = newValue;
        this.calculateSpread();
    };

    this.calculateSpread = function()  {
        var angle = (this.targetLength / this.distanceToTarget) * Math.sin(Math.radians(this.angleOnTheBow)) *
                    ((Math.cos(Math.radians(this.angleOnTheBow)) / Math.sqrt(Math.pow(1/this.quotient, 2) -
                    Math.pow(Math.sin(Math.radians(this.angleOnTheBow)), 2) )) + 1);
        if(Math.degrees(angle) > 20)
            this.spreadAngle = 20;
        else
            this.spreadAngle = Math.degrees(angle);
    };

    var that = this;
    
    setInterval(function() {
        if(that.enableParallaxMotor)  {
            var step = 0.0005;
            that.delta1 = 0;
            while(1)  {
                // turning speed correction is taken into account externally - while setting target bearing
                that.gyroAngle = that.deflectionAngle + that.targetBearing /*- that.turnRatioCorrection * 0.4*/ + that.parallaxCorrection;
                that.gyroAngle = that.gyroAngle % 360;
                if(that.gyroAngle < 0)
                    that.gyroAngle = 360 + that.gyroAngle;

                var result1 = that.calculateCartesianCoordinateOfIdealTorpedoStartPoint(that.gyroAngle);
                var result2 = that.calculatePolarCoordinateOfIdealTorpedoStartPoint(result1[0], result1[1]);
                var X = result2[0];
                var O = result2[1];
                if(that.parallaxForward === true)
                    // turning speed correction is taken into account externally - while setting target bearing
                    that.delta2 = X * Math.sin(Math.radians(that.targetBearing /*- that.turnRatioCorrection * 0.4*/) + that.delta1 - O) /
                                  that.distanceToTarget;
                else
                    // turning speed correction is taken into account externally - while setting target bearing
                    that.delta2 = X * Math.sin(Math.radians(that.targetBearing /*- that.turnRatioCorrection * 0.4*/) - that.delta1 - O) /
                    that.distanceToTarget;

                if(Math.degrees(that.delta1) > 25)
                    that.parallaxCorrection = 25;
                else if(Math.degrees(that.delta1) < -25)
                    that.parallaxCorrection = -25;
                else
                    that.parallaxCorrection = Math.degrees(that.delta1);

                if(that.parallaxForward === false)
                    that.parallaxCorrection = -that.parallaxCorrection;

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
            that.gyroAngle = that.deflectionAngle + that.targetBearing /*- that.turnRatioCorrection * 0.4*/ + that.parallaxCorrection;
            that.gyroAngle = that.gyroAngle % 360;
            if(that.gyroAngle < 0)
                that.gyroAngle = 360 + that.gyroAngle;

            var result1 = that.calculateCartesianCoordinateOfIdealTorpedoStartPoint(that.gyroAngle);
            var result2 = that.calculatePolarCoordinateOfIdealTorpedoStartPoint(result1[0], result1[1]);
            var X = result2[0];
            var O = result2[1];
            if(that.parallaxForward === true)
                // turning speed correction is taken into account externally - while setting target bearing
                that.delta2 = X * Math.sin(Math.radians(that.targetBearing /*- that.turnRatioCorrection * 0.4*/) + that.delta1 - O) /
                              that.distanceToTarget;
            else
                // turning speed correction is taken into account externally - while setting target bearing
                that.delta2 = X * Math.sin(Math.radians(that.targetBearing /*- that.turnRatioCorrection * 0.4*/) - that.delta1 - O) /
                              that.distanceToTarget;

            if(Math.degrees(that.delta1) > 25)
                that.parallaxCorrection = 25;
            else if(Math.degrees(that.delta1) < -25)
                that.parallaxCorrection = -25;
            else
                that.parallaxCorrection = Math.degrees(that.delta1);

            if(that.parallaxForward === false)
                that.parallaxCorrection = -that.parallaxCorrection;

            that.delta1 = Math.radians(that.manualParallax);
        }

        that.angleOnTheBow = 180 - (that.impactAngle + that.deflectionAngle + that.parallaxCorrection);
    }, 100);

    this.calculateCartesianCoordinateOfIdealTorpedoStartPoint = function(angle)  {
        if(that.parallaxForward === true)  {
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
        return this.parallaxCorrection;
    };

    this.getDelta2 = function()  {
        if(this.parallaxForward)
            return Math.degrees(this.delta2);
        else
            return -Math.degrees(this.delta2);
    };

    this.getMaximumDistanceToTarget = function()  {
        var d = Math.sin(Math.radians(this.impactAngle)) / Math.sin(Math.radians(this.impactAngle + this.deflectionAngle));
        if(isNaN(d))
            d = 1;

        return d;
    };

    this.enableParallaxMotor = function(enable)  {
        this.enableParallaxMotor = enable;
    };
};
