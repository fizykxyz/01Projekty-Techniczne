/*
 * _001_AVR_LED_BLINK main.c
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 2020-01-09   
 *     Edycja: 20:18:31
 */




#include <avr/iom64.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <inttypes.h>
#include <stdio.h>

//#include "../ADDS/pins.h"
//#include "../HD44780/hd44780.h"
//#include "SPI.h"
int x=0;

int main(void)  //rozpoczynamy
{
DDRB=DDRC=DDRD=0xFF;



	while(1) //pêtla nieskoñczona
	{
		x++;
		x%=3;
		if (!x)
		{
			PORTB=PORTC=PORTD^=0x55;
		}

     PORTB=PORTC=PORTD^=0xAA;

     _delay_ms(200);
	}
}
