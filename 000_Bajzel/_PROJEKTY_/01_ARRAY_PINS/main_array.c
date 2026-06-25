/*
 * 01_ARRAY_PINS main.c
 *
 *	    Autor: D¹browski Tomasz
 *  Stworzono: 2016-08-01
 *     Edycja: 08:28:46
 */



#include <avr/io.h>
#include <util/delay.h>

#include "ADDS/pins.h"

//************************** PROGRAM W£AŒCIWY - WYKORZYSTANIE ******************

//definicja przez tablicê
uint8_t diody[] = { G2O(PA,0), G2O(PA,1), G2O(PA,2),  G2O(PA,3), G2O(PA,4), G2O(PA,5),  G2O(PA,6), G2O(PA,7),
					G2O(PB,0), G2O(PB,1), G2O(PB,2),  G2O(PB,3), G2O(PB,4), G2O(PB,5),  G2O(PB,6), G2O(PB,7),
					G2O(PC,0), G2O(PC,1), G2O(PC,2),  G2O(PC,3), G2O(PC,4), G2O(PC,5),  G2O(PC,6), G2O(PC,7),
					G2O(PD,0), G2O(PD,1), G2O(PD,2),  G2O(PD,3), G2O(PD,4), G2O(PD,5),  G2O(PD,6), G2O(PD,7)};


uint8_t nrLed;
#define T_LEDON 300
#define T_LEDOFF 100
//zwyk³a definicja pinu
#define blinkLed G2O(PD,7)


int main(void)  //rozpoczynamy
{
OUTPUT(blinkLed);

	for(nrLed=0;nrLed<sizeof(diody);nrLed++) //ustawiamy nó¿nki AVR na wyjœcia
	{
		OUTPUT(diody[nrLed]);


	}

	while(1) //pêtla nieskoñczona
	{

		for(nrLed=0;nrLed<sizeof(diody);nrLed++)  //zapalamy i gasimy kolejne diody od w tablicy
			{
				HIGH(diody[nrLed]);
				_delay_ms(T_LEDON);
				LOW(diody[nrLed]);
				_delay_ms(T_LEDOFF);
				 //TOGGLE(blinkLed); //migaj¹ca podstawa litery L
				PUT(blinkLed,nrLed%2);
			}


	}
}


