function LargeKnob(ctx, x, y, stage, scale, invert)  {
    var m_knobDragBegin;
    var m_currentPosition = 0;
    var m_scale = scale;

    var image = new Image();
    image.onload = function()  {
        ctx.save();
        ctx.scale(1, 1 * invert);
        ctx.drawImage(image, x, y * invert, image.width * m_scale, image.height * m_scale);
        ctx.restore();
        installHandler();
    };
    image.src = "assets/img/LargeKnob/0001.png";


    var imageArray = [];
    var filename;
    for(var i = 0; i < 180; ++i)  {
        if(i % 2 == 0)  {
            var j = i + 1;
            if(j < 10)
                filename = "000" + j;
            else if(j < 100)
                filename = "00" + j;
            else
                filename = "0" + j;
            filename += ".png";
            var img = new Image();
            img.src = "assets/img/LargeKnob/" + filename;
            imageArray[i] = img;
        }
    }

    function rotateKnob(angle)  {
        m_currentPosition += angle;
        m_currentPosition = m_currentPosition % 360;

        m_currentPosition = Math.round(m_currentPosition);

        if(m_currentPosition < 0)
            m_currentPosition = 360 + m_currentPosition;

        if(m_currentPosition == 360)
            m_currentPosition = 0;

        ctx.save();
        ctx.scale(1, 1 * invert);
        ctx.clearRect(x, y * invert, image.width * m_scale, image.height * m_scale);
        ctx.restore();

        ctx.save();
        if(m_currentPosition < 90)  {
            ctx.scale(1, 1 * invert);
            ctx.drawImage(imageArray[(m_currentPosition * 2)], x, y * invert,
                              image.width * m_scale, image.height * m_scale);
        }
        else if(m_currentPosition < 180)  {
            ctx.scale(-1, 1 * invert);
            ctx.drawImage(imageArray[(179 - m_currentPosition) * 2], -(x + image.width * m_scale),  y * invert,
                          image.width * m_scale, image.height * m_scale);
        }
        else if(m_currentPosition < 270)  {
            ctx.scale(-1, 1 * invert);
            ctx.drawImage(imageArray[(178 - (269 - m_currentPosition) * 2)], -(x + image.width * m_scale),  y * invert,
                          image.width * m_scale, image.height * m_scale);
        }
        else  {
            ctx.scale(1, 1 * invert);
            ctx.drawImage(imageArray[(359 - m_currentPosition) * 2], x,  y * invert,
                          image.width * m_scale, image.height * m_scale);
        }
        ctx.restore();
    }

    function installHandler()  {
        var container = stage.addChild(new createjs.Container()).set({name:"container"});
        var wheel;
        if(invert == -1)
            wheel = container.addChild(new createjs.Shape()).set({name:"red", x:x, y:y - image.height * m_scale});
        else
            wheel = container.addChild(new createjs.Shape()).set({name:"red", x:x, y:y});
        wheel.graphics.beginFill("rgba(255, 255, 255, 1)").drawRect(0, 0, image.width * m_scale, image.height * m_scale);

        container.on("mousedown", function(evt)  {
            m_knobDragBegin = evt.stageX;
        });
        container.on("pressmove", function(evt)  {
            var delta = evt.stageX - m_knobDragBegin;
            var angle = delta * 360 / image.width;

            m_knobDragBegin += delta;

            rotateKnob(angle);
        });
        container.on("pressup", function(evt)  {
        });

        container.cursor = "pointer";

        window.addEventListener('wheel', function(evt) {
            var capture = false;
            if(invert == 1)  {
                if(evt.pageX > x && evt.pageX < (x + image.width * m_scale) &&
                   evt.pageY > y && evt.pageY < (y + image.height * m_scale))  {
                    capture = true;
                }
            }
            else if(invert == -1)  {
                if(evt.pageX > x && evt.pageX < (x + image.width * m_scale) &&
                   evt.pageY > y - image.height * m_scale && evt.pageY < y)  {
                    capture = true;
                }
            }
            if(capture)  {
                var delta = event.deltaY;
                if(evt.ctrlKey)
                    delta /= 20;
                rotateKnob(delta);
                evt.preventDefault();
            }
        }, false);
    }

    this.getCurrentSetting = function()  {
        return m_currentPosition;
    }
}

