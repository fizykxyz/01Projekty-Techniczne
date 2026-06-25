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
#include "pins.h"

volatile uint8_t *ucports[] = { &PORTA, &PORTB, &PORTC, &PORTD }; //tablica adresów portów IO


/**
typedef struct {
    volatile uint8_t * port;
    uint8_t pin;
} port_pin_t;

port_pin_t pinarray[] = {
    { &PORTB, (1 << 3) },
    { &PORTD, (1 << 2) },
    { &PORTA, (1 << 5) },
    { &PORTC, (1 << 7) }
};

static inline void set_pin(uint8_t n) {*pinarray[n].port |= pinarray[n].pin;}

static inline void clr_pin(uint8_t n) {*pinarray[n].port &= ~pinarray[n].pin;}
***/
