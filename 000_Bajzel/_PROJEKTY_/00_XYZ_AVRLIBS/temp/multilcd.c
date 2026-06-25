/*                  e-gadget.header
 * main.c
 *
 *  Created on: 2015-12-06
 *    Modyfied: 2015-12-06 18:08:13
 *      Author: D¹browski Tomasz
 *
 * Project name: "11_M32_MULTILCD"
 *
 *          MCU: ATmega32
 *        F_CPU: 8 000 000 Hz
 *
 *    Flash: 4 250 bytes   [ 13,0 % ]
 *      RAM:  170 bytes   [ 8,3 % ]
 *   EEPROM:  0 bytes   [ 0,0 % ]
 *
 */



#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdlib.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "ADDS/tools.h"
#include "HD44780/hd44780.h"
//#include "SPI.h"
#include "ADDS/charsets.h"

#define led1 B,7
#define led2 w2b(PB,4)

void NAPIS(char* napis);
char buf[16];
TSCREEN wysw[]={
			{16,2,w2b(PA,7)},
			{16,4,w2b(PA,6)},
			{16,4,w2b(PA,5)},
			{20,2,w2b(PA,4)}
	};


int main(void) //rozpoczynamy
{

#if PINARRAY

///for uint8_u i=0
HD44780_NR(0,wysw);
HD44780_INIT();
HD44780_NR(1,wysw);
HD44780_INIT();
HD44780_NR(2,wysw);
HD44780_INIT();
HD44780_NR(3,wysw);

#endif

HD44780_INIT();
HD44780_CLR();
PORTB=0xAA;
HD44780_TEXT("TEST");

_delay_ms(500);



	while(1) //pêtla nieskoñczona
	{
			NAPIS("Start skanowania magistrali");
			NAPIS(__DATE__ __TIME__);
			sprintf(buf,"%02X",SIGNATURE_0);
			NAPIS(buf);
	}
}


void NAPIS(char* napis)
{
static uint8_t nr=0;
//static uint8_t line;
HD44780_NR(nr,wysw);
HD44780_CLR();
_delay_ms(20);
HD44780_CHAR(48+nr);

HD44780_TEXT(napis);
HD44780_CHAR('0'+SIZE(wysw));
nr=nr+1;
if (nr>=SIZE(wysw)) nr=0;
_delay_ms(2000);

}








