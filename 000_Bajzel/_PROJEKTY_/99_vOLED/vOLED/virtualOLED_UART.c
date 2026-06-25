/*
* virtualOLED_UART.c
*
*      Author: artur85
*/
#include <avr/io.h>
#include "virtualOLED_UART.h"


void USART_Init(uint16_t baud )
{
/* Set baud rate*/
UBRRH = (uint8_t)(baud>>8);
UBRRL = (uint8_t)baud;
/* Enable transmitter*/
UCSRB = (1<<TXEN)|(1<<RXEN);
/* Set frame format: 8data, 1stop bit*/
UCSRC = (1<<URSEL)|(3<<UCSZ0);
}

void USART_Transmit( char data )
{
/* Wait for empty transmit buffer*/
while( !( UCSRA & (1<<UDRE)));

/* Put data into buffer, sends the data*/
UDR = data;
}
