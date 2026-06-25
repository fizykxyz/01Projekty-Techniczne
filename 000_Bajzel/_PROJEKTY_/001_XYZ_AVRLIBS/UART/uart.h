/*
 * ATMEGA32L_LCD uart.h
 *
 *  Stworzono: 2014 23-09-2014
 *      Autor: D¹browski Tomasz
 *     Edycja: 09:39:43
 */

#ifndef UART_H_
#define UART_H_

//#define UART_BAUD 19200
//#define _UBRR  (F_CPU/16/UART_BAUD-1) // dla U2X = 0
//#define _UBRR  (F_CPU/8/UART_BAUD-1) // dla U2X = 1

#define NL "\r\n"

void UART_INIT( void );
void UART_SENDB( void );
void UART_SEND(uint8_t data);
void UART_SENDSTR(char *s);

#endif /* UART_H_ */
