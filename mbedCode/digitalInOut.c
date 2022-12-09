#include "mbed.h"

DigitalOut led1(PC_0);
DigitalOut led2(PC_1);
DigitalOut led3(PC_2);
DigitalIn btn1(PC_13);
int main() {


    int mode = 0;

    if (led2.is_connected()) {
        printf("led2 connect\n");
    }



    if (led3.is_connected()) {
        printf("led3 connect\n");
    }


    if (btn1.is_connected()) {
        printf("btn1 connect\n");
    }

    btn1.mode(PullUp);


    while (1) {

        if (btn1 == 1) {
            mode = !mode;
        }

        if (mode) {
            led1 = 1;
            led2 = 1;
            led3 = 1;
            wait(0.5);
        }
        else {

            led1 = 0;
            led2 = 1;
            led3 = 1;
            wait(0.5);
            led1 = 1;
            led2 = 0;
            led3 = 1;
            wait(0.5);
            led1 = 1;
            led2 = 1;
            led3 = 0;
            wait(0.5);


        }


        printf("led1 : %d , led2 : %d  , led3 : %d  ,   btn1 : %d\n", (int)led1, (int)led2, (int)led3, (int)btn1);

    }



}