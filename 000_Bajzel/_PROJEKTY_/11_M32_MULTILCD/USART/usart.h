/*
 * 11_M32_MULTILCD usart.h
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 2015 1 gru 2015      
 *     Edycja: 09:41:46
 */

#ifndef USART_USART_H_
#define USART_USART_H_

//#define _UBRR  (F_CPU/16/UART_BAUD-1) // dla U2X = 0
//#define _UBRR  (F_CPU/8/UART_BAUD-1) // dla U2X = 1


// atrybuty znaku
#define RESET		0
#define BOLD 		1
#define DIM			2
#define UNDERLINE 	3
#define BLINK		4
#define REVERSE		7
#define HIDDEN		8

// kolory czcionki lub t³a
#define BLACK 		0
#define RED			1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define	WHITE		7

#define CLS "\x1b""[2J"
#define PENWHI "\x1b[37m"


#define NL "\r\n"




#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)


/* This variable is volatile so both main and RX interrupt can use it.
It could also be a uint8_t type */
extern volatile unsigned char value;

void USART_Init(void);
void USART_SendByte(uint8_t u8Data);
void USART_SendText(char *s);
uint8_t USART_ReceiveByte();

#endif /* USART_USART_H_ */
