function DeflectionAngleDial(TVReOffsetX, TVReOffsetY, dialCanvasContext, pointerCanvasContext)  {
        var dialOffsetX = 144;
        var dialOffsetY = 125;

        var dialWidth = 100;
        var dialHeight = 100;

        var pointerWidth = 23;
        var pointerHeight = 82;

        var pointerOffsetX = dialOffsetX + (dialWidth - pointerWidth) / 2;
        var pointerOffsetY = dialOffsetY + (dialHeight - pointerHeight) / 2;

        var DialImg = new Image();
        DialImg.onload = function()  {
            dialCanvasContext.drawImage(DialImg, TVReOffsetX + dialOffsetX, TVReOffsetY + dialOffsetY);
        };
        DialImg.src = "assets/img/deflectionAngleDial.png";

        var PointerImg = new Image();
        PointerImg.onload = function()  {
            pointerCanvasContext.drawImage(PointerImg, TVReOffsetX + pointerOffsetX, TVReOffsetY + pointerOffsetY);
        };
        PointerImg.src = "assets/img/pointer-2.png";

        this.setAngle = function(angle)  {
            // angle given in degrees, in range (-75;75)
            var convertedAngle;
            if(angle > 75 || angle < -75)
                convertedAngle = angle % 75;
            else
                convertedAngle = angle;
            convertedAngle = convertedAngle * 2.4;
            convertedAngle = Math.radians(convertedAngle);
            pointerCanvasContext.clearRect(TVReOffsetX + dialOffsetX, TVReOffsetY + dialOffsetY, DialImg.width, DialImg.height);
            pointerCanvasContext.save();
            pointerCanvasContext.setTransform(1, 0, 0, 1, 0, 0);

            pointerCanvasContext.translate(TVReOffsetX + pointerOffsetX + pointerWidth/2, TVReOffsetY + pointerOffsetY + pointerHeight/2);
            pointerCanvasContext.rotate(convertedAngle);
            pointerCanvasContext.translate(-(TVReOffsetX + pointerOffsetX + pointerWidth/2), -(TVReOffsetY + pointerOffsetY + pointerHeight/2));
            pointerCanvasContext.drawImage(PointerImg, TVReOffsetX + pointerOffsetX, TVReOffsetY + pointerOffsetY);
            pointerCanvasContext.restore();
        }
}
