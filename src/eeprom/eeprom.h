#ifndef __EEPROM_H__
#define __EEPROM_H__
#include <REG52.H>

unsigned char E2ReadByte(unsigned char addr);
void E2WriteByte(unsigned char addr, unsigned char dat);
void E2Read(unsigned char *buf, unsigned char addr, unsigned char len);
void E2Write(unsigned char *buf, unsigned char addr, unsigned char len);
void E2WritePage(unsigned char *buf, unsigned char addr, unsigned char len);
#endif // __EEPROM_H__