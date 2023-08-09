#include "laser.hpp"
#include "laserPos.hpp"

LaserPos::LaserPos(int posX, int posY, int direction, PinName pin): Laser(pin), posX(posX), posY(posY), direction(direction) {

}