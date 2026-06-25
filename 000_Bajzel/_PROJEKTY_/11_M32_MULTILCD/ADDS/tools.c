/*                  e-gadget.header
 * tools.c
 *
 *  Created on: 2015-12-06
 *    Modyfied: 2015-12-06 18:08:13
 *      Author: D¹browski Tomasz
 *
 * Project name: "11_M32_MULTILCD"
 *
 *          MCU: ATmega32
 *        F_CPU: 8 000 000 Hz
 *
 *    Flash: 4 250 bytes   [ 13,0 % ]
 *      RAM:  170 bytes   [ 8,3 % ]
 *   EEPROM:  0 bytes   [ 0,0 % ]
 *
 */


#include <util/delay.h>
#include <avr/pgmspace.h>
#include "tools.h"



#if PINARRAY
volatile uint8_t *portyIO[] = { &PORTA, &PORTB, &PORTC,&PORTD };
#endif






