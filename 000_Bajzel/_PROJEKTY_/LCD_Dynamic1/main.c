/////////////////////////////////////////////////////////////////////////////////////////////
// main.c - testowanie zaawansowanej obs³ugi alfanumerycznego wyœwietlacza LCD
//		Program dla p³ytki testowej AVT3505
//
// Autor: Rados³aw Koppel	Kompilator: WinAVR 20060125
/////////////////////////////////////////////////////////////////////////////////////////////
#include <avr/io.h>
#include <inttypes.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "makra.h"
#include "harddef.h"
#include "lcd.h"
#include "local.h"

// OpóŸnienie o 10ms x t
void delay_10ms8(uint8_t t)
{
	do
	{
		_delay_ms(10); // dla 8MHz mo¿emy opóŸniæ o max 32ms
	} while(t--!=0);
}

int main(void)
{
	// Inicjacja wyprowadzeñ
	DDR(LCD_CTRLPORT) = (1<<LCD_E | 1<<LCD_RW | 1<<LCD_RS | 1<<LCD_LED); 
	PORT(LCD_CTRLPORT) = ~(1<<LCD_E | 1<<LCD_LED); 
	// Test biblioteki wyœwietlacza
	lcd_Init();
	
	lcd_SetStatus(LCD_STATUS_DISP|LCD_STATUS_CURSOR);
	// Wypisanie przywitania
	fputs_P(str_all, lcd_GetFile());
	lcd_Update();
	delay_10ms8(200);
	// Przewijanie 
	uint8_t start=0;
	for(;;)
	{
		lcd_Cls();
		fputs_P(str_all+start, lcd_GetFile());
		lcd_Update();
		
		if(++start >= strlen_P(str_all))
			start = 0;
			
		delay_10ms8(100);
	}
	return 0;
}
