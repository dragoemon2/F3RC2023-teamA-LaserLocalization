#include <mbed.h>
#include "parameters.hpp"

/* parameters.hppの記述
#define PI (3.141592653589793)

#define EAST  (0)
#define NORTH (1)
#define WEST  (2)
#define SOUTH (3)
*/

#include "tofLaser.hpp"
TOFLaser laser1(0, D8, PB_3, PB_10);


int main(){
    laser1.init();

    while(1){
        printf("%d\n", laser1.read());
        //printf("%d %d\n", laser1.read(), laser3.read());
        wait_us(100000);
    }
}

#if 0
//----レーザーの設定----
#include "laser.hpp"
#include "tofLaser.hpp"

//DT35
Laser laserCore1(PC_1);
Laser laserCore2(PC_0);

//VL53L0X
TOFLaser laserCore3(0, PA_13, PB_3, PB_10);
TOFLaser laserCore4(1, PA_13, PB_3, PB_10);

//----ロボット上でのレーザーの位置の指定----
#include "laserPos.hpp"
LaserPos laser1(
    100, //DT35レーザー位置のx座標
    -150, //DT35レーザー位置のy座標
    SOUTH,
    laserCore1
);

LaserPos laser2(
    -100, //DT35レーザー位置のx座標
    -150, //DT35レーザー位置のy座標
    SOUTH,
    laserCore2
);

LaserPos laser3(
    -150, //VL53L0Xレーザー位置のx座標
    -100, //VL53L0Xレーザー位置のy座標
    WEST, //ロボットが東を向いた時の向き
    laserCore3
);

LaserPos laser4(
    -150, //VL53L0Xレーザー位置のx座標
    100, //VL53L0Xレーザー位置のy座標
    WEST, //ロボットが東を向いた時の向き
    laserCore4
);

//----レーザー自己位置推定の使用パターンと使える条件の指定----
#include "laserUse.hpp"

//レーザー2つ
LaserPairUse lu1(
    1000, //X座標の最小値
    2000, //X座標の最大値
    0, //Y座標の最小値
    2500, //Y座標の最大値
    PI/8, //方向のずれの範囲
    SOUTH, //壁の方角
    38, //原点から壁の距離
    laser1, //レーザー
    laser2 //レーザー
);

//レーザー1つ
LaserUse lu2(
    1000, //X座標の最小値
    2000, //X座標の最大値
    0, //Y座標の最小値
    500, //Y座標の最大値
    PI/8, //方向のずれの範囲
    WEST, //壁の方角
    2000, //原点から壁の距離
    laser3 //レーザー
);

//----使い方----
int main(){
    //VL53L0Xにアドレス書き込み，初期化
    laserCore3.init();
    laserCore4.init();

    //レーザーの値を読む int[mm]　読めないと負の値となる．
    //割り込み内では使用できない
    laser1.read(); //laserCore1.read()でもOK
    laser2.read(); //laserCore2.read()でもOK
    laser3.read(); //laserCore3.read()でもOK
    laser4.read(); //laserCore4.read()でもOK

    //DT35の値をノイズ除去して読む int[mm]
    //割り込み内では使用できない
    laser1.readDenoise(); //laserCore1.readDenoise()でもOK
    laser2.readDenoise(); //laserCore2.readDenoise()でもOK

    //条件付き自己位置推定
    //もちろん割り込み内では使用できない
    float X = 1500;
    float Y = 250;
    float D = 0;
    while(1){
        lu1.scan(&X, &Y, &D);
        lu2.scan(&X, &Y, &D);
        printf("%d %d %d\n", int(X), int(Y), int(180*D/PI));
        wait_us(10000);
    }
}

/*

#include <mbed.h>
#include <VL53L0X.h>
#include "laserPos.hpp"


Timer timer;
Ticker ticker;
int counter = 0;

TOFLaserPos laser(0, 0, 0, 0, PB_5);

int main() {
    int v;
    printf("start\n");
    
    laser.init();

    timer.start();

    while(1){
        v= laser.read();
        printf("%d\n", v);
        //wait_us(100000);
    }

    printf("%d\n", int(chrono::duration<float>(timer.elapsed_time()).count()*1000000/1000));
}
*/

/*

I2C i2c(PB_3, PB_10);
Timer timer;
DigitalOut hsens_power(PB_5,1); // power on hSens terminals on CORE2
VL53L0X sensor(i2c, timer);


int main()
{
    printf("Start...\n");
    
    sensor.init();
    printf("Initialisation completed!\n");
    sensor.setTimeout(500);
    // Start continuous back-to-back mode (take readings as
    // fast as possible).  To use continuous timed mode
    // instead, provide a desired inter-measurement period in
    // ms (e.g. sensor.startContinuous(100)).
    sensor.startContinuous(0);
    while (1)
    {
        printf("%u\n", sensor.readRangeContinuousMillimeters());
        if (sensor.timeoutOccurred())
        {
            printf("TIMEOUT!\n");
        }
    }
    
}*/
#endif

