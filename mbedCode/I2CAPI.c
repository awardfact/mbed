#include "mbed.h"




I2C i2c(PB_9, PB_8);
DigitalOut aaa(PB_7);

// 마스터 주소
const int master = 0x90;
// 습도 온도 체크썸 슬레이브 주소 
const int Write = 0xB8;
const int Read = 0xB9;
const int tmpD = 0x02;
const int tmpR = 0x03;
const int checkS = 0x04;


int main() {

    //i2c.abort_transfer();
    char data[5];
    int tmp = 0;
    //i2c.start();
    //wait(2);
    while (1) {

        // 슬레이브 주소와 데이터 수신신호를 보낸다 tmp에는 수식 확인 여부 (0이면 수신)
        data[0] = 0x01;
        data[1] = 0x00;
        tmp = i2c.write(Write, data, 1);
        printf("tmp : %d\n", tmp);
        wait(0.5);


        data[0] = 0x00;
        tmp = i2c.write(Write, data, 1);
        tmp = i2c.read(Read, data, 5);
        printf("data1%d  data2  :   %d   data 3 : %d data4 : %d  data 5 : %d \n ", data[0], data[1], data[2], data[3], data[4]);

        //        printf("tmp : %d\n" , tmp);


        printf("tmp : %d\n", tmp);



        wait(0.5);
        //  printf("tmp : %d\n" , tmp);







        //  tmp = i2c.write(master , data , 1);
        //  printf("tmp : %d\n" , tmp);
        //  tmp = i2c.read(humR , data , 1);
        //  printf("tmp : %d\n" , tmp);
        // printf("data1 : %d  \n " , data[0] );
        //  wait(0.5);



          //cmd[0] = 0x00;
          //i2c.write(addr8bit , cmd , 1);
          //i2c.read(addr8bit, cmd , 2);
          //float tmp = (float((cmd[0]<<8)|cmd[1]) / 256.0);



    }
    i2c.stop();
}