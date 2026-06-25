/*
 * 00_XYZ_AVRLIBS main7219.c
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 2016-09-26   
 *     Edycja: 11:52:06
 */



#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>


#include "MAX7219/max7219.h"


int main(void)  //rozpoczynamy
{

MAX7219_INIT();
MAX7219_W16BIT(0xF011);
	while(1) //pêtla nieskoñczona
	{
		_delay_ms(1000);
		TOGGLE(MAX7219_LOAD);

	}
}
