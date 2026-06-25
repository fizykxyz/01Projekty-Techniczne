/*
 * 13_M32_LED4x12 main.c
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 22 kwi 2016      
 *     Edycja: 06:10:50
 */



#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>


int main(void) { //rozpoczynamy

DDRA=123;

	while(1){ //pêtla nieskoñczona
	

PORTA^=123;
_delayms(1000);
	}
}
