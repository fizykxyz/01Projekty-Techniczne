/*
 * ATMEGA32L_LCD hd44780.h
 *
 *  Stworzono: 2014-09-17
 *      Autor: D¹browski Tomasz
 *     Edycja: 22:42:43
 */

#ifndef HD44780_H_
#define HD44780_H_

//pliki za³¹czone
#include <avr/io.h>

#include "../ADDS/pins.h"
#include "hd44780cfg.h"


//adresy rozkazów HD44780 - do przejrzenia i uporz¹dkowania

#define _HD44780_NOP asm volatile("nop\n\t""nop\n\t" "nop\n\t" "nop\n\t" ::); //raczej do usuniêcia

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


#define HD44780_COM 0 //komendy
#define HD44780_DAT 1 //dane

// deklaracje funkcji TODO: funkcje wyœwietlania string z EEPROM i FLASH, lub sparametryzowaæ HD44780_TEXT
void _HD44780_PULSE(void);  //podniesienie i opuszczenie linii E, nieu¿ywane mo¿na usun¹æ
void _HD44780_W4BIT(uint8_t NIBBLE); //wys³anie na magistrale danych 4bitów MSB (górnych)
void _HD44780_W8BIT(uint8_t DC, uint8_t OCTET); //wys³anie na szynê 8bitów (oktetu)
//void _HD44780_COM(uint8_t COMMAND);
//void _HD44780_DAT(uint8_t DATA);
void HD44780_NR(uint8_t NR,THD44780 *LCD );//wybranie aktywnego wyœw. przez podanie numeru z tablicy struktury LCD
void HD44780_INIT(void); //inicjacja wyœwietlacza
void HD44780_START(THD44780 *LCD,uint8_t SIZE); //inicjacja wyœwietlaczy ze struktury LCD
void HD44780_CUSTOMCHAR(uint8_t CHAR, uint8_t data[8]); //definicja w³asnego znaku na pozycji CHAR
void HD44780_CUSTOMCHARSET(const uint8_t data[64]); //definicja znaków w ca³ym DDRAM
void HD44780_CLR(void); //czyszczenie ekranu
void HD44780_HOME(void); //pozycja 0,0
void HD44780_CHAR(uint8_t CHAR); //wyœwietlenie znaku z RAM
void HD44780_TEXT(char * TEXT); //wyœwietlenie stringu z RAM
void HD44780_XY(int8_t PX,int8_t PY); //ustawienie kursora w pozycji PX,PY

//TODO dodaæ funkcje wykorzystuj¹ce rozkazy wyœwietlacza (kierunek tekstu, hw scroll, obs³uga kodowania tekstu)


#endif /* HD44780_H_ */
