/*
 * ATMEGA32L_LCD tools.c
 *
 *  Stworzono: 2014 17-09-2014
 *      Autor: D¹browski Tomasz
 *     Edycja: 22:43:19
 */


#include <util/delay.h>
#include <avr/pgmspace.h>
#include "tools.h"



#if PINARRAY
volatile uint8_t *portyIO[] = { &PORTA, &PORTB, &PORTC,&PORTD };
#endif

