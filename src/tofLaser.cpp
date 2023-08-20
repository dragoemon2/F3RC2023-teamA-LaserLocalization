#include "tofLaser.hpp"


#define DEFAULT_ADDRESS (0x29)
#define READ_DENOISE_N (15)


TOFLaser::TOFLaser(unsigned int id, PinName xshut_pin, PinName sda_pin, PinName scl_pin): xshut(xshut_pin), i2c(sda_pin, scl_pin), sensor(i2c, timer)
{
    xshut.write(1);
    address = DEFAULT_ADDRESS + 2 + 2*id;
    //init();
    last_value = -1;
}

void TOFLaser::init(){
    bool result;

    xshut.write(1);
    //i2c.timeout(100);

    sensor.setTimeout(100);
    sensor.setMeasurementTimingBudget(33000);
    sensor.setAddress(address);
    result = sensor.init();
    printf("%d\n",result);
    sensor.startContinuous(0);
}

void TOFLaser::restart(){
    //i2c.write(DEFAULT_ADDRESS, (const char *)buffer, 2, 0);
    sensor.stopContinuous();
    sensor.setAddress(DEFAULT_ADDRESS);
    init();
}

//値の読み取り(ノイズ除去なし)
int TOFLaser::read(){
    int value = last_value;
    if(sensor.readReg(VL53L0X::RESULT_INTERRUPT_STATUS) & 0x07){
        value = sensor.readRangeContinuousMillimeters(false);
        last_value = value;

        if(value > 8000 || value < 10){
            value = -1;
        }
    }else{
        //新しく読めてなかったときの処理
        value = -2;
    }

    if(sensor.timeoutOccurred()){
        restart();
        return -3;
    }

    if(value == 0x0b01){
        restart();
        return -4;
    }

    return value;
}

int TOFLaser::readDenoise(){
    return read();
}