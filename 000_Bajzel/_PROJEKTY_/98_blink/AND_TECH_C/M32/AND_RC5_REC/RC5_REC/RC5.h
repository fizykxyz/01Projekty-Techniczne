/*
 * RC5.h
 *
 * Created: 2014-02-12 20:32:24
 *  Author: Piotr
 */ 
#ifndef _RC5_H
#define _RC5_H

#include <avr/io.h>
#include <avr/interrupt.h>

//taktowanie z preskalera timera
#define T1_PRESCALER 64

#define T_TOLERANCE     10
#define T_MIN_HALF_BIT 	32 - T_TOLERANCE
#define T_MAX_HALF_BIT 	32 + T_TOLERANCE
#define T_MAX_BIT       64 + T_TOLERANCE

#define TOLERANCE       ((F_CPU*(T_TOLERANCE   ))/T1_PRESCALER)/36000
#define MIN_HALF_BIT 	((F_CPU*(T_MIN_HALF_BIT))/T1_PRESCALER)/36000
#define MAX_HALF_BIT 	((F_CPU*(T_MAX_HALF_BIT))/T1_PRESCALER)/36000
#define MAX_BIT         ((F_CPU*(T_MAX_BIT     ))/T1_PRESCALER)/36000

void RC5_init(void);
uint8_t RC5_get(uint8_t* addr, uint8_t* comm, uint8_t* tog);

#endif