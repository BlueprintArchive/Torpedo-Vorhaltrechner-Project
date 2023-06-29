function LageSchalter(ctx, TVReOffsetX, TVReOffsetY, stage)  {
    var SchalterOffsetX = 373;
    var SchalterOffsetY = 597;
    var SchalterWidth = 60;
    var SchalterHeight = 30;

    var state = 0;

    var Schalter1Img = new Image();
    Schalter1Img.onload = function()  {
        ctx.drawImage(Schalter1Img, TVReOffsetX + SchalterOffsetX, TVReOffsetY + SchalterOffsetY, SchalterWidth, SchalterHeight);
        installHandler();
    };
    Schalter1Img.src = "assets/img/lageSchalter1.png";
    var Schalter2Img = new Image();
    Schalter2Img.src = "assets/img/lageSchalter2.png";

    function installHandler()  {
        var container = stage.addChild(new createjs.Container()).set({name:"container"});
        var wheel = container.addChild(new createjs.Shape()).set({name:"red", x:TVReOffsetX + SchalterOffsetX, y:TVReOffsetY + SchalterOffsetY});
        wheel.graphics.beginFill("rgba(255, 255, 255, 1)").drawRect(0, 0, Schalter1Img.width, Schalter1Img.height);

        container.on("mousedown", function(evt)  {
            ctx.clearRect(TVReOffsetX + SchalterOffsetX, TVReOffsetY + SchalterOffsetY, SchalterWidth, SchalterHeight);
            if(state == 0)  {
                ctx.drawImage(Schalter2Img, TVReOffsetX + SchalterOffsetX, TVReOffsetY + SchalterOffsetY, SchalterWidth, SchalterHeight);
                state = 1;
            }
            else  {
                ctx.drawImage(Schalter1Img, TVReOffsetX + SchalterOffsetX, TVReOffsetY + SchalterOffsetY, SchalterWidth, SchalterHeight);
                state = 0;
            }
            audio.play("switch");
        });

        container.on("pressup", function(evt)  {
        });

        container.cursor = "pointer";
    }

    this.getState = function()  {
        return state;
    }
}

