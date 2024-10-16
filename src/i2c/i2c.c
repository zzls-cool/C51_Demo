#include "i2c.h"

#define I2CDelay() \
    {              \
        _nop_();   \
        _nop_();   \
        _nop_();   \
        _nop_();   \
    }
sbit I2C_SCL = P3 ^ 7;
sbit I2C_SDA = P3 ^ 6;

void I2CStart()
{
    I2C_SDA = 1;
    I2C_SCL = 1;
    I2CDelay();
    I2C_SDA = 0;
    I2CDelay();
    I2C_SCL = 0;
}

void I2CStop()
{
    I2C_SCL = 0;
    I2C_SDA = 0;
    I2CDelay();
    I2C_SCL = 1;
    I2CDelay();
    I2C_SDA = 1;
    I2CDelay();
}

bit I2CWrite(unsigned char dat)
{
    bit ack;
    unsigned char mask;
    for (mask = 0x80; mask != 0; mask >>= 1)
    {
        if ((mask & dat) == 0)
            I2C_SDA = 0;
        else
            I2C_SDA = 1;
        I2CDelay();
        I2C_SCL = 1;
        I2CDelay();
        I2C_SCL = 0;
    }
    I2C_SDA = 1;
    I2CDelay();
    I2C_SCL = 1;
    ack = I2C_SDA;
    I2CDelay();
    I2C_SCL = 0;

    return (~ack);
}

unsigned char I2CReadNAK()
{
    unsigned char mask;
    unsigned char dat;

    I2C_SDA = 1;                             // 首先确保主机释放 SDA
    for (mask = 0x80; mask != 0; mask >>= 1) // 从高位到低位依次进行
    {
        I2CDelay();
        I2C_SCL = 1;      // 拉高 SCL
        if (I2C_SDA == 0) // 读取 SDA 的值
            dat &= ~mask; // 为 0 时，dat 中对应位清零
        else
            dat |= mask; // 为 1 时，dat 中对应位置 1
        I2CDelay();
        I2C_SCL = 0; // 再拉低 SCL，以使从机发送出下一位
    }
    I2C_SDA = 1; // 8 位数据发送完后，拉高 SDA，发送非应答信号
    I2CDelay();
    I2C_SCL = 1; // 拉高 SCL
    I2CDelay();
    I2C_SCL = 0; // 再拉低 SCL 完成非应答位，并保持住总线
    return dat;
}

unsigned char I2CReadACK()
{
    unsigned char mask;
    unsigned char dat;

    I2C_SDA = 1;                             // 首先确保主机释放 SDA
    for (mask = 0x80; mask != 0; mask >>= 1) // 从高位到低位依次进行
    {
        I2CDelay();
        I2C_SCL = 1;      // 拉高 SCL
        if (I2C_SDA == 0) // 读取 SDA 的值
            dat &= ~mask; // 为 0 时，dat 中对应位清零
        else
            dat |= mask; // 为 1 时，dat 中对应位置 1
        I2CDelay();
        I2C_SCL = 0; // 再拉低 SCL，以使从机发送出下一位
    }
    I2C_SDA = 0; // 8 位数据发送完后，拉低 SDA，发送应答信号
    I2CDelay();
    I2C_SCL = 1; // 拉高 SCL
    I2CDelay();
    I2C_SCL = 0; // 再拉低 SCL 完成应答位，并保持住总线
    return dat;
}