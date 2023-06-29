function TargetSpeedDial(TVReOffsetX, TVReOffsetY, dialCanvasContext, pointerCanvasContext)  {
        var dialOffsetX = 266;
        var dialOffsetY = 114;

        var dialWidth = 115;
        var dialHeight = 115;

        var pointerWidth = 80;
        var pointerHeight = 80;

        var pointerOffsetX = dialOffsetX + (dialWidth - pointerWidth) / 2;
        var pointerOffsetY = dialOffsetY + (dialHeight - pointerHeight) / 2;

        var DialImg = new Image();
        DialImg.onload = function()  {
            dialCanvasContext.drawImage(DialImg, TVReOffsetX + dialOffsetX, TVReOffsetY + dialOffsetY);
        };
        DialImg.src = "assets/img/targetSpeedDial.png";

        var PointerImg = new Image();
        PointerImg.onload = function()  {
            pointerCanvasContext.drawImage(PointerImg, TVReOffsetX + pointerOffsetX, TVReOffsetY + pointerOffsetY);
        };
        PointerImg.src = "assets/img/pointer-1.png";

        this.setSpeed = function(speed)  {
            // speed given in knots, in range (0;50)
            var convertedAngle;
            if(speed > 50)
                convertedAngle = speed % 50;
            else if(speed < 0)
                convertedAngle = 50 - speed % 50;
            else
                convertedAngle = speed;
            convertedAngle = convertedAngle * 7.2;
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
