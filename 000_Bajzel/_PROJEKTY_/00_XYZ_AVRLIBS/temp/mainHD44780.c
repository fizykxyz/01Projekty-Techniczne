/*
 * 01_ARRAY_PINS main.c
 *
 *	    Autor: D¹browski Tomasz
 *  Stworzono: 2016-08-01
 *     Edycja: 08:28:46
 */



#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "ADDS/tools.h"
//#include "ADDS/pins.h"
#include "HD44780/hd44780.h"
#include "ADDS/memcheck.h"
//************************** PROGRAM W£AŒCIWY - WYKORZYSTANIE ******************




THD44780 wysw[]=
{
			{16,2,G2O(PA,7)},
			{16,2,G2O(PA,6)},
			{16,4,G2O(PA,5)},
			{20,4,G2O(PA,4)}
};

uint8_t nrLed;
uint16_t memo;
char buf[10];

#define T_LEDON 300
#define T_LEDOFF 100
//zwyk³a definicja pinu
//#define blinkLed G2O(PD, 7)


int main(void)  //rozpoczynamy
{

//	OUTPUT(blinkLed);


	for(nrLed=0;nrLed<SIZE(wysw);nrLed++) //ustawiamy nó¿nki AVR na wyjœcia
	{
		HD44780_NR(nrLed,wysw);
		HD44780_INIT();
	}
	HD44780_CLR();
	_delay_ms(20);

	HD44780_CHAR('0'+SIZE(wysw));
	itoa(get_mem_unused(),buf,10);
	itoa(RAMEND-RAMSTART,buf,10);
	HD44780_TEXT(buf);
	_delay_ms(2000);
	while(1) //pêtla nieskoñczona
	{

		for(nrLed=0;nrLed<SIZE(wysw);nrLed++)  //zapalamy i gasimy kolejne diody od w tablicy
			{
			HD44780_NR(nrLed,wysw);
			HD44780_XY(0,0);
			HD44780_TEXT("WYSWIETLACZ NR ");
			HD44780_CHAR('0'+nrLed);
			HD44780_XY(0,1);
			HD44780_CHAR('0'+nrLed);
			HD44780_XY(0,2);
			HD44780_CHAR('0'+nrLed);
			HD44780_XY(0,3);
			HD44780_CHAR('0'+nrLed);
			_delay_ms(500);
			}


	}
}


