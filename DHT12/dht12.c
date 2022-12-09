#include "mbed.h"

// scl(직렬쿨럭 입력) 단일버스여서 단일
DigitalOut scl(PB_8);
// sda (시리얼 데이터 양방향) 양방향이여서 INOUT으로 선언해준다
DigitalInOut sda(PB_9);
#define delay wait_us(40);

// 습도 온도 체크썸 슬레이브 주소
const int Write = 0xB8;
const int Read = 0xB9;

// i2c 시작하는 함수 둘다 high상태에서 sda가 0으로 바뀌면 시작
void i2cStart()
{
    sda.output();
    sda.write(0);
    wait_us(40);
    scl.write(0);
}

// 메시지를  보내는 함수
int i2cWrite(char msg)
{
    int i = 0;
    unsigned char tmp;
    sda.output();
    unsigned char tmp2;

    // 7비트 주소 + 1비트 읽기 쓰기 여부를 보낸다
    for (i = 7; i >= 0; i--)
    {

        tmp = msg;

        tmp = tmp >> i;
        tmp2 = 0x01;
        tmp = tmp2 & tmp;
        sda.write(tmp);
        wait_us(40);
        // 쿨럭을 high로
        scl.write(1);
        // 1비트씩 데이터를 보낸다
        // 50 마이크로세컨드 쉬는걸로 되어있어서 50마이크로 세컨드 쉼
        wait_us(40);
        // 1bit보냈으니까 다시 상태 변경
        scl.write(0);
        wait_us(40);
    }

    sda.input();
    wait_us(40);
    scl.write(1);

    tmp = sda.read();
    //
    wait_us(40);

    sda.output();
    scl.write(0);
    sda.write(0);

    return tmp;
}

// 슬레이브에게 메시지를 받는 함수
int i2cRead(int byte, char data[])
{

    int i = 0;
    int j = 0;
    char tmp;

    // 바이트 수만큼 읽음
    for (i = 0; i < byte; i++)
    {
        data[i] = 0x00;
        // 메시지를 받음

        for (j = 0; j < 8; j++)
        {
            sda.input();
            wait_us(40);
            // 처음 이외에는 비트를 왼쪽으로 한칸 옮김
            if (j != 0)
            {
                data[i] = data[i] << 1;
            }

            scl.write(1);

            // 쿨럭을 high로
            //  슬레이브에게 데이터를 읽음
            tmp = sda.read();
            // 데이터를 넣음
            data[i] += tmp;
            // 50 마이크로세컨드 쉬는걸로 되어있어서 50마이크로 세컨드 쉼
            wait_us(40);

            scl.write(0);
        }
        sda.output();
        wait_us(40);
        sda.write(0);
        // 한 바이트가 끝나면 슬레이브에 읽었다고 메시지를 보냄
        scl.write(1);

        wait_us(40);
        scl.write(0);
    }

    // 원상태로 복구하고 종료
    sda.output();
    wait_us(40);
    sda.write(0);
    wait_us(40);
    return tmp;
}

// i2c 종료
void i2cStop()
{

    scl.write(1);
    wait_us(40);
    sda.output();
    wait_us(40);
    sda.write(1);
    sda.input();
}

int main()
{

    // 습도 온도를 받기 위한 데이터 선언
    char data[5];
    int tmp;

    printf("start");

    // 처음에는 둘다 HIGH상태
    scl.write(1);
    sda.output();
    sda.write(1);
    wait_us(40);

    // 전원을 켠 후 2초정도 대기하라고 해서 대기
    wait(2);

    while (1)
    {
        i2cStart();
        tmp = 100;
        // 슬레이브에 읽는다고 메시지를 보낸다
        tmp = i2cWrite(Write);

        wait_us(40);
        tmp = 100;
        tmp = i2cWrite(0);

        wait_us(40);
        sda.output();
        sda.write(1);
        wait_us(40);
        scl.write(1);
        wait_us(40);
        i2cStart();
        wait_us(40);
        tmp = 100;
        tmp = i2cWrite(Read);

        wait_us(40);
        tmp = i2cRead(5, data);

        if (data[0] + data[1] + data[2] + data[3] != data[4])
        {
            printf("error!\n");

        }
        else {


            // 온도 실수의 첫자리가 1이면 영하인거라서 128을 빼주고 영하를 붙인다
            if (data[3] > 128)
            {
                printf("hum : %d.%d   tmp2 : -%d.%d  checkSum : %d \n", data[0], data[1], data[2], data[3] - 128, data[4]);
            }
            else
            {
                printf("hum : %d.%d   tmp2 : %d.%d   checkSum : %d  \n", data[0], data[1], data[2], data[3], data[4]);
            }

        }

        i2cStop();
        wait(0.5);
    }
}