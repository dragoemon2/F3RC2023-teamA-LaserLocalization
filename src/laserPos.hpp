#pragma once

#include "laser.hpp"
#include "_laser.hpp"
#include "tofLaser.hpp"
#include <mbed.h>
#include "parameters.hpp"

#define EAST  (0)
#define NORTH (1)
#define WEST  (2)
#define SOUTH (3)


//Laser, _LaserPosを継承したLaserPosクラス．ロボット上でのレーザーの位置に関する情報を含む
class LaserPos
{
    public:
        _Laser& laser;
        int posX;
        int posY;
        float direction;
        LaserPos(int posX, int posY, float direction, _Laser& laser);
        int read();
        int readDenoise();
};




