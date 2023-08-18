#include "laserUse.hpp"
#include "laserPos.hpp"
#include "parameters.hpp"
#include <mbed.h>

/*
LaserPos laser1(-200,0,SOUTH,A2);

int main(){
    while(1){
        wait_us(100000);
        printf("%d,%d\n", laser1.read(), laser1.readDenoise());
    }
}
*/



LaserPos laser1(-150,0,SOUTH,A2);
LaserPos laser2(150,0,SOUTH,A3);

LaserPairUse laserPair(-1000,1000,-1000,10000,PI/4,SOUTH,0,laser1,laser2);

int main(){
    float X = 0;
    float Y = 1000;
    float D = 0;
    while(true){
        //laserPair.scan(&X, &Y, &D, true, true);
        lu.scan(&X, &Y, &D, true, true);
        //printf("%d %d\n", int(laser1.analogin.read()*10000), int(laser2.analogin.read()*10000));
        //printf("%d %d\n", int(laser1.readDenoise()), int(laser2.readDenoise()));

        printf("%d %d %d| %d %d\n", int(X), int(Y), int(180*D/PI), int(laser1.readDenoise()), int(laser2.readDenoise()));
        wait_us(100000);
    }
}
