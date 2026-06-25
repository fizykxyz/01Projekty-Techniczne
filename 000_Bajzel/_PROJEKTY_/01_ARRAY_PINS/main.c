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
uint8_t hej;

//************************** PROGRAM W£AŒCIWY - WYKORZYSTANIE ******************

//definicja przez tablicê
uint8_t diody[] = { P(A,1), G2O(pA,1), G2O(pA,2),  G2O(pA,3), G2O(pA,4), G2O(pA,5),  G2O(pA,6), G2O(pA,7),
					G2O(pB,0), G2O(pB,1), G2O(pB,2),  G2O(pB,3), G2O(pB,4), G2O(pB,5),  G2O(pB,6), G2O(pB,7),
					G2O(pC,0), G2O(pC,1), G2O(pC,2),  G2O(pC,3), G2O(pC,4), G2O(pC,5),  G2O(pC,6), G2O(pC,7),
					G2O(pD,0), G2O(pD,1), G2O(pD,2),  G2O(pD,3), G2O(pD,4), G2O(pD,5),  G2O(pD,6), G2O(pD,7)};


uint8_t nrLed;
#define T_LEDON 200
#define T_LEDOFF 100
//zwyk³a definicja pinu


int main(void)  //rozpoczynamy
{

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
//				PUT(blinkLed,nrLed%2);
			}


	}
}


