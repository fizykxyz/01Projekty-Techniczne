/*
 * virtualOLED_UART.h
 *
 *      Author: artur85
 */

#ifndef VIRTUALOLED_UART_H_
#define VIRTUALOLED_UART_H_


#define UART_BAUD 9600
#define __UBRR ((F_CPU+UART_BAUD*8UL) / (16UL*UART_BAUD)-1)

void USART_Init(uint16_t baud );
void USART_Transmit( char data );

#endif /* VIRTUALOLED_UART_H_ */
