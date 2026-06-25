/*
 * AVT009 main.c
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 2020-04-21   
 *     Edycja: 09:37:00
 */




#include <avr/io.h>
#include <inttypes.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <util/delay.h>



#include "makra.h"
#include "harddef.h"
#include "lcd.h"
#include "locale.h"


void delay_10ms8(uint8_t t)
{
	do
	{
		_delay_ms(10);
	} while(t--!=0);
}


int main(void)  //rozpoczynamy
{
	// Inicjacja wyprowadzeñ
	DDR(LCD_CTRLPORT) = (1<<LCD_E | 1<<LCD_RW
			| 1<<LCD_RS | 1<<LCD_LED);
	PORT(LCD_CTRLPORT) = ~(1<<LCD_E | 1<<LCD_LED);
	//Test LCD
	lcd_Init();
	lcd_SetStatus(LCD_STATUS_DISP | LCD_STATUS_CURSOR);
	//wypisanie powitania
	fputs_P(str_Hello, lcd_GetFile());
	lcd_Update();
	delay_10ms8(200);
	//Przewijanie
	uint8_t start=0;



	while(1) //pêtla nieskoñczona
	{
		lcd_Cls();
		fputs_P(str_all+start, lcd_GetFile());
		lcd_Update();

		if(++start >= strlen_P(str_all)) start = 0;
		delay_10ms8(100);
	}
	return 0;
}
