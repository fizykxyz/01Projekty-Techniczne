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
#define uart_init(f) UART_INIT
#define uart_putc(s) UART_SEND(s)
#define uart_puts_P(s) UART_SENDSTR(s)
#define uart_puts(s) UART_SENDSTR(s)


void UART_INIT( void );
void UART_SENDB( void );
void UART_SEND(char data);
void UART_SENDSTR(char *s);
void UART_SENDSTR_P(const char *s);
void uart_put_int( const int val );
void uart_put_longint( const long int val );

#endif /* UART_H_ */
