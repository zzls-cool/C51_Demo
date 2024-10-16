#include <reg52.h>
#include <intrins.h>
#include <stdio.h>
#include <string.h>
#include "../lcd1602/lcd1602.h"
#include "../timer/timer.h"

void main()
{
	EA = 1;
	
    timer0_init(0xB800);
    InitLcd1602();
	
    while(1)
    {
    }
}


