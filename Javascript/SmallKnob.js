function SmallKnob(ctx, x, y, stage, scale)  {
    var m_qualityFactor = 2;
    var m_knobDragBegin;
    var m_currentPosition = 0;
    var m_scale = scale;
    var m_currentSetting;

    var image = new Image();
    image.onload = function()  {
        ctx.drawImage(image, x, y, image.width * m_scale, image.height * m_scale);
        installHandler();
    };
    image.src = "assets/img/SmallKnob/0001.png";


    var imageArray = [];
    var filename;
    for(var i = 0; i < 180; ++i)  {
        if(i % m_qualityFactor == 0 || i == 179)  {
            var j = i + 1;
            if(j < 10)
                filename = "000" + j;
            else if(j < 100)
                filename = "00" + j;
            else
                filename = "0" + j;
            filename += ".png";
            var img = new Image();
            img.src = "assets/img/SmallKnob/" + filename;
            imageArray[i] = img;
        }
    }

    function installHandler()  {
        var container = stage.addChild(new createjs.Container()).set({name:"container"});
        var wheel = container.addChild(new createjs.Shape()).set({name:"red", x:x, y:y});
        wheel.graphics.beginFill("rgba(255, 255, 255, 1)").drawRect(0, 0, image.width * m_scale, image.height * m_scale);

        container.on("mousedown", function(evt)  {
            m_knobDragBegin = evt.stageX;

            if(m_currentPosition == 0)
                m_currentSetting = 0;
            else if(m_currentPosition == 90)
                m_currentSetting = 1;
            else
                m_currentSetting = 2;
        });
        container.on("pressmove", function(evt)  {
            var delta = evt.stageX - m_knobDragBegin;
            var angle = delta * 360 / image.width / 10;

            m_currentPosition += angle;

            m_currentPosition = Math.round(m_currentPosition);

            if(m_currentPosition >= 180)
                m_currentPosition = 179;

            if(m_currentPosition < 0)
                m_currentPosition = 0;

            if(m_currentPosition % m_qualityFactor == 0)  {
                ctx.clearRect(x, y, image.width * m_scale, image.height * m_scale);
                ctx.drawImage(imageArray[m_currentPosition], x, y, image.width * m_scale, image.height * m_scale);
            }
        });
        container.on("pressup", function(evt)  {
            if(m_currentPosition < 45)
                m_currentPosition = 0;
            else if(m_currentPosition >= 45 && m_currentPosition < 135)
                m_currentPosition = 90;
            else
                m_currentPosition = 179;

            ctx.clearRect(x, y, image.width * m_scale, image.height * m_scale);
            ctx.drawImage(imageArray[m_currentPosition], x, y, image.width * m_scale, image.height * m_scale);

            if((m_currentPosition == 0 && m_currentSetting == 0) ||
               (m_currentPosition == 90 && m_currentSetting == 1) ||
               (m_currentPosition == 179 && m_currentSetting == 2))
                ;
            else
                audio.play("switch");
        });

        container.cursor = "pointer";

        window.addEventListener('wheel', function(evt) {
            if(evt.pageX > x && evt.pageX < (x + image.width * m_scale) &&
               evt.pageY > y && evt.pageY < (y + image.height * m_scale))  {
                var delta = event.deltaY;
                if(delta > 0)  {
                    if(m_currentPosition == 0)  {
                        m_currentPosition = 90;
                        audio.play("switch");
                    }
                    else if(m_currentPosition == 90)  {
                        m_currentPosition = 179;
                        audio.play("switch");
                    }
                }
                else  {
                    if(m_currentPosition == 179)  {
                        m_currentPosition = 90;
                        audio.play("switch");
                    }
                    else if(m_currentPosition == 90)  {
                        m_currentPosition = 0;
                        audio.play("switch");
                    }
                }
                ctx.clearRect(x, y, image.width * m_scale, image.height * m_scale);
                ctx.drawImage(imageArray[m_currentPosition], x, y, image.width * m_scale, image.height * m_scale);

                evt.preventDefault();
            }
        }, false);
    }

    this.getSpeedSetting = function()  {
        if(m_currentPosition < 45)
            return 30;
        else if(m_currentPosition >= 45 && m_currentPosition < 135)
            return 40;
        else
            return 44;
    }
}

