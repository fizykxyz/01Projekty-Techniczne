/*
 * 06_M32_PCD8544 main.c
 *
 *  Stworzono: 2015 9 mar 2015
 *      Autor: D¹browski Tomasz
 *     Edycja: 18:46:28
 */



#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>

//#include "HD44780.h"
//#include "SPI.h"

#include "PCD8544/pcd8544.h"
#include "PCD8544/font_verdana_14x14.h"

#define P_ON(prt,pin) (PORT##prt) |= (1<<(P##prt##pin))
#define P_OFF(prt,pin) (PORT##prt) &= ~(1<<(P##prt##pin))
#define P_SW(prt,pin) (PORT##prt) ^= (1<<(P##prt##pin))
#define P_ST(prt,pin) (PORT##prt) & (1<<(P##prt##pin))

//#define F_CPU 8000000 //ustawienie oscylatora na 8MHz

uint8_t szer=60;
uint8_t wys8=3;
//uint8_t gfxbuf[szer*wys8];
uint8_t gfxbuf[180];


int main(void) { //rozpoczynamy


PCD8544_INIT();
PCD8544_TEXT("£oœ¿ó³wiok",normal);

//pêtla nieskoñczona


//PCD8544_GFXTEXT(ekran,size1x,&font_verdana_14x14,10,30,szer,wys,"£",normal);
PCD8544_GFXTEXT(gfxbuf,size1x,font_verdana_14x14,1,7,szer,wys8,"BARAN",normal);
PCD8544_GFX2LCD(RAM,gfxbuf,10,2,szer,wys8);

_delay_ms(5000);
while(1){

for (uint8_t i='A';i<'Z';i++)
{
	PCD8544_GFXCLR(gfxbuf,size1x,szer,wys8);
	PCD8544_GFXCHAR(gfxbuf,size2x,font_verdana_14x14,0,7,szer,wys8,i,normal);
	PCD8544_GFX2LCD(RAM,gfxbuf,10,2,szer,wys8);
_delay_ms(500);
	}
}
}
