#include "timer.h"
#include "../lcd1602/lcd1602.h"
#include <stdio.h>

static unsigned char T0RH = 0; //T0 重载值的高字节
static unsigned char T0RL = 0; //T0 重载值的低字节
static unsigned timer0_count = 0;

/* 配置并启动 T0，ms-T0 定时时间 */
void timer0_init(unsigned int ms)
{
    unsigned long tmp; // 临时变量
    timer0_count = ms;
    tmp = 11059200 / 12;              // 定时器计数频率
    tmp = (tmp * ms) / 1000;          // 计算所需的计数值
    tmp = 65536 - tmp;                // 计算定时器重载值
    tmp = tmp + 12;                   // 补偿中断响应延时造成的误差
    T0RH = (unsigned char)(tmp >> 8); // 定时器重载值拆分为高低字节
    T0RL = (unsigned char)tmp;
    TMOD &= 0xF0; // 清零 T0 的控制位
    TMOD |= 0x01; // 配置 T0 为模式 1
    TH0 = T0RH;   // 加载 T0 重载值
    TL0 = T0RL;
    ET0 = 1; // 使能 T0 中断
    TR0 = 1; // 启动 T0
}

void timer0_deinit()
{
    ET0 = 0;
    TR0 = 0;
    T0RH = 0;
    T0RL = 0;
    timer0_count = 0;
}

void Interrput_Timer0() interrupt 1
{
    static unsigned count = 0;

    TH0 = T0RH;   // 加载 T0 重载值
    TL0 = T0RL;
    count++;

    if (count >= timer0_count)
    {
        count = 0;
    }
}