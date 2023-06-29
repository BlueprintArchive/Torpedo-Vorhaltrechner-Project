function DistanceDial(TVReOffsetX, TVReOffsetY, dialCanvasContext)  {
        var dialOffsetX = 139;
        var dialOffsetY = 218;

        var dialWidth = 75;
        var dialHeight = 75;

        var DialImg = new Image();
        DialImg.onload = function()  {
            dialCanvasContext.drawImage(DialImg, TVReOffsetX + dialOffsetX, TVReOffsetY + dialOffsetY);
            dialCanvasContext.clearRect(TVReOffsetX + dialOffsetX, TVReOffsetY + dialOffsetY, DialImg.width, DialImg.height/2);
        };
        DialImg.src = "assets/img/distanceDial.png";

        this.rotate = function(angle)  {
            dialCanvasContext.clearRect(TVReOffsetX + dialOffsetX, TVReOffsetY + dialOffsetY, DialImg.width, DialImg.height);
            dialCanvasContext.save();
            dialCanvasContext.setTransform(1, 0, 0, 1, 0, 0);

            dialCanvasContext.translate(TVReOffsetX + dialOffsetX + dialWidth/2, TVReOffsetY + dialOffsetY + dialHeight/2);
            dialCanvasContext.rotate(Math.radians(angle));
            dialCanvasContext.translate(-(TVReOffsetX + dialOffsetX + dialWidth/2), -(TVReOffsetY + dialOffsetY + dialHeight/2));
            dialCanvasContext.drawImage(DialImg, TVReOffsetX + dialOffsetX, TVReOffsetY + dialOffsetY);
            dialCanvasContext.restore();
            dialCanvasContext.clearRect(TVReOffsetX + dialOffsetX, TVReOffsetY + dialOffsetY, DialImg.width, DialImg.height/2);
        }
}
