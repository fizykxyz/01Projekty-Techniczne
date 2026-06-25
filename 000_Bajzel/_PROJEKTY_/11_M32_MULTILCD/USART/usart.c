/*
 * 11_M32_MULTILCD usart.c
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 2015 1 gru 2015      
 *     Edycja: 09:43:25
 */



#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>

#include "../ADDS/tools.h"
#include "usart.h"

volatile unsigned char value;

/* Interrupt Service Routine for Receive Complete
NOTE: vector name changes with different AVRs see AVRStudio -
Help - AVR-Libc reference - Library Reference - <avr/interrupt.h>: Interrupts
for vector names other than USART_RXC_vect for ATmega32 */

ISR(USART_RXC_vect){

   value = UDR;             //read UART register into value
   PORTB = ~value;          // output inverted value on LEDs (0=on)
}

void USART_Init(void){
   // Set baud rate
   UBRRL = BAUD_PRESCALE;// Load lower 8-bits into the low byte of the UBRR register
   UBRRH = (BAUD_PRESCALE >> 8);
	 /* Load upper 8-bits into the high byte of the UBRR register
    Default frame format is 8 data bits, no parity, 1 stop bit
  to change use UCSRC, see AVR datasheet*/

  // Enable receiver and transmitter and receive complete interrupt
  UCSRB = ((1<<TXEN)|(1<<RXEN) | (1<<RXCIE));
}
void USART_SendByte(uint8_t u8Data){

  // Wait until last byte has been transmitted
  while((UCSRA &(1<<UDRE)) == 0);

  // Transmit data
  UDR = u8Data;
}

void USART_SendText(char *s)		// wysy³a ³añcuch z pamiêci RAM na UART
{

	 char c;
	  while ((c = *s++)) USART_SendByte(c);			// dopóki nie napotkasz 0 wysy³aj znak
	}




// not being used but here for completeness
      // Wait until a byte has been received and return received data
uint8_t USART_ReceiveByte(){
  while((UCSRA &(1<<RXC)) == 0);
  return UDR;
}


