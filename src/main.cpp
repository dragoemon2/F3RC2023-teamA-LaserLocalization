#include <mbed.h>
#include "parameters.hpp"
#include "tofLaser.hpp"

TOFLaser laser1(1, PA_11, D14, D15);
TOFLaser laser2(2, PB_12, D14, D15);
//TOFLaser laser4(4, PA_12, PB_9, PB_8);


int main(){
    laser1.init();
    laser2.init();
    //laser4.init();

    while(1){
        //printf("%d %d %d\n", laser1.read(), laser2.read(), laser4.read());
        printf("%d %d\n", laser1.read(), laser2.read());
        //printf("%d\n", laser1.read());
        wait_us(100000);
    }
}
