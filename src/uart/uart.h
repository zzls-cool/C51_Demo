#ifndef __UART_H__
#define __UART_H__
#include <REG52.H>
#include <string.h>
void ConfigUART(unsigned int baud);
void SendUART(unsigned char* buf);
#endif // __UART_H__