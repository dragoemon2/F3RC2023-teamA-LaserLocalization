#pragma once
#include "laserPos.hpp"


class _LaserUse{
    public:
        //今の推定位置を考慮してレーザーによる自己位置推定が使える条件
        float minX; //X座標の下限
        float maxX; //X座標の上限
        float minY; //Y座標の下限
        float maxY; //Y座標の上限
        float errorD; //方角の不確かさ

        //使う壁の情報
        int wallDirection; //壁の方角
        int wallR; //原点から壁までの距離

        int laserDirection;//レーザーの方向

        _LaserUse(float minX, float maxX, float minY, float maxY, float errorD, int wallDirection, int wallR, int laserDirection);

        bool check(float X, float Y, float D);
        bool active;

        void activate();
        void inactivate();
        virtual void scan(float* X, float* Y, float* D, bool denoise=false, bool always=false);
};

class LaserUse: public _LaserUse
{
    public:
        //レーザー
        LaserPos laser;
        LaserUse(float minX, float maxX, float minY, float maxY, float errorD, int wallDirection, int wallR, LaserPos laser);

        void scan(float* X, float* Y, float* D, bool denoise=false, bool always=false);
};


class LaserPairUse: public _LaserUse
{
    public:
        //レーザー
        LaserPos laser1;
        LaserPos laser2;

        LaserPairUse(float minX, float maxX, float minY, float maxY, float errorD, int wallDirection, int wallR, LaserPos laser1, LaserPos laser2);

        void scan(float* X, float* Y, float* D, bool denoise=false, bool always=false);
};

