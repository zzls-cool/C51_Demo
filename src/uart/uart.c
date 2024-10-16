#include "uart.h"

static unsigned char rev_buf[16] = { 0 };

void ConfigUART(unsigned int baud)
{
    SCON = 0x50;
    TMOD &= 0x0F;
    TMOD |= 0x20;
    TH1 = 256 - (11059200/12/32)/baud;
    TL1 = TH1;
    ET1 = 0;
    ES = 1;
    TR1 = 1;
}

void SendUART(unsigned char* buf)
{
    ES = 0;
    while(*buf != 0)
    {
        SBUF = *buf;
        while(TI == 0);
        TI = 0;
        buf++;
    }
    ES = 1;
}

static unsigned char* RecvUART()
{
    static unsigned char buf[16] = { 0 };
    static unsigned char count = 0;

    if(count < 15)
    {
        buf[count] = SBUF;
        count++;
    }
    else
    {
        count = 0;
        memset(buf, 0, sizeof(buf));
    }

    return buf;
}

void InterruptUART() interrupt 4
{
    if(RI)
    {
        RecvUART();
        RI = 0;
    }
}