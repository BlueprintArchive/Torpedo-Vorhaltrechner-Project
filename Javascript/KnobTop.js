function KnobTop(ctx, x, y, stage, scale, type, limit, callback)  {
    var m_knobDragBegin;
    var m_currentAngle = 0;
    var m_scale = scale;

    var image = new Image();
    image.onload = function()  {
        drawKnob();
        installHandler();
    };
    switch(type)  {
        case 0:
            image.src = "assets/img/large_knob.png";
            break;
        case 1:
            image.src = "assets/img/second_knob.png";
            break;
        case 2:
            image.src = "assets/img/third_knob.png";
            break;
        case 3:
            image.src = "assets/img/length_knob.png";
            break;
        default:
            image.src = "assets/img/large_knob.png";
            break;
    }

    this.getCurrentAngle = function()  {
        return m_currentAngle;
    }

    this.setCurrentAngle = function(angle)  {
        m_currentAngle = angle;
    }

    function drawKnob()  {
        ctx.clearRect(x, y, image.width * m_scale, image.height * m_scale);
        ctx.save();
        ctx.setTransform(1, 0, 0, 1, 0, 0);
        ctx.translate(x + image.width / 2 * m_scale, y + image.width / 2 * m_scale);
        ctx.rotate(Math.radians(m_currentAngle));
        ctx.translate(-(x + image.width / 2 * m_scale), -(y + image.width / 2 * m_scale));
        ctx.drawImage(image, x, y, image.width * m_scale, image.height * m_scale);
        ctx.restore();
    }

    function commitPosition()  {
        if(limit > 0 && limit < 20)  {
            var j = 360 / limit;
            var k = j;
            while(k <= 360)  {
                if(m_currentAngle >= (k-j) && m_currentAngle < k)
                    break;
                else
                    k = k + j;
            }
            m_currentAngle = k - j;
            drawKnob();

            audio.play("switch");
        }
    }

    function rotateKnob(angle)  {
        m_currentAngle += angle;
        m_currentAngle = m_currentAngle % 360;

        if(limit > 20)  {
            if(m_currentAngle > limit)
               m_currentAngle = limit;
            if(m_currentAngle < -limit)
               m_currentAngle = -limit;
        }
        else if(limit < 0)  {
            if(m_currentAngle < -limit)
               m_currentAngle = -limit;
            if(m_currentAngle > 360 + limit)
               m_currentAngle = 360 + limit;
        }
    }

    function installHandler()  {
        var container = stage.addChild(new createjs.Container()).set({name:"container"});
        var wheel = container.addChild(new createjs.Shape()).set({name:"red", x:x, y:y});
        wheel.graphics.beginFill("rgba(255, 255, 255, 1)").drawRect(0, 0, image.width * m_scale, image.height * m_scale);

        container.on("mousedown", function(evt)  {
            m_knobDragBegin = evt.stageY;
        });
        container.on("pressmove", function(evt)  {
            var delta = evt.stageY - m_knobDragBegin;
            var angle = delta * 360 / image.height / 100;

            rotateKnob(angle);

            drawKnob();
            if(callback)
                callback(m_currentAngle);
        });
        container.on("pressup", function(evt)  {
            commitPosition();
            if(callback)
                callback(m_currentAngle);
        });

        container.cursor = "pointer";

        window.addEventListener('wheel', function(evt) {
            if(evt.pageX > x && evt.pageX < (x + image.width * m_scale) &&
               evt.pageY > y && evt.pageY < (y + image.height * m_scale))  {
                var delta = event.deltaY;
                if(evt.ctrlKey)
                    delta /= 50;
                if(limit > 20 || limit <= 0)  {
                    rotateKnob(delta);
                    drawKnob();
                }
                else  {
                    var j = 360 / limit;
                    if(delta > 0)
                        m_currentAngle += j;
                    else if(delta < 0)
                        m_currentAngle -= j;
                    commitPosition();
                }
                if(callback)
                    callback(m_currentAngle);
                evt.preventDefault();
            }
        }, false);

    }
}

