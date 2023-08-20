#pragma once
#include <VL53L0X.h>
#include <mbed.h>
#include "_laser.hpp"


class TOFLaser: public _Laser
{
    public:
        DigitalOut xshut;
        VL53L0X sensor;
        I2C i2c;
        Timer timer;

        TOFLaser(unsigned int id, PinName xshut_pin, PinName sda_pin=PB_3, PinName scl_pin=PB_10);
        void init();
        int read();
        int readDenoise();
        void restart();
    private:
        int address;
        int last_value;
};
