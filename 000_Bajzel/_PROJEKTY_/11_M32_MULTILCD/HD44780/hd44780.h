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


#define HD44780_CODEPAGE_ASCII		1
#define HD44780_CODEPAGE_ISO8859_2	2
#define HD44780_CODEPAGE_CP1250		3
#define HD44780_CODEPAGE_UTF8		4
#define HD44780_CODEPAGE_BEZPL		5

#define HD44780_COM 0 //komendy
#define HD44780_DAT 1 //dane


// deklaracje funkcji
void _HD44780_SETEN(uint8_t ONOFF);
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

#define HD44780_CURSOR_HOME					0x02

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





#if !defined(HD44780_CODEPAGE) || (HD44780_CODEPAGE == HD44780_CODEPAGE_ASCII)
#elif (HD44780_CODEPAGE == HD44780_CODEPAGE_ISO8859_2)
#	define _HD44780_CHAR_REWRITE(c) \
	{ \
		if (c == 0xB1 || c == 0xA1) \
			c = 0; \
		else if (c == 0xE6 || c == 0xC6) \
			c = 1; \
		else if (c == 0xEA || c == 0xCA) \
			c = 2; \
		else if (c == 0xB3 || c == 0xA3) \
			c = 3; \
		else if (c == 0xF1 || c == 0xD1) \
			c = 4; \
		else if (c == 0xF3 || c == 0xD3) \
			c = 5; \
		else if (c == 0xB6 || c == 0xA6) \
			c = 6; \
		else if (c == 0xBF || c == 0xBC || c == 0xAF || c == 0xAC) \
			c = 7; \
	}
#elif HD44780_CODEPAGE == HD44780_CODEPAGE_CP1250
#	define _HD44780_CHAR_REWRITE(c) \
	{ \
		if (c == 0xA5 || c == 0xB9) \
			c = 8; \
		else if (c == 0xE6 || c == 0xC6) \
			c = 9; \
		else if (c == 0xEA || c == 0xCA) \
			c = 10; \
		else if (c == 0xB3 || c == 0xA3) \
			c = 11; \
		else if (c == 0xF1 || c == 0xD1) \
			c = 12; \
		else if (c == 0xF3 || c == 0xD3) \
			c = 13; \
		else if (c == 0x8C || c == 0x9C) \
			c = 14; \
		else if (c == 0xAF || c == 0xBF || c == 0x8F || c == 0x9F) \
			c = 15; \
	}
#elif HD44780_CODEPAGE == HD44780_CODEPAGE_BEZPL
#	define _HD44780_CHAR_REWRITE(c) \
	{ \
		if (c == 0xB9) \
			c = 0x61; \
		else if (c == 0xE6) \
			c = 0x63; \
		else if (c == 0xEA) \
			c = 0x65; \
		else if (c == 0xB3) \
			c = 0x6C; \
		else if (c == 0xF1) \
			c = 0x6E; \
		else if (c == 0xF3) \
			c = 0x6F; \
		else if (c == 0x9C) \
			c = 0x73; \
			else if (c == 0x9F || c == 0xBF) \
			c = 0x7A; \
					else if (c == 0xA5) \
						c = 0x41; \
					else if (c == 0xC6) \
						c = 0x43; \
					else if (c == 0xCA) \
						c = 0x45; \
					else if (c == 0xA3) \
						c = 0x4C; \
					else if (c == 0xD1) \
						c = 0x4E; \
					else if (c == 0xD3) \
						c = 0x4F; \
					else if (c == 0x8C) \
						c = 0x53; \
					else if (c == 0x8F || c == 0xAF) \
						c = 0x5A; \
	}
#elif HD44780_CODEPAGE == HD44780_CODEPAGE_UTF8
uint8_t _hd44780_char_rewrite_unicodeBuff = 0;
#	define _HD44780_CHAR_REWRITE(c) \
	{ \
		if (c >= 0xC3 && c <= 0xC5) \
		{ \
			_hd44780_char_rewrite_unicodeBuff = c; \
			return; \
		} \
	\
		if (_hd44780_char_rewrite_unicodeBuff == 0xC4 && c >= 0x84 && c <= 0x87) \
			c = (c / 2) - 0x42; /* 0, 1 */ \
		else if (c >= 0x81 && c <= 0x84) \
			c = ((c + 1) / 2) - 0x3E; /* 3, 4 */ \
		else if (c == 0x93 || c == 0xB3) \
			c = 5; \
		else if (c >= 0x98 && c <= 0x9B) \
			c = ((c / 2) - 0x4C) * 4 + 2; /* 2, 6 */ \
		else if (c >= 0xB9 && c <= 0xBC) \
			c = 7; \
	}
#else
#	error "To kodowanie nie jest obs³ugiwane w tym zestawie znaków"
#endif


#endif /* HD44780_H_ */
