//**********************************************************
// plik hd44780.c
//
//LCD HDD44780 na 3 pinach 74HC595
//Wersja: 1.0
//Licencja: GNU GPL
//Autor: Sheep94
//Skype: asus850032
//
//szczegoly: http://mikrokontrolery.blogspot.com/2011/03/lcd-hd44780-na-3-pinach-74hc595.html
//
//Uwaga!
//Je¿eli po zaprogramowaniu mikrokontrolera wyœwietlacz nie chce ruszyæ poprawnie
//wy³¹cz zasilanie i w³¹cz ponownie
//
//**********************************************************

#include <avr/io.h>
#include <util/delay.h>
#include "hd44780.h"



void rejestr(unsigned char data, unsigned char WriteOrErase)
{
	if(WriteOrErase==1)
		tempdata=(tempdata|data);
	else
		tempdata&=~(data);
	STCP_0;

	SHCP_0;
	if(tempdata & 0x80)DS_1;else DS_0;
	SHCP_1;
	SHCP_0;
	if(tempdata & 0x40)DS_1;else DS_0;
	SHCP_1;
	SHCP_0;
	if(tempdata & 0x20)DS_1;else DS_0;
	SHCP_1;
	SHCP_0;
	if(tempdata & 0x10)DS_1;else DS_0;
	SHCP_1;
	SHCP_0;
	if(tempdata & 0x08)DS_1;else DS_0;
	SHCP_1;
	SHCP_0;
	if(tempdata & 0x04)DS_1;else DS_0;
	SHCP_1;
	SHCP_0;
	if(tempdata & 0x02)DS_1;else DS_0;
	SHCP_1;
	SHCP_0;
	if(tempdata & 0x01)DS_1;else DS_0;
	SHCP_1;
	STCP_1;
}

void WriteToLCD (char v,unsigned char rs)
{
	
	if(rs==1)RS1; else RS0;
	
	E1;

    if(v&0x10) D41; else D40;
    if(v&0x20) D51; else D50;
    if(v&0x40) D61; else D60;
    if(v&0x80) D71; else D70;
    E0;  
    _delay_ms(2);
	
	E1;
	
    if(v&0x01) D41; else D40;
    if(v&0x02) D51; else D50;
    if(v&0x04) D61; else D60;
    if(v&0x08) D71; else D70;
    E0;
	_delay_ms(2);
	
}

void lcd_init(void)
{

	#ifdef LCD_START_DELAY_MS
		_delay_ms(LCD_START_DELAY_MS);
	#endif

	DDR_DS |= (1<<pin_DS);
	DDR_STCP |= (1<<pin_STCP);
	DDR_SHCP |= (1<<pin_SHCP);

	//RW0;
	WriteToLCD(0x02,LCDCOMMAND);
	WriteToLCD(0x28,LCDCOMMAND);
    LCD_DISPLAY(0x04);
    LCD_CLEAR;
}


void lcd_puts(char *str)
{
    unsigned char i =0;

    while(str[i])
        LCD_WRITE_DATA(str[i++]) ;
}
