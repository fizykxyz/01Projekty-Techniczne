/*
 * RC5_send.h
 *
 *  Created on: 27-05-2013
 *      Author: Piotr Rzeszut
 *  
 * Description: Biblioteka do obs³ugi nadawania w standardzie RC5
 */
#ifndef RC5_send_h
#define RC5_send_h

#include <avr/io.h>
#include <avr/interrupt.h>

#define RC5_PORT PORTD
#define RC5_DDR DDRD
#define RC5_PN PD5


volatile uint8_t RC5_bitcount;
volatile uint16_t RC5_data;

void RC5_send_init(void);

void RC5_send(uint8_t toggle, uint8_t address, uint8_t command);

#endif //RC5_send_h
