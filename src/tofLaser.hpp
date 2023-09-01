#pragma once
#include <VL53L0X.h>
#include <mbed.h>
#include "_laser.hpp"


class TOFLaser: public _Laser
{
    public:
        DigitalInOut xshut;
        VL53L0X sensor;
        I2C i2c;
        Timer timer;

        //異なるTOFに異なるアドレスを書き込むために，非負整数idを用いて区別する．0から順に指定していけばOK
        TOFLaser(unsigned int id, PinName xshut_pin, PinName sda_pin=D14, PinName scl_pin=D15);
        void init();

        //値の読み取り．読み取れなかったときは負の値を返す．
        //エラー -1:値が極端に大きいor小さい，-2:読み取りが間に合わなかった，-3:タイムアウト発生，-4:通信エラー．再接続を試みる．
        int read();
        int readDenoise();

        //通信遮断時の再接続
        void restart();
    private:
        int address;
        int last_value;
};
