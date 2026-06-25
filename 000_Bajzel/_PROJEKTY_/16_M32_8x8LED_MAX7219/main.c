//ledmatrix7219d88 output example

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "max7219/max7219.h"
//#define FDATE (char const[]){ __DATE__[7], __DATE__[8], ..., ' ', ... , '\0' } //Znaki pl

/*
 * main function
 */
int main(void)
{
	//init ledmatrix
	max7219_init();
    max7219_digit0(1,2);
    max7219_decode(1,3);
while (1)
{

}

}
