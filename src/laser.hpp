#pragma once
#include <mbed.h>
#include <vector>
//#include "flash_api.h"
#include "_laser.hpp"

//DT35のレーザー
class Laser: public _Laser
{
public:
    PinName pin;
    AnalogIn analogin;

    int readDenoise(); //割り込みで実行するな!
    int read(); //割り込みで実行するな!
    void calibrate(int distance); //割り込みで実行するな!
    Laser(PinName pin); 

    void loadCalibrationFromFlash();
    void saveCalibrationToFlash();

    float A;
    float B; 

private:
    int convert(float input);
    int calibrationFit(bool save=true);
    
    vector<int> distances={};
    vector<int> outputs={};
    int calibrationNum=0;
};