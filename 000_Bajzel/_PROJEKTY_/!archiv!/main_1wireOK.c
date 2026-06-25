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
#include <string.h>
#include <avr/pgmspace.h>

#include "ADDS/tools.h"
#include "HD44780/hd44780.h"
#include "ADDS/memcheck.h"
#include "ONEWIRE/onewire.h"


//************************** PROGRAM W£AŒCIWY - WYKORZYSTANIE ******************




THD44780 wysw[]=
{
//			{16,2,G2O(PA,7)},
//			{16,2,G2O(PA,6)},
//			{16,4,G2O(PA,5)},
//			{20,4,G2O(PA,4)},
			{16,2,G2O(PA,7)}
};

uint8_t nrLed;
uint16_t memo;
char buf[10];

#define blinkLED G2O(PD,7)
#define T_LEDON 300
#define T_LEDOFF 100

//zwyk³a definicja pinu
//#define blinkLed G2O(PD, 7)

#define debug

const PROGMEM uint8_t HD44780_CHARSPLW[64]   =
{
	0x0E, 0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x02, /* \0 = ¥ */
	0x02, 0x0E, 0x15, 0x10, 0x10, 0x11, 0x0E, 0x00, /* \1 = Æ */
	0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F, 0x02, /* \2 = Ê */
	0x10, 0x12, 0x14, 0x18, 0x10, 0x10, 0x1F, 0x00, /* \3 = £ */
	0x02, 0x15, 0x19, 0x15, 0x13, 0x11, 0x11, 0x00, /* \4 = Ñ */
	0x02, 0x0E, 0x15, 0x11, 0x11, 0x11, 0x0E, 0x00, /* \5 = Ó */
	0x04, 0x0E, 0x10, 0x0E, 0x01, 0x11, 0x0E, 0x00, /* \6 = Œ */
	0x04, 0x1F, 0x09, 0x02, 0x04, 0x08, 0x1F, 0x00  /* \7 =¯ */
/*	0x1F, 0x01, 0x02, 0x1F, 0x08, 0x10, 0x1F, 0x00    \7 = ¯ */
/*	0x04, 0x1F, 0x09, 0x02, 0x04, 0x08, 0x1F, 0x00,         */
};




uint8_t znak[]  =
		  {
				  0b00100,
				  0b01010,
				  0b01010,
				  0b01110,
				  0b01110,
				  0b11111,
				  0b01110,
				  0b00100
		  };


uint8_t ids_count=0;
uint8_t ID[8];

//uint8_t  a, b, tt;

int8_t readtemp(uint8_t *ID)
 { uint16_t temp;
   OW_RESETPULSE();
   // Convert
   OW_SELECT(ID);
   _OW_W8BIT(0x44);
   OW_RESETPULSE();
   // Read Scratch memory area
uint16_t tconv=750;
while (tconv--)
{
//	znak[tconv%8]=(znak[tconv%8]>>1)|(znak[tconv%8]<<4);
//	HD44780_CUSTOMCHAR(0,znak);
	_delay_ms(1);
}
   //_delay_ms(750);
   OW_SELECT(ID);
   _OW_W8BIT(0xBE);
   temp = _OW_R8BIT();
   temp = (_OW_R8BIT()<<8)|temp;
//   b = _OW_R8BIT();
//   t = b;
//   t <<= 8;
//   t = t|a;
   temp = temp*0.0625;
	return(temp);
 }




uint8_t lcd,x,y;

void movecursor()
{
	y++;
	if (y>=wysw[lcd].LIN)
		{
		y=0;
		if ((31+x)<(wysw[lcd].KOL))
		{
		x+=16;
		}
		else
		{
		lcd++;
		if (lcd>=SIZE(wysw)) lcd=0;
		HD44780_NR(lcd,wysw);
			HD44780_CUSTOMCHAR(0,znak);
		x=0;
		}

		}
HD44780_XY(x,y);
}

int main(void)  //rozpoczynamy
{
	OUTPUT(blinkLED);
	LOW(blinkLED);
	HD44780_START(wysw,SIZE(wysw));
	HD44780_NR(0,wysw);
	HD44780_CUSTOMCHARSET(HD44780_CHARSPLW);
	HD44780_XY(2,0);
	HD44780_TEXT("uC-");
    HD44780_TEXT(strupr(STRING(MCU))); //HD44780_CHAR('0'+strlen(STRING(MCU)));
    HD44780_XY(2,1);
    HD44780_TEXT("@ ");
	HD44780_TEXT(STRING(F_CPU));
	HD44780_TEXT(" Hz");
	_delay_ms(200);
	HD44780_XY(0,2);
	HD44780_TEXT("RAM:");
	itoa(RAMEND-RAMSTART,buf,10);
	HD44780_TEXT(buf);
	HD44780_TEXT(" bytes");
	HD44780_XY(0,3);
	HD44780_CHAR('0'+SIZE(wysw));
	HD44780_TEXT(strupr(" wy\x06wietlaczy"));
//	itoa(get_mem_unused(),buf,10);
//	itoa(RAMEND-RAMSTART,buf,10);
//	HD44780_TEXT(buf);
	_delay_ms(6000);
	HD44780_CLR();
	HD44780_CHAR('v');
	HD44780_TEXT(STRING(__DATE__));
	HD44780_XY(0,1);
	HD44780_TEXT(STRING(__TIME__));
	_delay_ms(2000);
	HD44780_NR(0,wysw);

	while(1) //pêtla nieskoñczona
	{

		ids_count=0;
		OW_RESETSEARCH();
			 while(OW_SEARCH(ID) == OW_OK)
			  {
				 int8_t temp=readtemp(ID);
				 itoa(temp,buf,10);
				 ids_count++;
			   for (uint8_t i=0;i<8;i++)
			   		 {
				   	   	 HD44780_CHAR(NIBBLE2HEX(ID[i]>>4));
			   			 HD44780_CHAR(NIBBLE2HEX(ID[i]&0x0F));
			   		 }

			   movecursor();
			   HD44780_CHAR(0);
			   HD44780_TEXT("Temp");
			   HD44780_CHAR('0'+ids_count);
			   HD44780_TEXT(": ");
			   HD44780_TEXT(buf);
			   movecursor();
			   _delay_ms(800);
			   TOGGLE(blinkLED);
			  }









//		for(nrLed=0;nrLed<SIZE(wysw);nrLed++)
//			{
//			HD44780_NR(nrLed,wysw);
//			HD44780_XY(3,0);
//			HD44780_TEXT(("WY\x06WIETLACZ NR "));
//			HD44780_CHAR('0'+nrLed);
//			HD44780_XY(0,1);
//			HD44780_CHAR('0'+nrLed);
//			HD44780_XY(0,2);
//			HD44780_CHAR('0'+nrLed);
//			HD44780_XY(0,3);
//			HD44780_CHAR('0'+ids_count);
//			HD44780_HOME();
//			int8_t temp=readtemp();
//			//uint8_t temp= OW_RESETPULSE();
//			itoa(temp,buf,10);
//			HD44780_TEXT(buf);
////			HD44780_CHAR('0'+OW_RESETPULSE());
//			_delay_ms(500);
//			}


	}
}


