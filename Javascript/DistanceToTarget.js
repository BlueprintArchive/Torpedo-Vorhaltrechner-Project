function DistanceToTarget(a)  {
    var angles = [];
    angles[0] = 0;
    angles[1] = 13.54;
    angles[2] = 26.57;
    angles[3] = 37.57;
    angles[4] = 46.89;
    angles[5] = 55.78;
    angles[6] = 64.75;
    angles[7] = 71.57;
    angles[8] = 78.5;
    angles[9] = 84.29;
    angles[10] = 90.97;
    angles[11] = 95.91;
    angles[12] = 101.89;
    angles[13] = 106.61;
    angles[14] = 111.1;
    angles[15] = 149.59;
    angles[16] = 177.88;
    angles[17] = 200.07;
    angles[18] = 217.63;
    angles[19] = 231.47;
    angles[20] = 245.17;
    angles[21] = 256.37;
    angles[22] = 265.73;
    angles[23] = 274.90;
    angles[24] = 282.80;
    angles[25] = 290.06;
    angles[26] = 296.57;
    angles[27] = 303.94;
    angles[28] = 309.09;
    angles[29] = 315.63;
    angles[30] = 320.65;
    angles[31] = 325.81;
    angles[32] = 330.59;

    var distances = [];
    distances[0] = 300;
    distances[1] = 350;
    distances[2] = 400;
    distances[3] = 450;
    distances[4] = 500;
    distances[5] = 550;
    distances[6] = 600;
    distances[7] = 650;
    distances[8] = 700;
    distances[9] = 750;
    distances[10] = 800;
    distances[11] = 850;
    distances[12] = 900;
    distances[13] = 950;
    distances[14] = 1000;
    distances[15] = 1500;
    distances[16] = 2000;
    distances[17] = 2500;
    distances[18] = 3000;
    distances[19] = 3500;
    distances[20] = 4000;
    distances[21] = 4500;
    distances[22] = 5000;
    distances[23] = 5500;
    distances[24] = 6000;
    distances[25] = 6500;
    distances[26] = 7000;
    distances[27] = 7500;
    distances[28] = 8000;
    distances[29] = 8500;
    distances[30] = 9000;
    distances[31] = 9500;
    distances[32] = 10000;

    var angle =  a - 12.43;
    if(angle < 0)
        angle = 360 + angle;

    var i;
    for(i = 0; i < 33; ++i)  {
        if(angle < angles[i])
            break; 
    }
    if(i == 33)
        --i;

    var angle1 = angles[i-1];
    var angle2 = angles[i];
    distance = (angle - angle1)*(distances[i] - distances[i-1])/(angle2 - angle1) + distances[i-1];
    return distance;    
}

