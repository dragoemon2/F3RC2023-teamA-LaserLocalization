#include <stdio.h>
#include <mbed.h>
#include "laser.hpp"

using namespace std;

#define CALIBRATION_N (101) //calibrateで何回計測するか
#define READ_DENOISE_N (15) //readDenoiseで何回計測するか

#define TEIKOU 155

#if TEIKOU == 150
    #define DEFAULT_A (14334.0f)
    #define DEFAULT_B (-2605.8f)
#else
    #define DEFAULT_A (13043.0f)
    #define DEFAULT_B (-2374.0f)
#endif

//ふつうに読む(8usほどかかる)
int Laser::read()
{
    float dis = analogin.read();
    int dismm = convert(dis);
    return dismm;
}

//ノイズを除去して読む(72usほどかかる)
int Laser::readDenoise(){
    //READ_DENOISE_N=9回計測して中間値を出力
    int values[READ_DENOISE_N];
    for(int i=0;i<READ_DENOISE_N;i++){
        values[i] = read();
    }
    sort(values, values+READ_DENOISE_N); //並べ替え
    return values[READ_DENOISE_N/2];
}

//0~1の数値から距離[mm]に変換
int Laser::convert(float input){
    return int(A * input + B);
}

//キャリブレーションのために距離計測
void Laser::calibrate(int distance){
    float values[CALIBRATION_N];
    for(int i=0;i<CALIBRATION_N;i++){
        values[i] = analogin.read();
    }

    sort(values, values+CALIBRATION_N, greater<int>());

    distances.push_back(distance);
    outputs.push_back(values[CALIBRATION_N/2]);
    calibrationNum++;

    calibrationFit(true);
}

//最小二乗法でキャリブレーションによる回帰直線を更新
int Laser::calibrationFit(bool save){
    if(calibrationNum < 2){
        return 1;
    }

    float sumX = 0;
    float sumY = 0;
    float sumXsquared = 0;
    float sumXY = 0;

    for(int i=0;i<calibrationNum;i++){
        sumX += outputs[i];
        sumY += distances[i];
        sumXsquared += distances[i]*distances[i];
        sumXY += distances[i]*outputs[i];
    }

    //0除算回避
    if(sumXsquared - sumX*sumX == 0){
        return 1;
    }

    A = (calibrationNum*sumXY - sumX*sumY)/(calibrationNum*sumXsquared - sumX*sumX); //回帰直線の傾き
    B = (sumY - A*sumX)/calibrationNum; //回帰直線の定数項

    if(save){
        //saveCalibrationToFlash();
    }

    return 0;
}

//回帰直線のA,Bの値を読み込む(書いてない)
void Laser::loadCalibrationFromFlash() {
    if(false){
        //できたらここにキャリブレーション値をflashから読み込む処理を書きたい
    }else{
        A = DEFAULT_A;
        B = DEFAULT_B;
    }
}

//回帰直線のA,Bの値をflashに書き込む(書いてない)
void Laser::saveCalibrationToFlash() {
    //できたらここにキャリブレーション値をflashに書き込む処理を書きたい
}

//初期化
Laser::Laser(PinName pin) : pin(pin), analogin(pin)
{
    calibrationNum = 0;

    A = DEFAULT_A;
    B = DEFAULT_B;
    //loadCalibrationFromFlash();
}