/*
 * ATMEGA32L_LCD pcd8544.c
 *
 *  Stworzono: 2014 02-10-2014
 *      Autor: Dπbrowski Tomasz
 *     Edycja: 22:56:35
 */


#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include "avr/pgmspace.h"

#include "pcd8544.h"
#include "font_std_5x8.h"

#include "../HD44780/hd44780.h"
#include "../UART/uart.h"
void _PCD8544_8BIT(uint8_t DC, uint8_t OCTET)
{
	if(DC) SET(PORT, PCD8544_DC); else CLR(PORT, PCD8544_DC);

    uint8_t m;
    CLR(PORT, PCD8544_SCE);
     for(m=0x80; m; m>>=1)
    {
       if(OCTET & m)
          SET(PORT,PCD8544_SDIN);
       else
    	   CLR(PORT,PCD8544_SDIN);

       SET(PORT,PCD8544_SCLK);
       //LCD_NOP
       CLR(PORT,PCD8544_SCLK);
    }

     SET(PORT, PCD8544_SCE);
}

void PCD8544_INIT(void)
{
  SET(DDR, PCD8544_SCE);
  SET(DDR, PCD8544_RES);
  SET(DDR, PCD8544_DC);
  SET(DDR, PCD8544_SDIN);
  SET(DDR, PCD8544_SCLK);

  CLR(PORT, PCD8544_RES);
 // delay(1);
  SET(PORT, PCD8544_RES);

  _PCD8544_8BIT(PCD8544_COM, 0x21 );  // LCD Extended Commands.
  _PCD8544_8BIT(PCD8544_COM, 0xBf );  // Set LCD Vop (Contrast). //B1
  _PCD8544_8BIT(PCD8544_COM, 0x04 );  // Set Temp coefficent. //0x04
  _PCD8544_8BIT(PCD8544_COM, 0x14 );  // LCD bias mode 1:48. //0x13
  _PCD8544_8BIT(PCD8544_COM, 0x0C );  // LCD in normal mode. 0x0d for inverse
  _PCD8544_8BIT(PCD8544_COM, 0x20 );
  _PCD8544_8BIT(PCD8544_COM, 0x0C);

//  _PCD8544_8BIT(PCD8544_COM, 0x21 );  // LCD Extended Commands.
//   _PCD8544_8BIT(PCD8544_COM, 0xFF );  // Set LCD Vop (Contrast). //B1
//   _PCD8544_8BIT(PCD8544_COM, 0x06 );  // Set Temp coefficent. //0x04
//   _PCD8544_8BIT(PCD8544_COM, 0x13 );  // LCD bias mode 1:48. //0x13
//   _PCD8544_8BIT(PCD8544_COM, 0x20 );
//   _PCD8544_8BIT(PCD8544_COM, 0x0C );  // LCD in normal mode. 0x0d for inverse
//

  PCD8544_CLR();
  PCD8544_HOME();
}


void PCD8544_CONTRAST(uint8_t CONTRAST)
{
    _PCD8544_8BIT(PCD8544_COM, 0x21);

    // Ustawienie poziomu kontrastu
    _PCD8544_8BIT(PCD8544_COM, 0x80 | CONTRAST );

    // Tryb - horizontal addressing mode.
    _PCD8544_8BIT(PCD8544_COM, 0x20);
}

void PCD8544_MODE(uint8_t MODE){
	switch (MODE){
		case normal: _PCD8544_8BIT(PCD8544_COM,0x0C);
		break;
		case invert: _PCD8544_8BIT(PCD8544_COM,0x0D);
		break;
		case blank: _PCD8544_8BIT(PCD8544_COM,0x08);
		break;
		case allon: _PCD8544_8BIT(PCD8544_COM,0x09);
	    break;
	}
}


void PCD8544_CLR(void)
{
    uint16_t index;
	for (index = 0; index < PCD8544_W * PCD8544_H / 8; index++)
  {
    _PCD8544_8BIT(PCD8544_DAT, 0x00);
   // _delay_ms(100);
  }
}

void PCD8544_XY8(uint8_t PX, uint8_t PY8)
{
  _PCD8544_8BIT(PCD8544_COM, 0x80 | PX);  // Column.
  _PCD8544_8BIT(PCD8544_COM, 0x40 | PY8);  // Row.

}

void PCD8544_HOME(void){
	PCD8544_XY8( 0, 0);
}

void PCD8544_CHAR(uint8_t CHAR, uint8_t EFFECT)
{
  uint8_t index;
  uint8_t spc;
  uint8_t old=0;
  uint8_t new=0;
//  uint8_t rev=((EFFECT&revers)/revers);
  if (CHAR>0x7F){
  char pl[] = {"•∆ £—”åèØπÊÍ≥ÒÛúüø"};

  for(index=0; (index<sizeof pl) && (pl[index]!=CHAR); index++) ;
        if(index<18) CHAR= 0x80+index;

  }
  if ((CHAR>=pgm_read_byte(&FONT_STD_5x8[2])) && (CHAR<=pgm_read_byte(&FONT_STD_5x8[3]))){

  spc=((EFFECT&(lead1|lead2))/lead1);
  while (spc--){
       new= ((EFFECT&stroke)|(EFFECT&under)) * (EFFECT&whole)/whole;
	  _PCD8544_8BIT(PCD8544_DAT, new | (((EFFECT&bold)/bold) * old)) ;
       old=new;
  }
  for ( index = 0; index < pgm_read_byte(&FONT_STD_5x8[0]); index++)
  {
	  new = pgm_read_byte(&FONT_STD_5x8[(CHAR - pgm_read_byte(&FONT_STD_5x8[2]))*pgm_read_byte(&FONT_STD_5x8[0])+index+4])|(EFFECT&stroke)|(EFFECT&under);
	  	 _PCD8544_8BIT(PCD8544_DAT, new | (((EFFECT&bold)/bold) * old));
	  old = new;
  }
  spc=((EFFECT&(trail1|trail2))/trail1);
    while (spc--){
    	new= ((EFFECT&stroke)|(EFFECT&under)) * (EFFECT&whole)/whole;
    		  _PCD8544_8BIT(PCD8544_DAT, new | (((EFFECT&bold)/bold) * old)) ;
    	       old=new;
    	  }
  }
}

void PCD8544_TEXT(char * TEXT,uint8_t EFFECT)
{
while(*TEXT)  PCD8544_CHAR(*TEXT++,EFFECT);
}

#if PCD8544_GFX == 1

//int8_t GFX_CX=0,GFX_CY=0;

void PCD8544_GFXCLR(uint8_t IMG[],uint8_t MODE,int8_t WIDTH,int8_t HEIGHT8){
    uint16_t index;
	for	(index=0;index<WIDTH*HEIGHT8;index++){
		switch (MODE)
			    {
			      case normal: IMG[index] = 0x00; break;
			      case allon:  IMG[index] = 0xFF; break;
			      case invert: IMG[index] ^= 0xFF; break;
			    }
	}
}


void PCD8544_GFX2LCD(uint8_t MEMTYPE,unsigned char IMG[],int8_t POSX,int8_t POSY8,int8_t WIDTH,int8_t HEIGHT8){
	unsigned int x,y,index;
	for(x=0,y=0,index=0; y<HEIGHT8; y++)
	   {
		_PCD8544_8BIT(PCD8544_COM, 0x40|(POSY8+y)); // Ustawiamy wspÛ≥rzÍdnπ Y obszaru kopiowania
		_PCD8544_8BIT(PCD8544_COM, 0x80|(POSX)); // a teraz wspÛ≥rzÍdnπ X
		for(x=0; x<WIDTH ; x++,index++){
		    if (MEMTYPE)
			_PCD8544_8BIT(PCD8544_DAT, pgm_read_byte(&IMG[index]));
		    else
		    _PCD8544_8BIT(PCD8544_DAT, IMG[index]);
		}
	   }
}


uint8_t PCD8544_GFXROT(uint8_t ROT){  //odczyt = 0 > ustawia 1-0st,2-90st,3-180st,4-270st
static uint8_t rotation=1;
if (ROT){
	rotation=ROT;
	return 0;
}
else{
	return rotation;
}
}

void PCD8544_GFXPIXEL(uint8_t IMG[],uint8_t MODE, int8_t POSX,int8_t POSY,int8_t WIDTH,int8_t HEIGHT8){
	if ((POSX < 0) || (POSX >= WIDTH) || (POSY < 0) || (POSY >= HEIGHT8*8)) return;
		switch(PCD8544_GFXROT(0)){
	case rot0:
		break;
	case rot90:
		swap(POSX,POSY);
	    POSX = WIDTH - POSX - 1;
	    break;
	  case rot180:
	    POSX = WIDTH - POSX - 1;
	    POSY = HEIGHT8*8 - POSY - 1;
	    break;
	  case rot270:
		swap(POSX,POSY);
	    POSY = HEIGHT8*8 - POSY - 1;
	    break;
	}

	switch (MODE)
	    {
	      case normal: IMG[POSX+ (POSY/8)*WIDTH] |=  (1 << (POSY&7)); break;
	      case blank:  IMG[POSX+ (POSY/8)*WIDTH] &= ~(1 << (POSY&7)); break;
	      case invert: IMG[POSX+ (POSY/8)*WIDTH] ^=  (1 << (POSY&7)); break;
	    }
}

void PCD8544_GFXLINE(uint8_t IMG[],uint8_t MODE,int8_t POSX1, int8_t POSY1, int8_t POSX2, int8_t POSY2,int8_t WIDTH,int8_t HEIGHT8)
 {
     int8_t d, dx, dy, ai, bi, xi, yi; // zmienne pomocnicze
     if (POSX1 < POSX2) // ustalenie kierunku rysowania w osi x
		{ xi = 1; dx = POSX2 - POSX1; }
     else
		{ xi = -1; dx = POSX1 - POSX2; }
     if (POSY1 < POSY2) // ustalenie kierunku rysowania w osi y
		{ yi = 1; dy = POSY2 - POSY1; }
     else
		{ yi = -1; dy = POSY1 - POSY2; }

     PCD8544_GFXPIXEL(IMG,MODE,POSX1,POSY1,WIDTH,HEIGHT8); // pierwszy piksel
     if (dx > dy)  // oú wiodπca OX
     {   ai = (dy - dx) * 2; bi = dy * 2; d = bi - dx;
         while (POSX1 != POSX2) // pÍtla po kolejnych x
         {
             if (d >= 0)  // test wspÛ≥czynnika
              {  POSX1 += xi; POSY1 += yi; d += ai; }
             else
             {   d += bi; POSX1 += xi; }
             PCD8544_GFXPIXEL(IMG,MODE,POSX1,POSY1,WIDTH,HEIGHT8);
         }
     }
     else // oú wiodπca OY
     {   ai = ( dx - dy ) * 2; bi = dx * 2; d = bi - dy;
         while (POSY1 != POSY2) // pÍtla po kolejnych y
         {
             if (d >= 0)  // test wspÛ≥czynnika
             {   POSX1 += xi; POSY1 += yi; d += ai; }
             else
             {   d += bi; POSY1 += yi; }
             PCD8544_GFXPIXEL(IMG,MODE,POSX1,POSY1,WIDTH,HEIGHT8);
         }
     }
 }

void PCD8544_GFXBOX(uint8_t IMG[],uint8_t MODE,int8_t POSX1, int8_t POSY1, int8_t POSX2, int8_t POSY2,int8_t WIDTH,int8_t HEIGHT8)
{
	PCD8544_GFXLINE(IMG,MODE,POSX1,POSY1,POSX2,POSY1,WIDTH,HEIGHT8);
	PCD8544_GFXLINE(IMG,MODE,POSX2,POSY1,POSX2,POSY2,WIDTH,HEIGHT8);
	PCD8544_GFXLINE(IMG,MODE,POSX2,POSY2,POSX1,POSY2,WIDTH,HEIGHT8);
	PCD8544_GFXLINE(IMG,MODE,POSX1,POSY2,POSX1,POSY1,WIDTH,HEIGHT8);
}

void PCD8544_GFXBAR(uint8_t IMG[],uint8_t MODE,int8_t POSX1, int8_t POSY1, int8_t POSX2, int8_t POSY2,int8_t WIDTH,int8_t HEIGHT8)
{
	int8_t y;
	if (POSY1>POSY2) swap(POSY1,POSY2)
	for (y=POSY1;y<POSY2+1;y++)
		PCD8544_GFXLINE(IMG,MODE,POSX1,y,POSX2,y,WIDTH,HEIGHT8);

}
void PCD8544_GFXCIRCLE(uint8_t IMG[],uint8_t MODE,int8_t POSX, int8_t POSY, int8_t RADIUS,int8_t WIDTH,int8_t HEIGHT8)
{
  int8_t x = RADIUS;
  int8_t y = 0;
  int8_t radiusError = 1-x;

  while(x >= y)
  {
	    PCD8544_GFXPIXEL(IMG,MODE,x + POSX, y + POSY,WIDTH,HEIGHT8);
	    PCD8544_GFXPIXEL(IMG,MODE,y + POSX, x + POSY,WIDTH,HEIGHT8);
	    PCD8544_GFXPIXEL(IMG,MODE,-x + POSX, y + POSY,WIDTH,HEIGHT8);
	    PCD8544_GFXPIXEL(IMG,MODE,-y + POSX, x + POSY,WIDTH,HEIGHT8);
	    PCD8544_GFXPIXEL(IMG,MODE,-x + POSX, -y + POSY,WIDTH,HEIGHT8);
	    PCD8544_GFXPIXEL(IMG,MODE,-y + POSX, -x + POSY,WIDTH,HEIGHT8);
	    PCD8544_GFXPIXEL(IMG,MODE,x + POSX, -y + POSY,WIDTH,HEIGHT8);
	    PCD8544_GFXPIXEL(IMG,MODE,y + POSX, -x + POSY,WIDTH,HEIGHT8);
	  y++;
    if (radiusError<0)
    {
      radiusError += 2 * y + 1;
    }
    else
    {
      x--;
      radiusError += 2 * (y - x + 1);
    }
  }
}

void PCD8544_GFXCHAR(uint8_t IMG[],uint8_t MODE,uint8_t FONT[],int8_t POSX, int8_t POSY,int8_t WIDTH,int8_t HEIGHT8,uint8_t CHAR, uint8_t EFFECT)
{
  uint8_t index=0,y=0;
  uint8_t sizew=0,sizeh,size;
  uint8_t pages;
  size=((MODE&(size2x|size3x))/size2x);
  //size=2;
//  uint8_t spc;
//  uint8_t old=0;
//  uint8_t new=0;
  if (CHAR>0x7F){
  char pl[] = {"•∆ £—”åèØπÊÍ≥ÒÛúüø"};

  for(index=0; (index<sizeof pl) && (pl[index]!=CHAR); index++) ;
        if(index<18) CHAR= 0x80+index;

  }
  if ((CHAR>=pgm_read_byte(&FONT[2]))  && (CHAR<=pgm_read_byte(&FONT[3])))
		  {
	  sizew=pgm_read_byte(&FONT[0]);
	  sizeh=pgm_read_byte(&FONT[1]);
	  pages=(sizeh-1)/8+1;

    //  spc=((EFFECT&(lead1|lead2))/lead1);
//  while (spc--){
//       new= ((EFFECT&stroke)|(EFFECT&under)) * (EFFECT&whole)/whole;
//	  _PCD8544_8BIT(PCD8544_DAT, new | (((EFFECT&bold)/bold) * old)) ;
//       old=new;

  for ( index = 0; index < sizew; index++)
  {
	  for(y=0;y<sizeh;y++){
if ((pgm_read_byte(&FONT[4+(y/8)+(index+(CHAR-pgm_read_byte(&FONT[2]))*sizew)*pages])) & (1<<(y%8)))
{
//UART_SEND(size+48);
	if (size == 0)
	{
		PCD8544_GFXPIXEL(IMG,normal,index+POSX,y+POSY,WIDTH,HEIGHT8);
//		UART_SENDSTR(NL "TU ROZMIAR ");
//		UART_SEND(size+48);

	}
	else
	{
		PCD8544_GFXBAR(IMG,normal,index*size+POSX,y*size+POSY,index*size+size+POSX,y*size+size+POSY,WIDTH,HEIGHT8);
		//PCD8544_GFXCIRCLE(IMG,normal,POSX+index,POSY+y,size*2,WIDTH,HEIGHT8);
//		UART_SENDSTR(NL "Drugi rozmiar ");
//		UART_SEND(size+48);

	}

}
      //if (mode && normal)
	  }
//	  new = pgm_read_byte(&FONT_STD_5x8[(CHAR - pgm_read_byte(&FONT_STD_5x8[2]))*pgm_read_byte(&FONT_STD_5x8[0])+index+4])|(EFFECT&stroke)|(EFFECT&under);
//	   _PCD8544_8BIT(PCD8544_DAT, new | (((EFFECT&bold)/bold) * old));
//	  old = new;
  }
//  spc=((EFFECT&(trail1|trail2))/trail1);
//    while (spc--){
//    	new= ((EFFECT&stroke)|(EFFECT&under)) * (EFFECT&whole)/whole;
//    		  _PCD8544_8BIT(PCD8544_DAT, new | (((EFFECT&bold)/bold) * old)) ;
//    	       old=new;
    	  }

//GFX_CX=GFX_CX+sizew*(size+1);
}

void PCD8544_GFXTEXT(uint8_t IMG[],uint8_t MODE,uint8_t FONT[],int8_t POSX, int8_t POSY,int8_t WIDTH,int8_t HEIGHT8,char * TEXT, uint8_t EFFECT)
{
	while(*TEXT)
		{
		PCD8544_GFXCHAR(IMG,MODE,FONT,POSX,POSY,WIDTH,HEIGHT8,*TEXT++,EFFECT);
        POSX=POSX+(((MODE&(size2x|size3x))/size2x)+1)*pgm_read_byte(&FONT[0]);
		}
}
#endif

