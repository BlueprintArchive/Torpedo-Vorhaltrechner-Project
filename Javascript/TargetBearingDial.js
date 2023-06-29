function TargetBearingDial(TVReOffsetX, TVReOffsetY, dialCanvasContext, pointerCanvasContext)  {
        var angle1 = 0;
        var angle2 = 0;

        var dialOffsetX = 703;
        var dialOffsetY = 212;

        var dialWidth = 105;
        var dialHeight = 206;

        var pointer1Width = 23;
        var pointer1Height = 82;

        var pointer1aOffsetX = dialOffsetX + (dialWidth - pointer1Width) / 2;
        var pointer1aOffsetY = dialOffsetY + 53 - pointer1Height / 2;

        var pointer1bOffsetX = dialOffsetX + (dialWidth - pointer1Width) / 2;
        var pointer1bOffsetY = dialOffsetY + 153 - pointer1Height / 2;

        var pointer2Width = 20;
        var pointer2Height = 105;

        var pointer2aOffsetX = dialOffsetX + (dialWidth - pointer2Width) / 2;
        var pointer2aOffsetY = dialOffsetY + 53 - pointer2Height / 2;

        var pointer2bOffsetX = dialOffsetX + (dialWidth - pointer2Width) / 2;
        var pointer2bOffsetY = dialOffsetY +  153 - pointer2Height / 2;

        var DialImg = new Image();
        DialImg.onload = function()  {
            dialCanvasContext.drawImage(DialImg, TVReOffsetX + dialOffsetX, TVReOffsetY + dialOffsetY);
        };
        DialImg.src = "assets/img/targetBearingDial.png";

        var PointerImg1 = new Image();
        PointerImg1.onload = function()  {
            pointerCanvasContext.drawImage(PointerImg1, TVReOffsetX + pointer1aOffsetX, TVReOffsetY + pointer1aOffsetY);
            pointerCanvasContext.drawImage(PointerImg1, TVReOffsetX + pointer1bOffsetX, TVReOffsetY + pointer1bOffsetY);
        };
        PointerImg1.src = "assets/img/pointer-2.png";

        var PointerImg2 = new Image();
        PointerImg2.onload = function()  {
            pointerCanvasContext.drawImage(PointerImg2, TVReOffsetX + pointer2aOffsetX, TVReOffsetY + pointer2aOffsetY);
            pointerCanvasContext.drawImage(PointerImg2, TVReOffsetX + pointer2bOffsetX, TVReOffsetY + pointer2bOffsetY);
        };
        PointerImg2.src = "assets/img/pointer-3.png";

        this.setAngle1 = function(angle)  {
            // angle given in degrees, in range (0;360)
            angle1 = Math.radians(angle);

            this.drawPointers();
        }

        this.setAngle2 = function(angle)  {
            // angle given in degrees, in range (0;360)
            angle2 = Math.radians(angle);

            this.drawPointers();
        }

        this.drawPointers = function()  {
            pointerCanvasContext.clearRect(TVReOffsetX + dialOffsetX, TVReOffsetY + dialOffsetY, DialImg.width, DialImg.height);

            pointerCanvasContext.save();
            pointerCanvasContext.setTransform(1, 0, 0, 1, 0, 0);

            pointerCanvasContext.translate(TVReOffsetX + pointer1aOffsetX + pointer1Width/2, TVReOffsetY + pointer1aOffsetY + pointer1Height/2);
            pointerCanvasContext.rotate(angle1 * 36);
            pointerCanvasContext.translate(-(TVReOffsetX + pointer1aOffsetX + pointer1Width/2), -(TVReOffsetY + pointer1aOffsetY + pointer1Height/2));
            pointerCanvasContext.drawImage(PointerImg1, TVReOffsetX + pointer1aOffsetX, TVReOffsetY + pointer1aOffsetY);
            pointerCanvasContext.restore();

            pointerCanvasContext.save();
            pointerCanvasContext.setTransform(1, 0, 0, 1, 0, 0);

            pointerCanvasContext.translate(TVReOffsetX + pointer2aOffsetX + pointer2Width/2, TVReOffsetY + pointer2aOffsetY + pointer2Height/2);
            pointerCanvasContext.rotate(angle2 * 36);
            pointerCanvasContext.translate(-(TVReOffsetX + pointer2aOffsetX + pointer2Width/2), -(TVReOffsetY + pointer2aOffsetY + pointer2Height/2));
            pointerCanvasContext.drawImage(PointerImg2, TVReOffsetX + pointer2aOffsetX, TVReOffsetY + pointer2aOffsetY);
            pointerCanvasContext.restore();

            pointerCanvasContext.save();
            pointerCanvasContext.setTransform(1, 0, 0, 1, 0, 0);

            pointerCanvasContext.translate(TVReOffsetX + pointer1bOffsetX + pointer1Width/2, TVReOffsetY + pointer1bOffsetY + pointer1Height/2);
            pointerCanvasContext.rotate(angle1);
            pointerCanvasContext.translate(-(TVReOffsetX + pointer1bOffsetX + pointer1Width/2), -(TVReOffsetY + pointer1bOffsetY + pointer1Height/2));
            pointerCanvasContext.drawImage(PointerImg1, TVReOffsetX + pointer1bOffsetX, TVReOffsetY + pointer1bOffsetY);
            pointerCanvasContext.restore();

            pointerCanvasContext.save();
            pointerCanvasContext.setTransform(1, 0, 0, 1, 0, 0);

            pointerCanvasContext.translate(TVReOffsetX + pointer2bOffsetX + pointer2Width/2, TVReOffsetY + pointer2bOffsetY + pointer2Height/2);
            pointerCanvasContext.rotate(angle2);
            pointerCanvasContext.translate(-(TVReOffsetX + pointer2bOffsetX + pointer2Width/2), -(TVReOffsetY + pointer2bOffsetY + pointer2Height/2));
            pointerCanvasContext.drawImage(PointerImg2, TVReOffsetX + pointer2bOffsetX, TVReOffsetY + pointer2bOffsetY);
            pointerCanvasContext.restore();
        }
}
