#ifndef __I2C_H__
#define __I2C_H__
#include <REG52.H>
#include <INTRINS.H>
void I2CStart();
void I2CStop();
unsigned char I2CReadNAK();
unsigned char I2CReadACK();
bit I2CWrite(unsigned char dat);
#endif // __I2C_H__