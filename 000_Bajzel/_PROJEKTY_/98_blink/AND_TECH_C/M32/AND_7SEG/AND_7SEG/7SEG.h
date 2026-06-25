/*
 * _7SEG.h
 *
 * Created: 2014-02-08 17:39:14
 *  Author: Piotr
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#ifndef _7SEG_H
#define _7SEG_H

//definicje pinów steruj¹cych
#define SEGMENTS_DDR DDRA
#define SEGMENTS_PORT PORTA

#define SEG_DP PA0
#define SEG_A PA1
#define SEG_B PA2
#define SEG_C PA3
#define SEG_D PA4
#define SEG_E PA5
#define SEG_F PA6
#define SEG_G PA7

#define COM_DDR DDRC
#define COM_PORT PORTC
#define COM1 PC7
#define COM2 PC6
#define COM3 PC5
#define COM4 PC4

//zmienne dostêpne dla u¿ytkownika
volatile uint8_t LED_data[4];
volatile uint16_t LED_timer;


void LED_init(void);
void LED_dec(uint16_t number, uint8_t dot);
void LED_hex(uint16_t number, uint8_t dot);

#endif
