function ParallaxAngleDial(TVReOffsetX, TVReOffsetY, dialCanvasContext, pointerCanvasContext)  {
        var angle1 = 0;
        var angle2 = 0;

        var dialOffsetX = 413;
        var dialOffsetY = 120;

        var dialWidth = 105;
        var dialHeight = 104;

        var pointer1Width = 23;
        var pointer1Height = 82;

        var pointer1OffsetX = dialOffsetX + (dialWidth - pointer1Width) / 2;
        var pointer1OffsetY = dialOffsetY + (dialHeight - pointer1Height) / 2;

        var pointer2Width = 20;
        var pointer2Height = 105;

        var pointer2OffsetX = dialOffsetX + (dialWidth - pointer2Width) / 2;
        var pointer2OffsetY = dialOffsetY + (dialHeight - pointer2Height) / 2;

        var DialImg = new Image();
        DialImg.onload = function()  {
            dialCanvasContext.drawImage(DialImg, TVReOffsetX + dialOffsetX, TVReOffsetY + dialOffsetY);
        };
        DialImg.src = "assets/img/parallaxAngleDial.png";

        var PointerImg1 = new Image();
        PointerImg1.onload = function()  {
            pointerCanvasContext.drawImage(PointerImg1, TVReOffsetX + pointer1OffsetX, TVReOffsetY + pointer1OffsetY);
        };
        PointerImg1.src = "assets/img/pointer-2.png";

        var PointerImg2 = new Image();
        PointerImg2.onload = function()  {
            pointerCanvasContext.drawImage(PointerImg2, TVReOffsetX + pointer2OffsetX, TVReOffsetY + pointer2OffsetY);
        };
        PointerImg2.src = "assets/img/pointer-3.png";

        this.setAngle1 = function(angle)  {
            // angle given in degrees, in range (-25;25)
            var convertedAngle;
            if(angle > 25 || angle < -25)
                convertedAngle = angle % 25;
            else
                convertedAngle = angle;
            convertedAngle = convertedAngle * 6;
            convertedAngle = Math.radians(convertedAngle);

            angle1 = convertedAngle;

            this.drawPointers();
        }

        this.setAngle2 = function(angle)  {
            // angle given in degrees, in range (-25;25)
            var convertedAngle;
            if(angle > 25 || angle < -25)
                convertedAngle = angle % 25;
            else
                convertedAngle = angle;
            convertedAngle = convertedAngle * 6;
            convertedAngle = Math.radians(convertedAngle);

            angle2 = convertedAngle;

            this.drawPointers();
        }

        this.drawPointers = function()  {
            pointerCanvasContext.clearRect(TVReOffsetX + dialOffsetX, TVReOffsetY + dialOffsetY, DialImg.width, DialImg.height);

            pointerCanvasContext.save();
            pointerCanvasContext.setTransform(1, 0, 0, 1, 0, 0);

            pointerCanvasContext.translate(TVReOffsetX + pointer1OffsetX + pointer1Width/2, TVReOffsetY + pointer1OffsetY + pointer1Height/2);
            pointerCanvasContext.rotate(angle1);
            pointerCanvasContext.translate(-(TVReOffsetX + pointer1OffsetX + pointer1Width/2), -(TVReOffsetY + pointer1OffsetY + pointer1Height/2));
            pointerCanvasContext.drawImage(PointerImg1, TVReOffsetX + pointer1OffsetX, TVReOffsetY + pointer1OffsetY);
            pointerCanvasContext.restore();

            pointerCanvasContext.save();
            pointerCanvasContext.setTransform(1, 0, 0, 1, 0, 0);

            pointerCanvasContext.translate(TVReOffsetX + pointer2OffsetX + pointer2Width/2, TVReOffsetY + pointer2OffsetY + pointer2Height/2);
            pointerCanvasContext.rotate(angle2);
            pointerCanvasContext.translate(-(TVReOffsetX + pointer2OffsetX + pointer2Width/2), -(TVReOffsetY + pointer2OffsetY + pointer2Height/2));
            pointerCanvasContext.drawImage(PointerImg2, TVReOffsetX + pointer2OffsetX, TVReOffsetY + pointer2OffsetY);
            pointerCanvasContext.restore();
        }
}
