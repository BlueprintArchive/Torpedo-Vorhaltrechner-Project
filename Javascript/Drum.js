function Drum(offsetX, offsetY, pointerCanvasContext, scaleFactor, drawLineal)  {
        var m_scaleFactor = scaleFactor;
        var Radius = 25 * m_scaleFactor;
        var DrumWidth = 100 * m_scaleFactor;
        var DrumHeight = 3*Radius;
        var angle = 0;

        var canvasData;

        var m_drawLineal = drawLineal;

        var DrumImg = new Image();
        DrumImg.onload = function()  {
            pointerCanvasContext.drawImage(DrumImg, offsetX, offsetY, DrumWidth, DrumHeight);
            canvasData = pointerCanvasContext.getImageData(offsetX, offsetY, DrumWidth, DrumHeight);
        };
        DrumImg.src = "assets/img/drum.png";

        var linealHeight = 16;
        // offset relative to the zero position (at the height of the center line of the drum)
        var linealCustomOffset = 30;
        var linealOffsetX = offsetX - 10;
        var linealOffsetY = offsetY - linealHeight + Radius - linealCustomOffset;

        // angle offset resulting from the lineal custom offset
        var angleCustomOffset = Math.degrees(Math.asin(linealCustomOffset / Radius));
        if(!m_drawLineal)
            angleCustomOffset = 0;

        var LinealImg = new Image();

        if(m_drawLineal)  {
            LinealImg.onload = function()  {
                pointerCanvasContext.drawImage(LinealImg, linealOffsetX, linealOffsetY);
            }
            LinealImg.src = "assets/img/lineal-zoomed.png";
        }

        function drawPixel(x, y, r, g, b, a)  {
            var index = (Math.round(x) + Math.round(Radius - y) * DrumWidth) * 4;

            canvasData.data[index + 0] = r;
            canvasData.data[index + 1] = g;
            canvasData.data[index + 2] = b;
            canvasData.data[index + 3] = a;
        }

        function updateCurves()  {
            pointerCanvasContext.drawImage(DrumImg, offsetX, offsetY, DrumWidth, DrumHeight);
            canvasData = pointerCanvasContext.getImageData(offsetX, offsetY, DrumWidth, DrumHeight);

            for(var i = 0; i < 2 * Math.PI * Radius; ++i)  {
                var x, y, z;
                var x1, y1, z1;

                x = i;

                y = 0.2142 * x; // 3000 meters
                x1 = Radius * Math.sin(Math.radians(angle)+x/Radius);
                z2 = Radius * Math.cos(Math.radians(angle)+x/Radius);
                y1 = y;
                if(z1 > 0)
                    drawPixel(y1, x1, 255, 255, 255, 255);

                y = 0.2857 * x; // 4000 meters
                x1 = Radius * Math.sin(Math.radians(angle)+x/Radius);
                z2 = Radius * Math.cos(Math.radians(angle)+x/Radius);
                y1 = y;
                if(z1 > 0)
                    drawPixel(y1, x1, 255, 255, 255, 255);

                y = 0.35714 * x; // 5000 meters
                x1 = Radius * Math.sin(Math.radians(angle)+x/Radius);
                z2 = Radius * Math.cos(Math.radians(angle)+x/Radius);
                y1 = y;
                if(z1 > 0)
                    drawPixel(y1, x1, 255, 255, 255, 255);

                y = 0.42863 * x; // 6000 meters
                x1 = Radius * Math.sin(Math.radians(angle)+x/Radius);
                z1 = Radius * Math.cos(Math.radians(angle)+x/Radius);
                y1 = y;
                if(z1 > 0)
                    drawPixel(y1, x1, 255, 255, 255, 255);

                y = 0.5359 * x; // 7500 meters
                x1 = Radius * Math.sin(Math.radians(angle)+x/Radius);
                z1 = Radius * Math.cos(Math.radians(angle)+x/Radius);
                y1 = y;
                if(z1 > 0)
                    drawPixel(y1, x1, 255, 255, 255, 255);

                y = 0.57142 * x; // 8000 meters
                x1 = Radius * Math.sin(Math.radians(angle)+x/Radius);
                z1 = Radius * Math.cos(Math.radians(angle)+x/Radius);
                y1 = y;
                if(z1 > 0)
                    drawPixel(y1, x1, 255, 255, 255, 255);

                y = 0.89285 * x; // 12500 meters
                x1 = Radius * Math.sin(Math.radians(angle)+x/Radius);
                z1 = Radius * Math.cos(Math.radians(angle)+x/Radius);
                y1 = y;
                if(z1 > 0 && y1 < DrumWidth)
                    drawPixel(y1, x1, 255, 255, 255, 255);

            }
            pointerCanvasContext.putImageData(canvasData, offsetX, offsetY);

            if(m_drawLineal)  {
                drawCurvesLabels(170);

                drawCurvesLabels(350);

                pointerCanvasContext.drawImage(LinealImg, linealOffsetX, linealOffsetY);
            }
        }

        function drawCurvesLabels(offset)  {
            // drawing the labels
            pointerCanvasContext.font = "10px Arial";
            pointerCanvasContext.fillStyle = "white";

            var x = offset;

            var y = 0.2142*x;  // 3000 meters
            var x1 = Radius -Radius * Math.sin(Math.radians(angle)+x/Radius);
            var z1 = Radius * Math.cos(Math.radians(angle)+x/Radius);
            var y1 = y - 4; // centering the number over the line
            if(z1 > 0)
                pointerCanvasContext.fillText("30", offsetX + y1, offsetY + x1);

            var y = 0.2857*x;  // 4000 meters
            var x1 = Radius -Radius * Math.sin(Math.radians(angle)+x/Radius);
            var z1 = Radius * Math.cos(Math.radians(angle)+x/Radius);
            var y1 = y - 4; // centering the number over the line
            if(z1 > 0)
                pointerCanvasContext.fillText("40", offsetX + y1, offsetY + x1);

            var y = 0.35714*x;  // 5000 meters
            var x1 = Radius -Radius * Math.sin(Math.radians(angle)+x/Radius);
            var z1 = Radius * Math.cos(Math.radians(angle)+x/Radius);
            var y1 = y - 4; // centering the number over the line
            if(z1 > 0)
                pointerCanvasContext.fillText("50", offsetX + y1, offsetY + x1);

            var y = 0.42863*x;  // 6000 meters
            var x1 = Radius -Radius * Math.sin(Math.radians(angle)+x/Radius);
            var z1 = Radius * Math.cos(Math.radians(angle)+x/Radius);
            var y1 = y - 4; // centering the number over the line
            if(z1 > 0)
                pointerCanvasContext.fillText("60", offsetX + y1, offsetY + x1);

            var y = 0.5359*x;  // 7500 meters
            var x1 = Radius -Radius * Math.sin(Math.radians(angle)+x/Radius) + 5;
            var z1 = Radius * Math.cos(Math.radians(angle)+x/Radius);
            var y1 = y - 8; // centering the number over the line
            if(z1 > 0)
                pointerCanvasContext.fillText("75", offsetX + y1, offsetY + x1);

            var y = 0.57142*x;  // 8000 meters
            var x1 = Radius -Radius * Math.sin(Math.radians(angle)+x/Radius) - 5;
            var z1 = Radius * Math.cos(Math.radians(angle)+x/Radius);
            var y1 = y + 1; // centering the number over the line
            if(z1 > 0)
                pointerCanvasContext.fillText("80", offsetX + y1, offsetY + x1);

            var y = 0.89285*x;  // 125000 meters
            var x1 = Radius -Radius * Math.sin(Math.radians(angle)+x/Radius);
            var z1 = Radius * Math.cos(Math.radians(angle)+x/Radius);
            var y1 = y - 4; // centering the number over the line
            if(z1 > 0)
                pointerCanvasContext.fillText("125", offsetX + y1, offsetY + x1);
        }

        this.set = function(value)  {
            // The input value is the maximum distance to the target for the
            // unitary torpedo range. At the drum is performed multiplication
            // of the "value" and the torpedo range.
            // The interpolation is perfromed between two points:
            // (0, 0) - self explaining
            // (1, 5000) - torpedo range (arbitray selected for the TIII)
            // At the horizontal lineal, the mark 5000 is at the x coordinate:
            // 300 (width of the lineal) - 14000 (max value at the lineal)
            //  y                        -  5000
            // that is: y = 107
            // The multiplication function for TIII torpedo is given with the formula:
            // y = 5/14 * x
            // that is: x = 300
            // Knowing the drum perimeter: 2*PI*Radius, the angle for the x = 300 can
            // be calculated:
            // 2*PI*Radius - 360
            //  300        - angle
            // angle = 229.18 deg
            // The following interpolation can be done:
            // (x0;y0) = (0;0)
            // (x1;y1) = (1;229.18)
            // That means, that final formula is:
            // angle = 229,18*e

            // The custom angle offset is applied due to displacement of the lineal
            // off the center, zero position
            angle = angleCustomOffset + value * (-229.18);
            updateCurves();
        }
}
