#include "laserUse.hpp"
#include "laserPos.hpp"
#include "parameters.hpp"
#include <mbed.h>

LaserPos laser1(-200,0,SOUTH,A2);
LaserPos laser2(200,0,SOUTH,A3);

LaserPairUse laserPair(-1000,1000,-1000,10000,PI/4,SOUTH,0,laser1,laser2);

int main(){
    float X = 0;
    float Y = 1000;
    float D = 0;
    while(true){
        laserPair.scan(&X, &Y, &D, false, true);
        //printf("%d %d\n", int(laser1.read()), int(laser2.read()));
        //printf("%d %d\n", int(laser1.readDenoise()), int(laser2.readDenoise()));

        printf("%d %d %d\n", int(X), int(Y), int(180*D/PI));
        wait_us(100000);
    }
}