#ifndef __LCD1602_H__
#define __LCD1602_H__
#include <REG52.H>
#define LCD1602_DB P1

sbit LCD1602_RS = P2^7;
sbit LCD1602_RW = P2^6;
sbit LCD1602_E = P2^5;

void InitLcd1602();
void LcdShowStr(unsigned char x, unsigned char y,
                unsigned char *str, unsigned char len);

#endif // __LCD1602_H__