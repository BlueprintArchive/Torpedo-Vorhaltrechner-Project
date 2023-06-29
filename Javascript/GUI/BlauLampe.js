function BlauLampe(ctx, TVReOffsetX, TVReOffsetY, stage)  {
    var LampOffsetX = 828;
    var LampOffsetY = 304;
    var LampWidth = 20;
    var LampHeight = 20;

    var Lamp1Img = new Image();
    Lamp1Img.onload = function()  {
        ctx.drawImage(Lamp1Img, TVReOffsetX + LampOffsetX, TVReOffsetY + LampOffsetY, LampWidth, LampHeight);
    };
    Lamp1Img.src = "assets/img/blauLampOff.png";
    var Lamp2Img = new Image();
    Lamp2Img.src = "assets/img/blauLampOn.png";

    this.setState = function(state)  {
        if(state == 0)
            ctx.drawImage(Lamp1Img, TVReOffsetX + LampOffsetX, TVReOffsetY + LampOffsetY, LampWidth, LampHeight);
        else
            ctx.drawImage(Lamp2Img, TVReOffsetX + LampOffsetX, TVReOffsetY + LampOffsetY, LampWidth, LampHeight);
    }
}

