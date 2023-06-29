function GyroAngleDial(TVReOffsetX, TVReOffsetY, dialCanvasContext, pointerCanvasContext)  {
        var gyroAngle = 0;
        var spreadAngle = 0;

        var dialOffsetX = 410;
        var dialOffsetY = 256;

        var dialWidth = 232;
        var dialHeight = 120;

        var pointer1Width = 23;
        var pointer1Height = 82;

        var pointer1OffsetX = dialOffsetX + 59 - pointer1Width / 2;
        var pointer1OffsetY = dialOffsetY + (dialHeight - pointer1Height) / 2;

        var pointer2Width = 140;
        var pointer2Height = 140;

        var pointer2OffsetX = dialOffsetX + 172 - pointer2Width / 2;
        var pointer2OffsetY = dialOffsetY + (dialHeight - pointer2Height) / 2 - 1;

        var DialImg = new Image();
        DialImg.onload = function()  {
            dialCanvasContext.drawImage(DialImg, TVReOffsetX + dialOffsetX, TVReOffsetY + dialOffsetY);
        };
        DialImg.src = "assets/img/gyroAngleDial.png";

        var PointerImg1 = new Image();
        PointerImg1.onload = function()  {
            pointerCanvasContext.drawImage(PointerImg1, TVReOffsetX + pointer1OffsetX, TVReOffsetY + pointer1OffsetY);
        };
        PointerImg1.src = "assets/img/pointer-2.png";

        var PointerImg2 = new Image();
        PointerImg2.onload = function()  {
            pointerCanvasContext.drawImage(PointerImg2, TVReOffsetX + pointer2OffsetX, TVReOffsetY + pointer2OffsetY);
        };
        PointerImg2.src = "assets/img/pointer-5.png";

        var blendWidth = 120;
        var blendHeight = 120;

        var blendOffsetX = dialOffsetX + 172 - blendWidth / 2;
        var blendOffsetY = dialOffsetY + (dialHeight - blendHeight) / 2;

        var Blend1aImg = new Image();
        Blend1aImg.onload = function()  {
            pointerCanvasContext.drawImage(Blend1aImg, TVReOffsetX + blendOffsetX, TVReOffsetY + blendOffsetY);
        };
        Blend1aImg.src = "assets/img/blend1a.png";

        var Blend1bImg = new Image();
        Blend1bImg.onload = function()  {
            pointerCanvasContext.drawImage(Blend1bImg, TVReOffsetX + blendOffsetX, TVReOffsetY + blendOffsetY);
        };
        Blend1bImg.src = "assets/img/blend1b.png";

        var Blend2aImg = new Image();
        Blend2aImg.onload = function()  {
            pointerCanvasContext.drawImage(Blend2aImg, TVReOffsetX + blendOffsetX, TVReOffsetY + blendOffsetY);
        };
        Blend2aImg.src = "assets/img/blend2a.png";

        var Blend2bImg = new Image();
        Blend2bImg.onload = function()  {
            pointerCanvasContext.drawImage(Blend2bImg, TVReOffsetX + blendOffsetX, TVReOffsetY + blendOffsetY);
        };
        Blend2bImg.src = "assets/img/blend2b.png";

        var Blend3aImg = new Image();
        Blend3aImg.onload = function()  {
            pointerCanvasContext.drawImage(Blend3aImg, TVReOffsetX + blendOffsetX, TVReOffsetY + blendOffsetY);
        };
        Blend3aImg.src = "assets/img/blend3a.png";

        var Blend3bImg = new Image();
        Blend3bImg.onload = function()  {
            pointerCanvasContext.drawImage(Blend3bImg, TVReOffsetX + blendOffsetX, TVReOffsetY + blendOffsetY);
        };
        Blend3bImg.src = "assets/img/blend3b.png";

        this.setGyroAngle = function(angle)  {
            // angle given in degrees, in range (0;360)
            gyroAngle = Math.radians(angle);

            this.drawPointers();
        }

        this.setSpreadAngle = function(angle)  {
            // angle given in degrees, in range (0;20)
            var convertedAngle;
            if(angle > 20)
                convertedAngle = angle % 20;
            else if(angle < 0)
                convertedAngle = -angle % 20;
            else
                convertedAngle = angle;

            spreadAngle = Math.radians(convertedAngle);

            this.drawPointers();
        }

        this.drawPointers = function()  {
            pointerCanvasContext.clearRect(TVReOffsetX + dialOffsetX, TVReOffsetY + dialOffsetY, DialImg.width, DialImg.height);

            pointerCanvasContext.save();
            pointerCanvasContext.setTransform(1, 0, 0, 1, 0, 0);

            pointerCanvasContext.translate(TVReOffsetX + pointer1OffsetX + pointer1Width/2, TVReOffsetY + pointer1OffsetY + pointer1Height/2);
            pointerCanvasContext.rotate(gyroAngle * 36);
            pointerCanvasContext.translate(-(TVReOffsetX + pointer1OffsetX + pointer1Width/2), -(TVReOffsetY + pointer1OffsetY + pointer1Height/2));
            pointerCanvasContext.drawImage(PointerImg1, TVReOffsetX + pointer1OffsetX, TVReOffsetY + pointer1OffsetY);
            pointerCanvasContext.restore();

            pointerCanvasContext.save();
            pointerCanvasContext.setTransform(1, 0, 0, 1, 0, 0);

            pointerCanvasContext.translate(TVReOffsetX + pointer2OffsetX + pointer2Width/2, TVReOffsetY + pointer2OffsetY + pointer2Height/2);
            pointerCanvasContext.rotate(gyroAngle);
            pointerCanvasContext.translate(-(TVReOffsetX + pointer2OffsetX + pointer2Width/2), -(TVReOffsetY + pointer2OffsetY + pointer2Height/2));
            pointerCanvasContext.drawImage(PointerImg2, TVReOffsetX + pointer2OffsetX, TVReOffsetY + pointer2OffsetY);
            pointerCanvasContext.restore();

            pointerCanvasContext.save();
            pointerCanvasContext.setTransform(1, 0, 0, 1, 0, 0);

            pointerCanvasContext.translate(TVReOffsetX + blendOffsetX + blendWidth/2, TVReOffsetY + blendOffsetY + blendHeight/2);
            pointerCanvasContext.rotate(spreadAngle * 0.5);
            pointerCanvasContext.translate(-(TVReOffsetX + blendOffsetX + blendWidth/2), -(TVReOffsetY + blendOffsetY + blendHeight/2));
            pointerCanvasContext.drawImage(Blend1aImg, TVReOffsetX + blendOffsetX, TVReOffsetY + blendOffsetY);
            pointerCanvasContext.restore();

            pointerCanvasContext.save();
            pointerCanvasContext.setTransform(1, 0, 0, 1, 0, 0);

            pointerCanvasContext.translate(TVReOffsetX + blendOffsetX + blendWidth/2, TVReOffsetY + blendOffsetY + blendHeight/2);
            pointerCanvasContext.rotate(-spreadAngle * 0.5);
            pointerCanvasContext.translate(-(TVReOffsetX + blendOffsetX + blendWidth/2), -(TVReOffsetY + blendOffsetY + blendHeight/2));
            pointerCanvasContext.drawImage(Blend1bImg, TVReOffsetX + blendOffsetX, TVReOffsetY + blendOffsetY);
            pointerCanvasContext.restore();

            pointerCanvasContext.save();
            pointerCanvasContext.setTransform(1, 0, 0, 1, 0, 0);

            pointerCanvasContext.translate(TVReOffsetX + blendOffsetX + blendWidth/2, TVReOffsetY + blendOffsetY + blendHeight/2);
            pointerCanvasContext.rotate(spreadAngle);
            pointerCanvasContext.translate(-(TVReOffsetX + blendOffsetX + blendWidth/2), -(TVReOffsetY + blendOffsetY + blendHeight/2));
            pointerCanvasContext.drawImage(Blend2aImg, TVReOffsetX + blendOffsetX, TVReOffsetY + blendOffsetY);
            pointerCanvasContext.restore();

            pointerCanvasContext.save();
            pointerCanvasContext.setTransform(1, 0, 0, 1, 0, 0);

            pointerCanvasContext.translate(TVReOffsetX + blendOffsetX + blendWidth/2, TVReOffsetY + blendOffsetY + blendHeight/2);
            pointerCanvasContext.rotate(-spreadAngle);
            pointerCanvasContext.translate(-(TVReOffsetX + blendOffsetX + blendWidth/2), -(TVReOffsetY + blendOffsetY + blendHeight/2));
            pointerCanvasContext.drawImage(Blend2bImg, TVReOffsetX + blendOffsetX, TVReOffsetY + blendOffsetY);
            pointerCanvasContext.restore();

            pointerCanvasContext.save();
            pointerCanvasContext.setTransform(1, 0, 0, 1, 0, 0);

            pointerCanvasContext.translate(TVReOffsetX + blendOffsetX + blendWidth/2, TVReOffsetY + blendOffsetY + blendHeight/2);
            pointerCanvasContext.rotate(spreadAngle * 1.5);
            pointerCanvasContext.translate(-(TVReOffsetX + blendOffsetX + blendWidth/2), -(TVReOffsetY + blendOffsetY + blendHeight/2));
            pointerCanvasContext.drawImage(Blend3aImg, TVReOffsetX + blendOffsetX, TVReOffsetY + blendOffsetY);
            pointerCanvasContext.restore();

            pointerCanvasContext.save();
            pointerCanvasContext.setTransform(1, 0, 0, 1, 0, 0);

            pointerCanvasContext.translate(TVReOffsetX + blendOffsetX + blendWidth/2, TVReOffsetY + blendOffsetY + blendHeight/2);
            pointerCanvasContext.rotate(-spreadAngle * 1.5);
            pointerCanvasContext.translate(-(TVReOffsetX + blendOffsetX + blendWidth/2), -(TVReOffsetY + blendOffsetY + blendHeight/2));
            pointerCanvasContext.drawImage(Blend3bImg, TVReOffsetX + blendOffsetX, TVReOffsetY + blendOffsetY);
            pointerCanvasContext.restore();
        }
}
