#include "lcd1602.h"

void LcdWaitReady()
{
    unsigned char sta;

    LCD1602_DB = 0xFF;
    LCD1602_RS = 0;
    LCD1602_RW = 1;

    do {
        LCD1602_E = 1;
        sta = LCD1602_DB;
        LCD1602_E = 0;
    } while(sta & 0x80);
}

void LcdWriteCmd(unsigned char cmd)
{
    LcdWaitReady();
    LCD1602_RS = 0;
    LCD1602_RW = 0;
    LCD1602_DB = cmd;
    LCD1602_E = 1;
    LCD1602_E = 0;
}

void LcdWriteDat(unsigned char dat)
{
    LcdWaitReady();
    LCD1602_RS = 1;
    LCD1602_RW = 0;
    LCD1602_DB = dat;
    LCD1602_E = 1;
    LCD1602_E = 0;
}

void LcdSetCursor(unsigned char x, unsigned char y)
{
    unsigned char addr;

    if(y == 0)
        addr = 0x00 + x;
    else
        addr = 0x40 + x;

    LcdWriteCmd(addr | 0x80);
}

void LcdShowStr(unsigned char x, unsigned char y,
                unsigned char *str, unsigned char len)
{
    LcdSetCursor(x, y);
    while (len--)
    {
        LcdWriteDat(*str++);
    }
}

void InitLcd1602()
{
    LcdWriteCmd(0x38);  //16*2 显示，5*7 点阵，8 位数据接口
    LcdWriteCmd(0x0C);  //显示器开，光标关闭
    LcdWriteCmd(0x06);  //文字不动，地址自动+1
    LcdWriteCmd(0x01);  //清屏
}