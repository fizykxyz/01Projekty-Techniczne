/*
 * ATMEGA32L_LCD hd44780.h
 *
 *  Stworzono: 2014 17-09-2014
 *      Autor: D¹browski Tomasz
 *     Edycja: 22:42:43
 */

#ifndef HD44780_H_
#define HD44780_H_
//pliki za³¹czone
#include <avr/io.h>

#include "../ADDS/pins.h"
#include "hd44780cfg.h"


#define HD44780_COM 0 //komendy
#define HD44780_DAT 1 //dane


// deklaracje funkcji
void _HD44780_PULSE(void);
void _HD44780_4BIT(uint8_t NIBBLE);
void _HD44780_8BIT(uint8_t DC, uint8_t OCTET);
//void _HD44780_COM(uint8_t COMMAND);
//void _HD44780_DAT(uint8_t DATA);
void HD44780_NR(uint8_t NR,TSCREEN* EN );
void HD44780_INIT(void);
void HD44780_CUSTOMCHAR(uint8_t CHAR, const uint8_t data[8]);
void HD44780_CUSTOMCHARSET(const uint8_t data[64]);
void HD44780_CLR(void);
void HD44780_HOME(void);
void HD44780_CHAR(uint8_t CHAR);
void HD44780_TEXT(char * TEXT);
void HD44780_XY(int8_t PX,int8_t PY);

#if ( (HD44780_W == 20) && (HD44780_H == 4) )
//#define LCD_LINE1 0x00      // adres 1 znaku 1 wiersza
//#define LCD_LINE2 0x28      // adres 1 znaku 2 wiersza
//#define LCD_LINE3 0x14     // adres 1 znaku 3 wiersza
//#define LCD_LINE4 0x54     // adres 1 znaku 4 wiersza

#define LCD_LINE1 0x00      // adres 1 znaku 1 wiersza
#define LCD_LINE2 0x40      // adres 1 znaku 2 wiersza
#define LCD_LINE3 0x14     // adres 1 znaku 3 wiersza
#define LCD_LINE4 0x54     // adres 1 znaku 4 wiersza
#else
#define LCD_LINE1 0x00      // adres 1 znaku 1 wiersza
#define LCD_LINE2 0x40      // adres 1 znaku 2 wiersza
#define LCD_LINE3 0x10     // adres 1 znaku 3 wiersza             Tutaj
#define LCD_LINE4 0x50     // adres 1 znaku 4 wiersza             i tu
#endif


//adresy rozkazów HD44780



#define _HD44780_NOP asm volatile("nop\n\t""nop\n\t" "nop\n\t" "nop\n\t" ::);

#define HD44780_CLEAR					0x01

#define HD44780_CURSOR_HOME				0x02

#define HD44780_ENTRY_MODE				0x04
	#define HD44780_EM_SHIFT_CURSOR		0
	#define HD44780_EM_SHIFT_DISPLAY	1
	#define HD44780_EM_DECREMENT		0
	#define HD44780_EM_INCREMENT		2

#define HD44780_DISPLAY_ONOFF			0x08
	#define HD44780_DISPLAY_OFF			0
	#define HD44780_DISPLAY_ON			4
	#define HD44780_CURSOR_OFF			0
	#define HD44780_CURSOR_ON			2
	#define HD44780_CURSOR_NOBLINK		0
	#define HD44780_CURSOR_BLINK		1

#define HD44780_DISPLAY_CURSOR_SHIFT	0x10
	#define HD44780_SHIFT_CURSOR		0
	#define HD44780_SHIFT_DISPLAY		8
	#define HD44780_SHIFT_LEFT			0
	#define HD44780_SHIFT_RIGHT			4

#define HD44780_FUNCTION_SET			0x20
	#define HD44780_FONT5x7				0
	#define HD44780_FONT5x10			4
	#define HD44780_ONE_LINE			0
	#define HD44780_TWO_LINE			8
	#define HD44780_4_BIT				0
	#define HD44780_8_BIT				16

#define HD44780_CGRAM_SET				0x40

#define HD44780_DDRAM_SET				0x80





#endif /* HD44780_H_ */
