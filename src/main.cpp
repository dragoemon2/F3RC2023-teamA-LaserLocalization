#include <mbed.h>
#include "laser.hpp"
#include "Ticker.h"

#include <math.h>


int N=9;


Laser laser(A0);
Ticker ticker;
bool flag=0;

Timer timer;

void interrupt(){
    flag=true;
}


int main() {
    timer.start();
    
    ticker.attach(&interrupt, 50ms);

    while(true){
        if(flag){
            int v = laser.readDenoise();
            printf("%d\n", v);
        }
        wait_ns(1);
    }
}