/*
 * 00_XYZ_AVRLIBS max7219.c
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 2016-09-26   
 *     Edycja: 11:48:24
 */

#include <avr/io.h>
#include <util/delay.h>

#include "../ADDS/pins.h"
#include "max7219.h"

void MAX7219_INIT()
{
	//output
	OUTPUT(MAX7219_DIN);
	OUTPUT(MAX7219_CLK);
	OUTPUT(MAX7219_LOAD);

	MAX7219_WREG(MAX7219_REGSHUTDOWN, 0x01); // normal mode
	MAX7219_WREG(MAX7219_REGINTENSITY, 0x0F); // set intensity 0x00 - 0x0F
	MAX7219_WREG(MAX7219_REGSCANLIMIT, 0x07); // scan digits 0,1
    MAX7219_WREG(MAX7219_REGDECODE, 0x00); // no decoding
    MAX7219_WREG(MAX7219_REGTEST, 0x00);
//  MAX7219_WREG(MAX721CLEAR);
}

void MAX7219_W8BIT(uint8_t OCTET)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		LOW(MAX7219_CLK);
		if ((OCTET << i) & 0x80)
			HIGH(MAX7219_DIN);
		else
			LOW(MAX7219_DIN);
		HIGH(MAX7219_CLK);
	}
}

void MAX7219_WREG(uint8_t REG, uint8_t DATA)
{
	LOW(MAX7219_LOAD);
	MAX7219_W8BIT(REG);
	MAX7219_W8BIT(DATA);
	HIGH(MAX7219_LOAD);
}

void MAX7219_BRIGHTNESS (uint8_t BRIGHT)
{
  MAX7219_WREG(MAX7219_REGINTENSITY, BRIGHT & 0x0F);
}

