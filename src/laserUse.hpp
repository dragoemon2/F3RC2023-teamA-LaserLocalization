#pragma once
#include "laserPos.hpp"


class _LaserUse{
    public:
        int wallDirection;
        int wallR;
        int laserDirection;//レーザーの方向

        bool active;

        void activate(int wallDirection, int wallR);
        void inactivate();
        bool check(float X, float Y, float D);
        //virtual void scan(float* X, float* Y, float* D, bool denoise=false);
        _LaserUse(int wallDirection, int wallR, int laserDirection);
};

class LaserUse: public _LaserUse
{
    public:
        //レーザー
        LaserPos laser;
        LaserUse(int wallDirection, int wallR, LaserPos laser);

        void scan(float* X, float* Y, float* D, bool denoise=false, bool always=false);
};


class LaserPairUse: public _LaserUse
{
    public:
        //レーザー
        LaserPos laser1;
        LaserPos laser2;

        LaserPairUse(int wallDirection, int wallR, LaserPos laser1, LaserPos laser2);

        void scan(float* X, float* Y, float* D, bool denoise=false, bool always=false);
};

