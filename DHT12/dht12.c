#include "mbed.h"

// scl(������ �Է�) ���Ϲ������� ����
DigitalOut scl(PB_8);
// sda (�ø��� ������ �����) ������̿��� INOUT���� �������ش�
DigitalInOut sda(PB_9);
#define delay wait_us(40);

// ���� �µ� üũ�� �����̺� �ּ�
const int Write = 0xB8;
const int Read = 0xB9;

// i2c �����ϴ� �Լ� �Ѵ� high���¿��� sda�� 0���� �ٲ�� ����
void i2cStart()
{
    sda.output();
    sda.write(0);
    wait_us(40);
    scl.write(0);
}

// �޽�����  ������ �Լ�
int i2cWrite(char msg)
{
    int i = 0;
    unsigned char tmp;
    sda.output();
    unsigned char tmp2;

    // 7��Ʈ �ּ� + 1��Ʈ �б� ���� ���θ� ������
    for (i = 7; i >= 0; i--)
    {

        tmp = msg;

        tmp = tmp >> i;
        tmp2 = 0x01;
        tmp = tmp2 & tmp;
        sda.write(tmp);
        wait_us(40);
        // ���� high��
        scl.write(1);
        // 1��Ʈ�� �����͸� ������
        // 50 ����ũ�μ����� ���°ɷ� �Ǿ��־ 50����ũ�� ������ ��
        wait_us(40);
        // 1bit�������ϱ� �ٽ� ���� ����
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

// �����̺꿡�� �޽����� �޴� �Լ�
int i2cRead(int byte, char data[])
{

    int i = 0;
    int j = 0;
    char tmp;

    // ����Ʈ ����ŭ ����
    for (i = 0; i < byte; i++)
    {
        data[i] = 0x00;
        // �޽����� ����

        for (j = 0; j < 8; j++)
        {
            sda.input();
            wait_us(40);
            // ó�� �̿ܿ��� ��Ʈ�� �������� ��ĭ �ű�
            if (j != 0)
            {
                data[i] = data[i] << 1;
            }

            scl.write(1);

            // ���� high��
            //  �����̺꿡�� �����͸� ����
            tmp = sda.read();
            // �����͸� ����
            data[i] += tmp;
            // 50 ����ũ�μ����� ���°ɷ� �Ǿ��־ 50����ũ�� ������ ��
            wait_us(40);

            scl.write(0);
        }
        sda.output();
        wait_us(40);
        sda.write(0);
        // �� ����Ʈ�� ������ �����̺꿡 �о��ٰ� �޽����� ����
        scl.write(1);

        wait_us(40);
        scl.write(0);
    }

    // �����·� �����ϰ� ����
    sda.output();
    wait_us(40);
    sda.write(0);
    wait_us(40);
    return tmp;
}

// i2c ����
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

    // ���� �µ��� �ޱ� ���� ������ ����
    char data[5];
    int tmp;

    printf("start");

    // ó������ �Ѵ� HIGH����
    scl.write(1);
    sda.output();
    sda.write(1);
    wait_us(40);

    // ������ �� �� 2������ ����϶�� �ؼ� ���
    wait(2);

    while (1)
    {
        i2cStart();
        tmp = 100;
        // �����̺꿡 �д´ٰ� �޽����� ������
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


            // �µ� �Ǽ��� ù�ڸ��� 1�̸� �����ΰŶ� 128�� ���ְ� ���ϸ� ���δ�
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