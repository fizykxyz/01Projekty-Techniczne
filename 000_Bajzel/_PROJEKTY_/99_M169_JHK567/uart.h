/*================================================================
==================================================================
=	Definicje dla pliku obs³ugi interfejsu USART1
=	CPU = ATMega169
=	Copywright - LiveLights
==================================================================	
================================================================*/

#ifndef _AVR_UART_H_
#define _AVR_UART_H_


#include "ATmega169_LCD_test.h"


#define _UCSRA 	0xC0

#define UDRE_ 	DAJ_BIT(_UCSRA).bit5					// flaga pustego bufora transmisji USART
#define RXC_ 	DAJ_BIT(_UCSRA).bit7					// flaga zakonczonego odbioru USART


void uart_init(unsigned long);
void uart_char(unsigned char);
void uart_tab(unsigned char*,unsigned int);
unsigned char uart_get(void);
unsigned char uart_get_tab(unsigned char*,unsigned int);

//void uart_tab_pgm(PGM_P, unsigned char);


#endif

