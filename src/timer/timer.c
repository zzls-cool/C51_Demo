#include "timer.h"

static unsigned timer0_value = 0x00;

void timer0_init(unsigned value)
{
	timer0_value = value;
    TMOD = 0x01;
    TH0 = timer0_value >> 8;
    TL0 = timer0_value & 0xFF;
    ET0 = 1;
    TR0 = 1;
}

void timer0_deinit()
{
	ET0 = 0;
    TR0 = 0;
	timer0_value = 0;
}

void Interrput_Timer0() interrupt 1
{
    static unsigned count = 0;

    TH0 = timer0_value >> 8;
    TL0 = timer0_value & 0xFF;

    count++;

    if(count >= 50)
    {
        count = 0;
    }
}