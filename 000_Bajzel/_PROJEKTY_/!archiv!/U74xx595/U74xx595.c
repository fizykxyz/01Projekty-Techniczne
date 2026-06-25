/*
 * 00_XYZ_AVRLIBS U74xx595.c
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 2016-09-19   
 *     Edycja: 11:57:20
 */


#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "U74xx595.h"



void U74xx595_NR(uint8_t NR, TU74xx595 *STRUCT)
{
	U74xx595_OE.COE=STRUCT[NR].COE;
}


void U74xx595_INIT()
{
	OUTPUT(U74xx595_OE.COE);
	OUTPUT(U74xx595_DS);
	OUTPUT(U74xx595_SH);
	OUTPUT(U74xx595_ST);


}


void U74xx595_START(TU74xx595 *STRUCT,uint8_t SIZE)
{
	for(uint8_t nrLcd=0;nrLcd<SIZE;nrLcd++)
		{
			U74xx595_NR(nrLcd,STRUCT);
			U74xx595_INIT();
			_delay_ms(20);
			//HD44780_CHAR(SIZE+48);
		}

}



void U74xx595_W8BIT (uint8_t OCTET)
{
	LOW(U74xx595_OE.COE);
	LOW(U74xx595_DS);
	LOW(U74xx595_SH);
	LOW(U74xx595_ST);
	for(uint8_t i=0;i<8;i++)
	{

		if ((OCTET << BITORDER[i])  & (0x80)) HIGH(U74xx595_DS); else LOW(U74xx595_DS);

		HIGH(U74xx595_SH);
		LOW(U74xx595_SH);
	}
	HIGH(U74xx595_ST);
//	HIGH(U74xx595_OE.COE);
}
