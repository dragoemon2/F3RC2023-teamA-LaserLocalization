#include "LaserUse.hpp"
#include "LaserPos.hpp"
#include "parameters.hpp"
#include "simpleFunctions.hpp"
#include <math.h>


_LaserUse::_LaserUse(float minX, float maxX, float minY, float maxY, float errorD, int wallDirection, int wallR, int laserDirection): minX(minX), maxX(maxX), minY(minY), maxY(maxY), errorD(errorD), wallDirection(wallDirection), wallR(wallR), laserDirection(laserDirection) {
    //初期化
    activate();
}


void _LaserUse::activate(){
    active = true;
}

void _LaserUse::inactivate(){
    active = false;
}

LaserUse::LaserUse(float minX, float maxX, float minY, float maxY, float errorD, int wallDirection, int wallR, LaserPos laser): _LaserUse(minX, maxX, minY, maxY, errorD, wallDirection, wallR, laser.direction), laser(laser) {
    //初期化
}

LaserPairUse::LaserPairUse(float minX, float maxX, float minY, float maxY, float errorD, int wallDirection, int wallR, LaserPos laser1, LaserPos laser2): _LaserUse(minX, maxX, minY, maxY, errorD, wallDirection, wallR, laser1.direction), laser1(laser1), laser2(laser2) {
    if(laser1.direction != laser2.direction){
        printf("Exception: failed raisers");
    }
    if(laserDirection == NORTH || laserDirection == SOUTH){
        if(laser1.posY != laser2.posY || laser1.posX == laser2.posX){
            printf("Exception: failed raisers");
        }
    }else if(laserDirection == EAST || laserDirection == WEST){
        if(laser1.posY == laser2.posY || laser1.posX != laser2.posX){
            printf("Exception: failed raisers");
        }
    }
}

bool _LaserUse::check(float X, float Y, float D){
    if(!active){
        return false;
    }
    float diffD = radiansMod(D - (wallDirection - laserDirection)*PI/2);
    return (minX <= X && X < maxX && minY <= Y && Y < maxY && -errorD < diffD && diffD < errorD);
}

void LaserUse::scan(float* X, float* Y, float* D, bool denoise, bool always){
    if(!always && !check(*X, *Y, *D)){
        return;
    }

    float theta1 = wallDirection*PI/2 - *D;
    float theta2 = theta1 - laserDirection*PI/2;

    float length;
    if(denoise){
        length = float(laser.readDenoise());
    }else{
        length = float(laser.read());
    }

    if(length < 0){
        return;
    }

    //ロボットの位置ベクトルと壁の単位法線ベクトルの内積
    float value = wallR - (laser.posX*cos(theta1) + laser.posY*sin(theta1) + length*cos(theta2));

    switch (wallDirection)
    {
    case EAST:
        *X = value;
        break;
    case WEST:
        *X = -value;
        break;
    case NORTH:
        *Y = value;
        break;
    case SOUTH:
        *Y = -value;
        break;
    default:
        break;
    }
}


void LaserPairUse::scan(float* X, float* Y, float* D, bool denoise, bool always){
    if(!always && !check(*X, *Y, *D)){
        return;
    }

    float length1, length2;
    if(denoise){
        length1 = float(laser1.readDenoise());
        length2 = float(laser2.readDenoise());
    }else{
        length1 = float(laser1.read());
        length2 = float(laser2.read());
    }

    if(length1 < 0 || length2 < 0){
        return;
    }

    //まず角度を調べる．

    float beta = laser1.direction*PI/4 + laser2.direction*PI/4; //レーザー向き
    float beta_err = laser1.direction*PI/4 - laser2.direction*PI/4; //いまは0

    float diffX = laser1.posX - laser2.posX;
    float diffY = laser1.posY - laser2.posY;

    float tangent = -(cos(beta)*diffX+sin(beta)*diffY+(length1-length2)*cos(beta_err))/(-sin(beta)*diffX+cos(beta)*diffY+(length1+length2)*sin(beta_err));

    float gamma = atan(tangent); //D - (wallDirection - laserDirection)*PI/2の値

    /*
    switch (wallDirection) {
    case EAST:
        gamma = atan((length1-length2)/(laser1.posY - laser2.posY));
        break;
    
    case WEST:
        gamma = atan((length1-length2)/(laser1.posX - laser2.posX));
        break;
    
    case NORTH:
        gamma = atan(-(length1-length2)/(laser1.posY - laser2.posY));
        break;
    
    case SOUTH:
        gamma = atan(-(length1-length2)/(laser1.posX - laser2.posX));
        break;
    
    default:
        gamma = *D - (wallDirection - laserDirection)*PI/2;
        break;
    }
    */

    *D = wallDirection*PI/2 - beta - gamma;

    //次に距離を調べる

    float theta1 = wallDirection*PI/2 - *D;

    //ロボットの位置ベクトルと壁の単位法線ベクトルの内積
    float value = wallR - (laser1.posX*cos(theta1) + laser1.posY*sin(theta1) + length1*cos(gamma));

    switch (wallDirection)
    {
    case EAST:
        *X = value;
        break;
    case WEST:
        *X = -value;
        break;
    case NORTH:
        *Y = value;
        break;
    case SOUTH:
        *Y = -value;
        break;
    default:
        break;
    }
}
