/*
 * _1wirebb.c
 *
 * Created: 2014-08-23 10:47:04
 *  Author: tmf
 */ 


#include <avr\io.h>
#include <util\delay.h>
#include <avr\interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "1wire_basic.h"
#include "1wire.h"
#include "DS18B20.h"
#include "KS0108lib.h"
#include "graphics.h"

int main()
{
	OW_init();
	GLCD_init();
	GLCD_cls();

	char buf[9];
	uint8_t aID[8];
	uint8_t ID[4][7];
	uint8_t i, pos=0;
	int16_t temp;

	for(i=0;i<4;i++)
	{
		Error=OW_OK;
		pos=OWI_Search(OW_SearchROM, aID, pos);
		if(Error!=OW_OK) break;
		memcpy(&ID[i][0],aID, 7);  //Skopiuj zeskanowane ID
		GLCD_goto(0,i*9);
		for(uint8_t ii=0;ii<8;ii++)
		{
			sprintf(buf,"%02X",aID[ii]);
			GLCD_puttext(buf);
		}
		if(pos==0) break;
	}

	while(1)
	{
		OW_Start_Conversion(1);
		//_delay_ms(750);
		for(uint8_t ii=0;ii<=i;ii++)
		{
			temp=OW_GetTemperature(&ID[ii][0]);
			sprintf(buf,"%+03i.%1i", temp/16, (temp & 0b1111)*625);
			GLCD_goto(0,36+ii*9);
			GLCD_puttext(buf);
		}
	}
}

