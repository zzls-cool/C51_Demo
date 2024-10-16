#include <reg52.h>
#include <intrins.h>
#include <stdio.h>

#include "../lcd1602/lcd1602.h"

void main()
{
    InitLcd1602();
    LcdShowStr(0, 0, "Hello world!", 16);
    while(1);
}
