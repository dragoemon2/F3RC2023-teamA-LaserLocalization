# F3RC2023-teamA-LaserLocalization

レーザーDT35，VL53L0Xのクラスと，レーザーによる自己位置推定のクラスです．

クラス間の参照関係が複雑なので図に示します．(すみません)



## 使い方

```cpp
//値は適当です．

#include <mbed.h>
#include "parameters.hpp"

/* parameters.hppの記述
#define PI (3.141592653589793)

#define EAST  (0)
#define NORTH (1)
#define WEST  (2)
#define SOUTH (3)
*/

//----レーザーとロボット上でのその位置の指定----
#include "laserPos.hpp"

//DT35
LaserPos laser1(
    100, //DT35レーザー位置のx座標
    -150, //DT35レーザー位置のy座標
    SOUTH, //ロボットが東を向いた時の向き
    PC_1 //ピン
);

LaserPos laser2(
    -100, //DT35レーザー位置のx座標
    -150, //DT35レーザー位置のy座標
    SOUTH, //ロボットが東を向いた時の向き
    PC_0 //ピン
);

//VL53L0X
TOFLaserPos laser3(
    -150, //VL53L0Xレーザー位置のx座標
    -100, //VL53L0Xレーザー位置のy座標
    WEST, //ロボットが東を向いた時の向き
    0, //番号unsigned int レーザーによって異なるものを指定．
    PA_13, //xshutピン
    PB_3, //SDAピン
    PB_10 //SCLピン
);

TOFLaserPos laser4(
    -150, //VL53L0Xレーザー位置のx座標
    100, //VL53L0Xレーザー位置のy座標
    WEST, //ロボットが東を向いた時の向き
    1, //番号unsigned int レーザーによって異なるものを指定．
    PA_14, //xshutピン
    PB_3, //SDAピン
    PB_10 //SCLピン
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
    laser3.init();
    laser4.init();

    //レーザーの値を読む int[mm]　読めないと負の値となる．
    //割り込み内では使用できない
    laser1.read();
    laser2.read();
    laser3.read();
    laser4.read();

    //DT35の値をノイズ除去して読む int[mm]
    //割り込み内では使用できない
    laser1.readDenoise();
    laser2.readDenoise();

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

```


## 今後

1. VL53L0Xの複数台使用ができるか確認
2. マルチスレッドでの使用を試す．(これが上手くいけばいろいろと楽)
3. レーザーの設置角度の誤差を考慮できるようにする．(計算式は既にそうなっているが，レーザーの向きをintで指定してしまったため修正が面倒くさい)
4. この自己位置推定のプログラムを足回りのプログラムに統合する．(割り込み内で使えないのがとても面倒)
5. レーザー自己位置推定の使用条件LaserUse, LaserPairUseの値の入力
