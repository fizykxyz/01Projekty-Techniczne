/*
 * ATMEGA32L_LCD DS18B20.c
 *
 *  Stworzono: 2014-09-18
 *      Autor: D¹browski Tomasz
 *     Edycja: 21:54:56
 */

//#include <inttypes.h>
//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <avr/sleep.h>
#include <util/delay.h>
//#include <stdio.h>

//#include "../ONEWIRE/onewire.h"
#include "ds18xxx.h"


// Odczyt trybu zasilania dla danego czujnika
uint8_t DS18xxx_PARASITE(uint8_t *ID)
{
	  uint8_t PARASITE = EXT_POWER;
	  OW_RESETPULSE();
	  OW_COMMAND(ID, DS18XXX_POWER_SUPPLY, EXT_POWER);
	  if (_OW_IO1BIT(1, EXT_POWER) == 0) PARASITE = PARASITE_MODE;
	  OW_RESETPULSE();
	  return PARASITE;
}


uint8_t DS18xxx_STARTMEAS(uint8_t *ID, uint8_t PARASITE)
{
	OW_RESETPULSE();
	OW_COMMAND(ID,DS18XXX_CONVERT_T, PARASITE);
	return OW_ERR_OK;
}

// Odczyt notatnika (scratch) z czujnika ID do tablicy SCRACTH, od³ugoœci LEN
uint8_t DS18xxx_READSCRATCH(uint8_t *ID, uint8_t *SCRATCH, uint8_t LEN)
{
	//OW_RESETPULSE();
	OW_COMMAND(ID, DS18XXX_READ, EXT_POWER);
	for (uint8_t i=0;i<LEN;i++)
	{
		SCRATCH[i]=OW_R8BIT();
	}
	return OW_ERR_OK;
}



// Odczyt temperatury bez oczekiwania 750ms
uint16_t DS18xxx_READ_RAWTEMP(uint8_t *ID, uint8_t PARASITE)
{

	uint16_t ret;
	OW_COMMAND(ID, DS18XXX_READ, PARASITE);
	ret=OW_R8BIT();
	ret=ret|OW_R8BIT()<<8;
	return ret;
}



uint8_t DS18xxxSIGN(uint16_t RAW)
{
	return (RAW & 0x8000)>>15;
}

uint8_t DS18xxxINT(uint16_t RAW)
{
	if (RAW & 0x8000)
		{
		RAW^=0xFFFF;
		RAW++;
		}
	return RAW>>4;
}

uint16_t DS18xxxFRAC(uint16_t RAW)
{
	if (RAW & 0x8000)
		{
		RAW=~RAW;
		RAW++;
		}
	return (RAW & 0b1111)*625U;
}


//
//// Odczyt bitów temperatury dok³.12bit
//int16_t DS18B20_TMPBITS(uint8_t *ID, uint8_t PARASITE)
//{
//	int16_t DS_TEMP;
//    OW_COMMAND(ID, START_CONVERT, PARASITE );
//	_delay_ms(DS18xxx_TIME_CONV);
//	DS18xxx_READSCRATCH(ID, &DS_TEMP, 2);
//		return DS_TEMP;
//}
