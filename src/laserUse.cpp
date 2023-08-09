#include "LaserUse.hpp"
#include "LaserPos.hpp"
#include "parameters.hpp"
#include <math.h>

float radiansMod(float x, float y=2*PI){
    //x (mod y) を -y/2 ~ y/2の範囲で出力
    //主に2つの方角のなす角度を計算するのに使用
    return fmod((fmod(x,y) + y/2),y) + y/2;
}

_LaserUse::_LaserUse(float minX, float maxX, float minY, float maxY, float errorD, int wallDirection, int wallR, int laserDirection): minX(minX), maxX(maxX), minY(minY), maxY(maxY), errorD(errorD), wallDirection(wallDirection), wallR(wallR), laserDirection(laserDirection) {
    //初期化
}


LaserUse::LaserUse(float minX, float maxX, float minY, float maxY, float errorD, int wallDirection, int wallR, LaserPos laser): _LaserUse(minX, maxX, minY, maxY, errorD, wallDirection, wallR, laser.direction), laser(laser) {
    //初期化
}

LaserPairUse::LaserPairUse(float minX, float maxX, float minY, float maxY, float errorD, int wallDirection, int wallR, LaserPos laser1, LaserPos laser2): _LaserUse(minX, maxX, minY, maxY, errorD, wallDirection, wallR, laser1.direction), laser1(laser1), laser2(laser2) {
    if(laser1.direction != laser2.direction){
        printf("Exception: failed raisers");
    }
    if(laserDirection == NORTH || laserDirection == SOUTH){
        if(laser1.posY != laser2.posY || laser1.posX == laser2.posY){
            printf("Exception: failed raisers");
        }
    }else if(laserDirection == EAST || laserDirection == WEST){
        if(laser1.posY == laser2.posY || laser1.posX != laser2.posY){
            printf("Exception: failed raisers");
        }
    }
}

bool _LaserUse::check(float X, float Y, float D){
    float diffD = radiansMod(D - (wallDirection - laserDirection)*PI/2);
    return (minX <= X && X < maxX && minY <= Y && Y < maxY && -errorD < diffD && diffD < errorD);
}

void LaserUse::scan(float* X, float* Y, float* D, bool denoise){
    if(!check(*X, *Y, *D)){
        return;
    }

    float theta1 = *D - wallDirection*PI/2;
    float theta2 = theta1 + laserDirection*PI/2;

    int length;
    if(denoise){
        length = laser.readDenoise();
    }else{
        length = laser.read();
    }

    //ロボットの位置ベクトルと壁の単位法線ベクトルの内積
    float value = wallR + laser.posX*cos(theta1) + laser.posY*sin(theta1) - length*cos(theta2);

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


void LaserPairUse::scan(float* X, float* Y, float* D, bool denoise){
    if(!check(*X, *Y, *D)){
        return;
    }

    int length1, length2;
    if(denoise){
        length1 = laser1.readDenoise();
        length2 = laser2.readDenoise();
    }else{
        length1 = laser1.read();
        length2 = laser2.readDenoise();
    }

    //まず角度を調べる．

    float gamma; //D - (wallDirection - laserDirection)*PI/2の値
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

    *D = gamma + (wallDirection - laserDirection)*PI/2;

    //次に距離を調べる．

    float theta1 = *D - wallDirection*PI/2;
    float theta2 = theta1 + laserDirection*PI/2;

    //ロボットの位置ベクトルと壁の単位法線ベクトルの内積
    float value = wallR - (laser1.posX*cos(theta1) + laser1.posY*sin(theta1) + length1*cos(theta2));

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
