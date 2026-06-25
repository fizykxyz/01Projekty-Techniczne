/*
 * ATMEGA32L_LCD hd44780.c
 *
 *  Stworzono: 2014 17-09-2014
 *      Autor: D¹browski Tomasz
 *     Edycja: 22:43:19
 */



#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "HD44780.h"

//-------------------------------------------------------------------------------------------------
//
// Funkcja opóŸniaj¹ca
//
//-------------------------------------------------------------------------------------------------

void _HD44780_PULSE(void)
{
   SET(PORT, HD44780_E);
  /*
   * Guarantee at least 500 ns of pulse width.  For high CPU
   * frequencies, a delay loop is used.  For lower frequencies, NOPs
   * are used, and at or below 1 MHz, the native pulse width will
   * already be 1 us or more so no additional delays are needed.
   */
#if F_CPU > 4000000UL
  _delay_us(5);
#else
  /*
   * When reading back, we need one additional NOP, as the value read
   * back from the input pin is sampled close to the beginning of a
   * CPU clock cycle, while the previous edge on the output pin is
   * generated towards the end of a CPU clock cycle.
   */

#  if F_CPU > 1000000UL
  __asm__ volatile("nop");
#    if F_CPU > 2000000UL
  __asm__ volatile("nop");
  __asm__ volatile("nop");
#    endif /* F_CPU > 2000000UL */
#  endif /* F_CPU > 1000000UL */
#endif

  CLR(PORT, HD44780_E);
 }

//-------------------------------------------------------------------------------------------------
//
// Funkcja wystawiaj¹ca pó³bajt na magistralê danych
//
//-------------------------------------------------------------------------------------------------
void _HD44780_4BIT(uint8_t NIBBLE)
{
if(NIBBLE & 0x01) SET(PORT,HD44780_D4); else CLR(PORT,HD44780_D4);
if(NIBBLE & 0x02) SET(PORT,HD44780_D5); else CLR(PORT,HD44780_D5);
if(NIBBLE & 0x04) SET(PORT,HD44780_D6); else CLR(PORT,HD44780_D6);
if(NIBBLE & 0x08) SET(PORT,HD44780_D7); else CLR(PORT,HD44780_D7);
//_HD44780_PULSE();
}

//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu bajtu na magistalê /DC>0 - dane , DC=0 komendy
//
//-------------------------------------------------------------------------------------------------
void _HD44780_8BIT(uint8_t DC,uint8_t OCTET)
{
if(DC) SET(PORT,HD44780_RS); else CLR(PORT,HD44780_RS);

SET(PORT,HD44780_E);
_HD44780_4BIT(OCTET >> 4);
CLR(PORT,HD44780_E);
SET(PORT,HD44780_E);
_HD44780_4BIT(OCTET);
CLR(PORT,HD44780_E);
_delay_ms(1);
}


/*
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu rozkazu do wyœwietlacza
//
//-------------------------------------------------------------------------------------------------
void _HD44780_COM(uint8_t COMMAND)
{
CLR(PORT,HD44780_RS);
_HD44780_8BIT(COMMAND);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu danych do pamiêci wyœwietlacza
//
//-------------------------------------------------------------------------------------------------
void _HD44780_DAT(uint8_t DATA)
{
SET(PORT,HD44780_RS);
_HD44780_8BIT(DATA);
}

*/

//*********************Funkcje wysokiego poziomu***************************************************

//-------------------------------------------------------------------------------------------------
//
// Procedura inicjalizacji kontrolera HD44780.
//
//-------------------------------------------------------------------------------------------------
void HD44780_INIT(void)
{
uint8_t i;
SET(DDR,HD44780_D4);
SET(DDR,HD44780_D5);
SET(DDR,HD44780_D6);
SET(DDR,HD44780_D7);
SET(DDR,HD44780_RS);
SET(DDR,HD44780_E);

_delay_ms(20);
CLR(PORT,HD44780_RS);
CLR(PORT,HD44780_E);

for(i = 0; i < 3; i++) // trzykrotne powtórzenie bloku instrukcji
  {
  SET(PORT,HD44780_E); //  E = 1
  _HD44780_4BIT(0x03); // tryb 8-bitowy
  CLR(PORT,HD44780_E); // E = 0
  _delay_ms(5); // czekaj 5ms
  }

SET(PORT,HD44780_E); //  E = 1
_HD44780_4BIT(0x02); // tryb 4-bitowy
CLR(PORT,HD44780_E); // E = 0

_delay_ms(1);


_HD44780_8BIT(HD44780_COM,HD44780_FUNCTION_SET | HD44780_FONT5x7 | HD44780_TWO_LINE | HD44780_4_BIT); // interfejs 4-bity, 2-linie, znak 5x7
_HD44780_8BIT(HD44780_COM,HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_OFF); // wy³¹czenie wyswietlacza
_HD44780_8BIT(HD44780_COM,HD44780_ENTRY_MODE | HD44780_EM_SHIFT_CURSOR | HD44780_EM_INCREMENT);// inkrementaja adresu i przesuwanie kursora
_HD44780_8BIT(HD44780_COM,HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_CURSOR_NOBLINK); // w³¹cz LCD, bez kursora i mrugania
_HD44780_8BIT(HD44780_COM,HD44780_CLEAR); // czyszczenie zawartosæi pamieci DDRAM

}

void HD44780_CUSTOMCHAR(uint8_t CHAR, const uint8_t data[8])
{

	uint8_t i;
	 /* adres CGRAM 0 */
	for (i = 0; i< 8 ; i++){
			_HD44780_8BIT(HD44780_COM,HD44780_CGRAM_SET+CHAR*8+ i);
			_HD44780_8BIT(HD44780_DAT,pgm_read_byte(data + i));
		}
		_HD44780_8BIT(HD44780_COM,HD44780_DDRAM_SET);

}

void HD44780_CUSTOMCHARSET(const uint8_t data[64])
{
	uint8_t i;
	 /* adres CGRAM 0 */
	for (i = 0; i < 64; i++){
		_HD44780_8BIT(HD44780_COM,HD44780_CGRAM_SET+i);
		_HD44780_8BIT(HD44780_DAT,pgm_read_byte(data + i));
	}
	_HD44780_8BIT(HD44780_COM,HD44780_DDRAM_SET);
}

void HD44780_CLR(void){
		_HD44780_8BIT(HD44780_COM,HD44780_CLEAR);
	}

void HD44780_HOME(void){
		//_HD44780_8BIT(HD44780_COM,HD44780_CURSOR_HOME);
		HD44780_XY(0,0);

	}

//-------------------------------------------------------------------------------------------------
//
// Funkcja wyœwietlenia napisu na wyswietlaczu.
//
//-------------------------------------------------------------------------------------------------

void HD44780_CHAR(uint8_t CHAR)
{
#ifdef _HD44780_CHAR_REWRITE
	if (CHAR & 0x80)
		_HD44780_CHAR_REWRITE(CHAR);
#endif
	_HD44780_8BIT(HD44780_DAT,CHAR);
}



void HD44780_TEXT(char * TEXT)
{
while(*TEXT)  HD44780_CHAR(*TEXT++);
}

void HD44780_XY(int8_t PX,int8_t PY){
	uint8_t line=0;
	switch(PY)
	  {
	case 0: line = LCD_LINE1; break;
	#if (HD44780_H>1)
	    case 1: line=LCD_LINE2; break;
	#endif
	#if (HD44780_H>2)
	    case 2: line=LCD_LINE3; break;
	#endif
	#if (HD44780_H>3)
	    case 3: line=LCD_LINE4; break;
	#endif
	  }
	  _HD44780_8BIT(HD44780_COM,0x80+line+PX);
	}

