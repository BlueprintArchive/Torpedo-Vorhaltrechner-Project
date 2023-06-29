function AngleOnTheBowDial(TVReOffsetX, TVReOffsetY, dialCanvasContext, pointerCanvasContext)  {
        var dialOffsetX = 264;
        var dialOffsetY = 402;

        var dialWidth = 120;
        var dialHeight = 119;

        var pointer1Width = 23;
        var pointer1Height = 82;

        var pointer1OffsetX = dialOffsetX + (dialWidth - pointer1Width) / 2;
        var pointer1OffsetY = dialOffsetY + (dialHeight - pointer1Height) / 2;

        var pointer2Width = 80;
        var pointer2Height = 80;

        var pointer2OffsetX = dialOffsetX + (dialWidth - pointer2Width) / 2;
        var pointer2OffsetY = dialOffsetY + (dialHeight - pointer2Height) / 2;

        var Pointer1Img = new Image();
        Pointer1Img.onload = function()  {
            pointerCanvasContext.drawImage(Pointer1Img, TVReOffsetX + pointer1OffsetX, TVReOffsetY + pointer1OffsetY);
        };
        Pointer1Img.src = "assets/img/pointer-2.png";

        var Pointer2Img = new Image();
        Pointer2Img.onload = function()  {
            dialCanvasContext.drawImage(Pointer2Img, TVReOffsetX + pointer2OffsetX, TVReOffsetY + pointer2OffsetY);
        };
        Pointer2Img.src = "assets/img/pointer-4.png";

        var DialImg = new Image();
        DialImg.onload = function()  {
            dialCanvasContext.drawImage(DialImg, TVReOffsetX + dialOffsetX, TVReOffsetY + dialOffsetY);
        };
        DialImg.src = "assets/img/angleOnTheBowDial.png";

        this.setAngle = function(angle)  {
            // angle given in degrees, in range (-180;180)
            var convertedAngle;
            if(angle > 180 || angle < -180)
                convertedAngle = angle % 180;
            else
                convertedAngle = angle;
            convertedAngle = Math.radians(convertedAngle);

            pointerCanvasContext.clearRect(TVReOffsetX + dialOffsetX, TVReOffsetY + dialOffsetY, DialImg.width, DialImg.height);
            pointerCanvasContext.save();
            pointerCanvasContext.setTransform(1, 0, 0, 1, 0, 0);

            pointerCanvasContext.translate(TVReOffsetX + pointer1OffsetX + pointer1Width/2, TVReOffsetY + pointer1OffsetY + pointer1Height/2);
            pointerCanvasContext.rotate(convertedAngle);
            pointerCanvasContext.translate(-(TVReOffsetX + pointer1OffsetX + pointer1Width/2), -(TVReOffsetY + pointer1OffsetY + pointer1Height/2));
            pointerCanvasContext.drawImage(Pointer1Img, TVReOffsetX + pointer1OffsetX, TVReOffsetY + pointer1OffsetY);
            pointerCanvasContext.restore();

            dialCanvasContext.clearRect(TVReOffsetX + dialOffsetX, TVReOffsetY + dialOffsetY, DialImg.width, DialImg.height);
            dialCanvasContext.save();
            dialCanvasContext.setTransform(1, 0, 0, 1, 0, 0);

            dialCanvasContext.translate(TVReOffsetX + pointer2OffsetX + pointer2Width/2, TVReOffsetY + pointer2OffsetY + pointer2Height/2);
            dialCanvasContext.rotate(convertedAngle * 36);
            dialCanvasContext.translate(-(TVReOffsetX + pointer2OffsetX + pointer2Width/2), -(TVReOffsetY + pointer2OffsetY + pointer2Height/2));
            dialCanvasContext.drawImage(Pointer2Img, TVReOffsetX + pointer2OffsetX, TVReOffsetY + pointer2OffsetY);
            dialCanvasContext.restore();

            dialCanvasContext.drawImage(DialImg, TVReOffsetX + dialOffsetX, TVReOffsetY + dialOffsetY);
        }
}
