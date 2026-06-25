/*
 * 01_ARRAY_PINS main.c
 *
 *	    Autor: D¹browski Tomasz
 *  Stworzono: 2016-08-01
 *     Edycja: 08:28:46
 */



#include <avr/io.h>
#include <util/delay.h>
//#include "ADDS/tools.h"
#include "ADDS/pins.h"
#include "HD44780/hd44780.h"
//************************** PROGRAM W£AŒCIWY - WYKORZYSTANIE ******************


uint8_t nrLed;
#define T_LEDON 300
#define T_LEDOFF 100
//zwyk³a definicja pinu
#define blinkLed G2O(PD, 7)

TSCREEN wysw[]=
{
			{16,2,G2O(PA,7)},
			{16,4,G2O(PA,6)},
			{20,2,G2O(PA,5)}
};



int main(void)  //rozpoczynamy
{

	OUTPUT(blinkLed);


	for(nrLed=0;nrLed<3;nrLed++) //ustawiamy nó¿nki AVR na wyjœcia
	{
	//	HD44780_NR(nrLed,wysw);
		HD44780_INIT();
	}

	while(1) //pêtla nieskoñczona
	{

		for(nrLed=0;nrLed<3;nrLed++)  //zapalamy i gasimy kolejne diody od w tablicy
			{
//			HD44780_NR(nrLed,wysw);
//			HD44780_TEXT("WYŒWIETLACZ NR ");
//			HD44780_CHAR('0'+nrLed);
			_delay_ms(500);
			}


	}
}


