/*
 * 002_XYZ_AVRLIBS tools.c
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 2020-09-22   
 *     Edycja: 00:16:38
 */

#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <avr/sleep.h>
#include <util/delay.h>
//#include <inttypes.h>
//#include <stdio.h>

#include "tools.h"
//#include "../HD44780/hd44780.h"
//#include "SPI.h"


//#define delayms(MS) {while((MS)--) {_delay_ms(1);}}}


void delayms(uint16_t ms)
{
	while(ms--)
	{
		_delay_ms(1);
	}
}


/*
void delayms( uint16_t ms )
{
for(; ms>0; ms--){
uint16_t i;
for(i=(1000/(16000000UL/F_CPU)); i>0; i--){
__asm__ __volatile__ ("nop");
__asm__ __volatile__ ("nop");
}
}
}

*/
