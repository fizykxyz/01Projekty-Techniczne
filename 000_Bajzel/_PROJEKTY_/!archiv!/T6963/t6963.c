/*
 * 00_XYZ_AVRLIBS t6963.c
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 2016-09-06   
 *     Edycja: 13:05:33
 */



#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>


#include "t6963.h"
//#include "../FONTS/font_std_5x8.h"

//Wybór ekranu nr i struktura go przechowuj¹ca
void T6963_NR(uint8_t NR, TT6963 *LCD )
{
T6963_CE.WIDTH=LCD[NR].WIDTH;
T6963_CE.HEIGHT=LCD[NR].HEIGHT;
T6963_CE.CEN=LCD[NR].CEN;
};





void T6963_DELAY(void)
{
volatile uint8_t i;
for(i = 0; i < (F_CPU/1000000); i++)
  {
  asm("nop");
  }
}

void _T6963_BUSOUTPUT()
{
	OUTPUT(T6963_D0);OUTPUT(T6963_D1);
	OUTPUT(T6963_D2);OUTPUT(T6963_D3);
	OUTPUT(T6963_D4);OUTPUT(T6963_D5);
	OUTPUT(T6963_D6);OUTPUT(T6963_D7);
}

void _T6963_BUSINPUT()
{
	INPUT(T6963_D0);INPUT(T6963_D1);
	INPUT(T6963_D2);INPUT(T6963_D3);
	INPUT(T6963_D4);INPUT(T6963_D5);
	INPUT(T6963_D6);INPUT(T6963_D7);
}


void _T6963_INITALIZEINTERFACE(void)
{
//linie danych jako wyjœcie
	_T6963_BUSOUTPUT();

//linie kontrolne jako wyjœcie
OUTPUT(T6963_WR);
OUTPUT(T6963_RD);
OUTPUT(T6963_CE.CEN);
OUTPUT(T6963_CD);
OUTPUT(T6963_RST);
OUTPUT(T6963_FS);

HIGH(T6963_WR);
HIGH(T6963_RD);
HIGH(T6963_CE.CEN);
HIGH(T6963_CD);
HIGH(T6963_RST);
HIGH(T6963_FS);
}

void T6963_INIT()
{
	_T6963_INITALIZEINTERFACE();
	LOW(T6963_RST);
	_delay_ms(1);
	HIGH(T6963_RST);

	#if (GLCD_FONT_WIDTH == 8)
		LOW(T6963_FS);
	#endif

	_T6963_W8BIT(T6963_DAT, GLCD_GRAPHIC_HOME & 0xFF);
	_T6963_W8BIT(T6963_DAT, GLCD_GRAPHIC_HOME >> 8);
	_T6963_W8BIT(T6963_COM, T6963_SET_GRAPHIC_HOME_ADDRESS);

	_T6963_W8BIT(T6963_DAT, GLCD_GRAPHIC_AREA);
	_T6963_W8BIT(T6963_DAT, 0x00);
	_T6963_W8BIT(T6963_COM, T6963_SET_GRAPHIC_AREA);

	_T6963_W8BIT(T6963_DAT, GLCD_TEXT_HOME);
	_T6963_W8BIT(T6963_DAT, GLCD_TEXT_HOME >> 8);
	_T6963_W8BIT(T6963_COM, T6963_SET_TEXT_HOME_ADDRESS);

	_T6963_W8BIT(T6963_DAT, GLCD_TEXT_AREA);
	_T6963_W8BIT(T6963_DAT, 0x00);
	_T6963_W8BIT(T6963_COM, T6963_SET_TEXT_AREA);

	_T6963_W8BIT(T6963_DAT, GLCD_OFFSET_REGISTER);
	_T6963_W8BIT(T6963_DAT, 0x00);
	_T6963_W8BIT(T6963_COM, T6963_SET_OFFSET_REGISTER);

	_T6963_W8BIT(T6963_COM, T6963_DISPLAY_MODE  | T6963_GRAPHIC_DISPLAY_ON   | T6963_TEXT_DISPLAY_ON /*| T6963_CURSOR_DISPLAY_ON*/);
	_T6963_W8BIT(T6963_COM, T6963_MODE_SET | 0);

}


uint8_t _T6963_READ()
{

	return (READ(T6963_D7)<<7)|(READ(T6963_D6)<<6)|\
		   (READ(T6963_D5)<<5)|(READ(T6963_D4)<<4)|\
		   (READ(T6963_D3)<<3)|(READ(T6963_D2)<<2)|\
		   (READ(T6963_D1)<<1)|(READ(T6963_D0));

}

uint8_t T6963_CHCECKSTATUS(void)
{
uint8_t status;

_T6963_BUSINPUT();

LOW(T6963_RD);
LOW(T6963_CE.CEN);
T6963_DELAY();

status=_T6963_READ();

_T6963_BUSOUTPUT();

HIGH(T6963_RD);
HIGH(T6963_CE.CEN);
return status;
}






void _T6963_W8BIT(uint8_t DC, uint8_t OCTET)
{
	while(!(T6963_CHCECKSTATUS()&0x03));


	if(DC)
		{
		LOW(T6963_CD);
		}
	LOW(T6963_WR);
	LOW(T6963_CE.CEN);
	if(OCTET & 0x01) HIGH(T6963_D0); else LOW(T6963_D0);
	if(OCTET & 0x02) HIGH(T6963_D1); else LOW(T6963_D1);
	if(OCTET & 0x04) HIGH(T6963_D2); else LOW(T6963_D2);
	if(OCTET & 0x08) HIGH(T6963_D3); else LOW(T6963_D3);
	if(OCTET & 0x10) HIGH(T6963_D4); else LOW(T6963_D4);
	if(OCTET & 0x20) HIGH(T6963_D5); else LOW(T6963_D5);
	if(OCTET & 0x40) HIGH(T6963_D6); else LOW(T6963_D6);
	if(OCTET & 0x80) HIGH(T6963_D7); else LOW(T6963_D7);
//PORTD=OCTET;



	T6963_DELAY();
	HIGH(T6963_WR);
	HIGH(T6963_CE.CEN);
	if(DC)
		{
		HIGH(T6963_CD);
		}

}

uint8_t _T6963_R8BIT()
{
   uint8_t octet;
   while(!(T6963_CHCECKSTATUS()&0x03));

   _T6963_BUSINPUT();

   LOW(T6963_RD);
   LOW(T6963_CE.CEN);
   LOW(T6963_CD);
   T6963_DELAY();

   octet=_T6963_READ();

   HIGH(T6963_RD);
   HIGH(T6963_CE.CEN);
   HIGH(T6963_CD);

   _T6963_BUSOUTPUT();

   return octet;
}


void T6963_WRITEDISPLAY(uint8_t OCTET)
{
	_T6963_W8BIT(T6963_DAT, OCTET);
	_T6963_W8BIT(T6963_COM, T6963_DATA_WRITE_AND_INCREMENT);
}


void _T6963_SETADDRESS (uint16_t ADDRESS)
{
	_T6963_W8BIT(T6963_DAT, ADDRESS & 0xFF);
	_T6963_W8BIT(T6963_DAT, ADDRESS >> 8);
	_T6963_W8BIT(T6963_COM, T6963_SET_ADDRESS_POINTER);
}

void T6963_CLR()
{
	_T6963_SETADDRESS(GLCD_TEXT_HOME);
	for(uint16_t i=0; i < GLCD_TEXT_SIZE; i++)
	{
		T6963_WRITEDISPLAY(0x00);
	}
}

void T6963_CLRGEN()
{
	_T6963_SETADDRESS(GLCD_EXTERNAL_CG_HOME);
	for(uint16_t i=0; i < 256*8; i++)
	{
		T6963_WRITEDISPLAY(0x00);
	}
}


void T6963_CHAR(uint8_t CHAR)
{
	T6963_WRITEDISPLAY(CHAR-32);
}


void T6963_TEXT(char *TEXT)
{
	while(*TEXT)  T6963_CHAR(*TEXT++);
}


void T6963_XY8(uint8_t PX, uint8_t PY8)
{
	uint16_t ADDRESS = GLCD_TEXT_HOME +  PX + (GLCD_TEXT_AREA * PY8);
	_T6963_SETADDRESS(ADDRESS);
}

//TODO: dopisaæ reszte graficznych instrukcji



void T6963_GFXCLR()
{
	_T6963_SETADDRESS(GLCD_GRAPHIC_HOME);
	for(uint16_t i=0; i < GLCD_GRAPHIC_SIZE; i++)
	{
		T6963_WRITEDISPLAY(0x00);
	}
}

void T6963_GFXPXY(uint8_t x, uint8_t y)
{
unsigned int address;
address = GLCD_GRAPHIC_HOME + (x / GLCD_FONT_WIDTH) + (GLCD_GRAPHIC_AREA * y);
_T6963_SETADDRESS(address);
}

//-------------------------------------------------------------------------------------------------
//
// Displays bitmap from program memory
//
//-------------------------------------------------------------------------------------------------
void T6963_GFX2LCD(uint8_t *IMG, uint8_t X, uint8_t Y, uint8_t WIDTH, uint8_t HEIGHT)
{
uint8_t i, j;

for(j = 0; j < HEIGHT; j++)
{
	T6963_GFXPXY(X, Y + j);
for(i = 0; i < WIDTH/GLCD_FONT_WIDTH; i++)
  {
	T6963_WRITEDISPLAY(pgm_read_byte(IMG + i + (GLCD_GRAPHIC_AREA * j)));
  }
}
}

//-------------------------------------------------------------------------------------------------
//
// Writes single char pattern to character generator area of display RAM memory
//
//-------------------------------------------------------------------------------------------------
void T6963_CUSTOMCHAR(uint8_t charCode, uint8_t * defChar)
{
uint16_t address;
uint8_t i;

address = GLCD_EXTERNAL_CG_HOME + (8 * charCode);

_T6963_SETADDRESS(address);

for(i = 0; i < 8 ; i++)
  {
	T6963_WRITEDISPLAY(*(defChar + i));
  }
}

//-------------------------------------------------------------------------------------------------
//
// Set (if color==1) or clear (if color==0) pixel on screen
//
//-------------------------------------------------------------------------------------------------
void T6963_GFXPIXEL(uint8_t x, uint8_t y, uint8_t color)
{
uint8_t tmp;
uint16_t address;

address = GLCD_GRAPHIC_HOME + (x / GLCD_FONT_WIDTH) + (GLCD_GRAPHIC_AREA * y);

_T6963_SETADDRESS(address);

_T6963_W8BIT(T6963_COM,T6963_DATA_READ_AND_NONVARIABLE);
tmp = _T6963_R8BIT();

if(color)
  tmp |= (1 <<  (GLCD_FONT_WIDTH - 1 - (x % GLCD_FONT_WIDTH)));
else
 tmp &= ~(1 <<  (GLCD_FONT_WIDTH - 1 - (x % GLCD_FONT_WIDTH)));

T6963_WRITEDISPLAY(tmp);

}
