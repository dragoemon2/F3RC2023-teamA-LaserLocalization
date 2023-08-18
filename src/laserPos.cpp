#include "laser.hpp"
#include "laserPos.hpp"

LaserPos::LaserPos(int posX, int posY, float direction, PinName pin): Laser(pin), posX(posX), posY(posY), direction(direction) {

}