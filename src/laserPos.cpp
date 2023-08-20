#include "laser.hpp"
#include "laserPos.hpp"

LaserPos::LaserPos(int posX, int posY, float direction, _Laser& laser): posX(posX), posY(posY), direction(direction), laser(laser)  {

}

int LaserPos::read(){
    return laser.read();
}

int LaserPos::readDenoise(){
    return laser.readDenoise();
}


