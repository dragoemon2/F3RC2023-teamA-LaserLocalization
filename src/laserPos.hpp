#pragma once

#include "laser.hpp"
#include <mbed.h>
#include "parameters.hpp"

#define EAST  (0)
#define NORTH (1)
#define WEST  (2)
#define SOUTH (3)

//Laserを継承したLaserPosクラス．ロボット上でのレーザーの位置に関する情報を含む
class LaserPos : public Laser
{
    public:
        int posX;
        int posY;
        int direction;

        LaserPos(int posX, int posY, int direction, PinName pin);
};