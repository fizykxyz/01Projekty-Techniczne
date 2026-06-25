/*
 * 01_ARRAY_PINS main.c
 *
 *	    Autor: D¹browski Tomasz
 *  Stworzono: 2016-08-01
 *     Edycja: 08:28:46
 */



#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "ADDS/tools.h"
#include <string.h>
//#include "ADDS/pins.h"
#include "HD44780/hd44780.h"
#include "ADDS/memcheck.h"
#include "ONEWIRE/onewire.h"
#include "1wire.c"


//************************** PROGRAM W£AŒCIWY - WYKORZYSTANIE ******************




THD44780 wysw[]=
{
			{16,2,G2O(PA,7)},
			{16,2,G2O(PA,6)},
			{16,4,G2O(PA,5)},
			{20,4,G2O(PA,4)}
};

uint8_t nrLed;
uint16_t memo;
char buf[10];

#define T_LEDON 300
#define T_LEDOFF 100

//zwyk³a definicja pinu
//#define blinkLed G2O(PD, 7)

#define debug

uint8_t ids[8][8];
uint8_t ids_count=0;
uint8_t ID[8];

uint8_t  a, b, tt;

int8_t readtemp(void)
 { uint16_t t;
   OW_INIT();
   // Convert
   _OW_W8BIT(0xCC, 0);
   _OW_W8BIT(0x44, 0);
   OW_INIT();
   // Read Scratch memory area
   _OW_W8BIT(0xCC, 0);
   _OW_W8BIT(0xBE, 0);
   a = _OW_R8BIT();
   b = _OW_R8BIT();
   t = b;
   t <<= 8;
   t = t|a;
   tt = t*0.0625;
	return(tt);
 }





uint8_t znak[]=
		  {
				  0b10000,
				  0b10010,
				  0b10100,
				  0b11000,
				  0b10000,
				  0b10000,
				  0b11111,
				  0b00000
		  };


int main(void)  //rozpoczynamy
{

//	OUTPUT(blinkLed);



		HD44780_INIT();


	_delay_ms(20);
	HD44780_XY(2,1);
	HD44780_TEXT(strupr(STRING(MCU)));
	HD44780_CHAR('0'+strlen(STRING(MCU)));
	_delay_ms(2000);

	HD44780_CHAR('0'+SIZE(wysw));
	itoa(get_mem_unused(),buf,10);
	itoa(RAMEND-RAMSTART,buf,10);
	HD44780_TEXT(buf);
	HD44780_CUSTOMCHAR(0,znak);
	_delay_ms(2000);


	HD44780_CHAR(0);
	 onewire_findinit();
	 HD44780_XY(3,2);
	 HD44780_CHAR(onewire_find(ID));
	 for (uint8_t i=0;i<8;i++)
	 {
		 HD44780_CHAR(NIBBLE2HEX(ID[i]>>4));
		 HD44780_CHAR(NIBBLE2HEX(ID[i]&0x0F));

	 }
	 HD44780_XY(3,3);
		 HD44780_CHAR(onewire_find(ID));
		 for (uint8_t i=0;i<8;i++)
		 {
			 HD44780_CHAR(NIBBLE2HEX(ID[i]>>4));
			 HD44780_CHAR(NIBBLE2HEX(ID[i]&0x0F));

		 }
	 _delay_ms(2000);


	 //	 while(ids_count < 8 && onewire_find(ids[id_count]) == ONEWIRE_OK)
//	  {
//	   ids_count++;
//	  }

	while(1) //pêtla nieskoñczona
	{

		for(nrLed=0;nrLed<SIZE(wysw);nrLed++)  //zapalamy i gasimy kolejne diody od w tablicy
			{
			HD44780_NR(nrLed,wysw);
			HD44780_XY(0,0);
			HD44780_TEXT("WYSWIETLACZ NR ");
			HD44780_CHAR('0'+nrLed);
			HD44780_XY(0,1);
			HD44780_CHAR('0'+nrLed);
			HD44780_XY(0,2);
			HD44780_CHAR('0'+nrLed);
			HD44780_XY(0,3);
			HD44780_CHAR('0'+nrLed);
			HD44780_HOME();
			int8_t temp=readtemp();
			itoa(temp,buf,10);
			HD44780_TEXT(buf);
			_delay_ms(500);
			}


	}
}


