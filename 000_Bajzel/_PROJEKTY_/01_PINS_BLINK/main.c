/*
 * 01_PINS_BLINK main.c
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 2015 2 lip 2015      
 *     Edycja: 06:33:41
 */



#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
//#include "HD44780.h"
//#include "SPI.h"
#define P_ON(prt,pin) (PORT##prt) |= (1<<(P##prt##pin))
#define P_OFF(prt,pin) (PORT##prt) &= ~(1<<(P##prt##pin))
#define P_SW(prt,pin) (PORT##prt) ^= (1<<(P##prt##pin))
#define P_ST(prt,pin) (PORT##prt) & (1<<(P##prt##pin))

//#define F_CPU 8000000 //ustawienie oscylatora na 8MHz
//#include "ADDS/tools.h"

uint8_t DODDR ={DDRA,DDRB};

int main(void) { //rozpoczynamy

DDRA=0xFF;
DDRB=0xFF;
DDRC=0xFF;

	#define LED D,1
 OUTPUT(LED);
	HIGH(LED);

	while(1){ //pêtla nieskoñczona
	


_delay_ms(300);
TOGGLE(LED);
	}
}
