function TVReGUI(TVReOffsetX, TVReOffsetY)  {
        var backgroundCanvas = document.getElementById("backgroundCanvas");
        var backgroundCanvasContext = backgroundCanvas.getContext("2d");

        var TVRePanelImg = new Image();
        TVRePanelImg.onload = function()  {
            backgroundCanvasContext.drawImage(TVRePanelImg, TVReOffsetX, TVReOffsetY);
        };
        TVRePanelImg.src = "assets/img/TVRe.png";

        var parallaxSwitchPosition = true;   // true - 0, false - 180

        var distanceToTarget = 1200;
        var targetLength = 141.25; // for angle setting 95 deg

        var imageCanvas = document.getElementById("imageCanvas");
        var imageCanvasContext = imageCanvas.getContext("2d");

        var dialCanvas = document.getElementById("dialCanvas");
        var dialCanvasContext = dialCanvas.getContext("2d");

        var pointerCanvas = document.getElementById("pointerCanvas");
        var pointerCanvasContext = pointerCanvas.getContext("2d");

        var stage = new createjs.Stage("imageCanvas");
        stage.name = "stage";
        stage.enableMouseOver(20);

        var targetSpeedKnob = new LargeKnob(imageCanvasContext, TVReOffsetX + 250, TVReOffsetY + 5, stage, 0.3, 1);
        var parallaxCorrectionKnob = new LargeKnob(imageCanvasContext, TVReOffsetX + 500, TVReOffsetY + 5, stage, 0.3, 1);

        var torpedoSpeedKnob = new SmallKnob(imageCanvasContext, TVReOffsetX + 387, TVReOffsetY - 25, stage, 0.2);

        var angleOnTheBowKnob = new LargeKnob(imageCanvasContext, TVReOffsetX + 220, TVReOffsetY + 627, stage, 0.3, -1);
        var spreadAngleKnob = new LargeKnob(imageCanvasContext, TVReOffsetX + 500, TVReOffsetY + 627, stage, 0.3, -1);

        var targetBearingKnob = new KnobTop(imageCanvasContext, TVReOffsetX + 740, TVReOffsetY + 70, stage, 0.4, 0, 0);
        var parallaxSwitchKnob = new KnobTop(imageCanvasContext, TVReOffsetX + 535, TVReOffsetY + 127, stage, 0.25, 1, 2,
                                         function(angle)  {
                                             if(parallaxSwitchKnob.getCurrentAngle() == 0)
                                                 parallaxSwitchPosition = true;
                                             else if(parallaxSwitchKnob.getCurrentAngle() == 180)
                                                 parallaxSwitchPosition = false;
                                         });

        var turningSpeedCorrectionKnob = new KnobTop(imageCanvasContext, TVReOffsetX + 538, TVReOffsetY + 413, stage, 0.25, 1, 54,
                                         function(angle)  {
                                         });

        var targetLengthKnob = new KnobTop(imageCanvasContext, TVReOffsetX + 357, TVReOffsetY + 353, stage, 0.25, 3, 95,
                                         function(angle)  {
                                             var a = 95 - angle;
                                             var l = (a - 0)*(1.15 - 1)/(190 - 0) + 1;
                                             l = l * 2;
                                             l = Math.pow(10, l);
                                             targetLength = l;
                                         });
        var distanceToTargetKnob = new KnobTop(imageCanvasContext, TVReOffsetX + 271, TVReOffsetY + 264, stage, 0.3, 1, -13,
                                         function(angle)  {
                                             distanceToTarget = DistanceToTarget(angle);
                                         });
        var initialAngle = (1200 - 1000)*(149.59 - 111.1)/(1500 - 1000) + 111.1 + 12.43;

        distanceToTargetKnob.setCurrentAngle(initialAngle);

        var TorpedoRunKnob = new KnobTop(imageCanvasContext, TVReOffsetX + 148, TVReOffsetY + 228, stage, 0.2, 2, 5,
                                         function(angle)  {
                                             distanceDial.rotate(angle);
                                         });

        var deflectionAngleDial = new DeflectionAngleDial(TVReOffsetX, TVReOffsetY, dialCanvasContext, pointerCanvasContext);
        var targetSpeedDial = new TargetSpeedDial(TVReOffsetX, TVReOffsetY, dialCanvasContext, pointerCanvasContext);
        var parallaxAngleDial = new ParallaxAngleDial(TVReOffsetX, TVReOffsetY, dialCanvasContext, pointerCanvasContext);
        var spreadAngleDial = new SpreadAngleDial(TVReOffsetX, TVReOffsetY, dialCanvasContext, pointerCanvasContext);
        var ownCourseChangeDial = new OwnCourseChangeDial(TVReOffsetX, TVReOffsetY, dialCanvasContext, pointerCanvasContext);
        var angleOnTheBowDial = new AngleOnTheBowDial(TVReOffsetX, TVReOffsetY, dialCanvasContext, pointerCanvasContext);
        var targetBearingDial = new TargetBearingDial(TVReOffsetX, TVReOffsetY, dialCanvasContext, pointerCanvasContext);
        var gyroAngleDial = new GyroAngleDial(TVReOffsetX, TVReOffsetY, dialCanvasContext, pointerCanvasContext);

        var drum = new Drum(TVReOffsetX + 130, TVReOffsetY + 285, dialCanvasContext, 1, false);
        var drumZoom = new Drum(650, 650, dialCanvasContext, 3, true);

        var lageSchalter = new LageSchalter(imageCanvasContext, TVReOffsetX, TVReOffsetY, stage);

        var blauSchalter = new BlauSchalter(imageCanvasContext, TVReOffsetX, TVReOffsetY, stage);

        var blauLampe = new BlauLampe(imageCanvasContext, TVReOffsetX, TVReOffsetY, stage);

        var deckungLampe = new DeckungLampe(imageCanvasContext, TVReOffsetX, TVReOffsetY, stage);

        var distanceDial = new DistanceDial(TVReOffsetX, TVReOffsetY, pointerCanvasContext);

        var spreadAngle = 0;
        var parallaxAngle = 0;

        this.setPeriscopeBearing = function(angle)  {
              targetBearingDial.setAngle2(angle);
        }

        this.setTargetBearing = function(angle)  {
              targetBearingDial.setAngle1(angle);
        }

        this.getTurningSpeedCorrection = function()  {
              var turningSpeedCorrection = turningSpeedCorrectionKnob.getCurrentAngle() / 18;
              return Math.truncateDecimals(turningSpeedCorrection, 2);
        }

        this.getTargetBearing = function()  {
              var targetBearing = Math.truncateDecimals(targetBearingKnob.getCurrentAngle(), 2);
              if(targetBearing < 0)
                  targetBearing = 360 + targetBearing;

              return targetBearing;
        }

        this.getTorpedoSpeed = function()  {
              return torpedoSpeedKnob.getSpeedSetting();
        }

        this.getTargetSpeed = function()  {
              return targetSpeedKnob.getCurrentSetting() * this.getTorpedoSpeed() / 180;
        }

        this.setTargetSpeed = function(speed)  {
              targetSpeedDial.setSpeed(speed);
        }

        this.getAngleOnTheBow = function()  {
              var angleOnTheBow = angleOnTheBowKnob.getCurrentSetting();
              return angleOnTheBow;
        }

        this.setAngleOnTheBow = function(angle)  {
              angleOnTheBowDial.setAngle(angle);
        }

        this.getSpreadAngle = function()  {
              return (spreadAngle + spreadAngleKnob.getCurrentSetting()) / 18;
        }

        this.resetSpreadAngleKnob = function()  {
              spreadAngle = -spreadAngleKnob.getCurrentSetting();
        }

        this.getParallaxAngle = function()  {
              var angle = parallaxAngle + parallaxCorrectionKnob.getCurrentSetting();
              angle = angle % 360;
              if(angle >= 0 && angle < 180)
                  angle /= 7.2;
              else  {
                  angle = 360 - angle;
                  angle /= -7.2;
              }
              return angle;
        }

        this.resetParallaxAngleKnob = function(angle)  {
              var t;
              if(angle > 0)
                  t = angle * 7.2;
              else  {
                  t = angle * (-7.2);
                  t = 360 - t;
              }

              parallaxAngle = t - parallaxCorrectionKnob.getCurrentSetting();
        }

        this.setDeflectionAngle = function(angle)  {
              deflectionAngleDial.setAngle(angle);
        }

        this.setOwnCourseChange = function(angle1, angle2)  {
              ownCourseChangeDial.setAngle1(angle1);
              ownCourseChangeDial.setAngle2(angle2);
        }

        this.getParallax = function()  {
              return parallaxSwitchPosition;
        }

        this.setParallaxAngle1 = function(angle)  {
              parallaxAngleDial.setAngle1(angle);
        }

        this.setParallaxAngle2 = function(angle)  {
              parallaxAngleDial.setAngle2(angle);
        }

        this.setGyroAngle = function(angle)  {
              gyroAngleDial.setGyroAngle(angle);
        }

        this.getDistanceToTarget = function()  {
              return distanceToTarget;
        }

        this.getTargetLength = function()  {
              return targetLength;
        }

        this.setSpreadAngle = function(angle1, angle2)  {
              spreadAngleDial.setAngle1(angle1);
              spreadAngleDial.setAngle2(angle2);
              gyroAngleDial.setSpreadAngle(angle2);
        }

        this.setMaximumDistance = function(e)  {
              drum.set(e);
              drumZoom.set(e);
        }

        this.getBlauSchalterState = function()  {
              return blauSchalter.getState();
        }

        this.setBlauLampe = function(state)  {
              blauLampe.setState(state);
        }

        this.getLageSchalterState = function()  {
              return lageSchalter.getState();
        }
}
