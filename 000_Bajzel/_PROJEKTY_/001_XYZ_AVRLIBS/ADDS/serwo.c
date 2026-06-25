/*
 * ATMEGA32L_LCD serwo.c
 *
 *  Stworzono: 2014 16-10-2014
 *      Autor: D¹browski Tomasz
 *     Edycja: 13:42:18
 */



#include <avr/io.h>
//#define F_CPU 16000000
#include <util/delay.h>
#include "serwo.h"

void SERWO1_INIT(void)
{

ICR1=20000;      // wartoœæ TOP
OCR1B=1500;    // wype³nienie dla pozycji œrodkowej

TCCR1A=0x22;    //preskaler przez 8 i wyjœcie OCR1B
TCCR1B=0x12;   // pwm poprawnej fazy jako TOP ICR1

DDRD|=(1<<4);  //wyjœcie OCR1B
}

void SERWO1(uint8_t ALFA)
{
if ((ALFA>=0) && (ALFA<=SERWO1_ANGLE)){
	OCR1B=SERWO1_ALFA(ALFA);
}
}
