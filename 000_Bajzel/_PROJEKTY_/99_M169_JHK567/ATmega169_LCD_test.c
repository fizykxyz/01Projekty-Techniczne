/*================================================================
==================================================================
=	Plik glowny programu testowego ATmega169 + LCD
=	wersja = 1.0
=	Copywright - LiveLights
==================================================================	
================================================================*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "ATmega169_LCD_test.h"
#include <util/delay.h>
#include "lcd_JHK567.h"
#include "uart.h"



//================================================================
//===== Zmienne globalne =========================================
//================================================================

//================================================================


void piec(){
	LCD567_segment(SEG_T3,1);
		LCD567_segment(SEG_6A,1);
		LCD567_segment(SEG_6B,1);
		LCD567_segment(SEG_6E,1);
		LCD567_segment(SEG_6F,1);
		LCD567_segment(SEG_6G,1);

		LCD567_segment(SEG_5E,1);
		LCD567_segment(SEG_5F,1);

		LCD567_segment(SEG_4A,1);
		LCD567_segment(SEG_4D,1);
		LCD567_segment(SEG_4E,1);
		LCD567_segment(SEG_4F,1);
		LCD567_segment(SEG_4G,1);

		LCD567_segment(SEG_3A,1);
		LCD567_segment(SEG_3D,1);
		LCD567_segment(SEG_3E,1);
		LCD567_segment(SEG_3F,1);

}

//=====================================================================================================
//===== GLOWNA CZESC PROGRAMU =========================================================================
//=====================================================================================================
int main(void)
{
//	unsigned int xx1;

//	uart_init(115200);																// inicjalizacja UARTa
	//uart_tab("Test m169+LCD\n\r",0);

	LCD567_init();																	// inicjalizacja LCD

/*	LCD567_digit(0,0);
	LCD567_digit(1,1);
	LCD567_digit(2,2);
	LCD567_digit(3,3);
	LCD567_digit(4,4);
	LCD567_digit(5,5);
	LCD567_digit(6,6);
	LCD567_digit(7,7);
	LCD567_digit(8,8);*/

//	LCD567_segment(SEG_M3,1);
//	LCD567_segment(SEG_L,1);
//	LCD567_segment(SEG_K,1);
//	LCD567_segment(SEG_W,1);
//	LCD567_segment(SEG_H,1);;
//	LCD567_segment(SEG_M,1);
//	LCD567_segment(SEG_G,1);
//	LCD567_segment(SEG_J,1);
//	LCD567_segment(SEG_C,1);


	//LCD567_segment(SEG_4G,1);

//	_delay_ms(6000);
//	LCD567_small_number(95);
//	LCD567_number(30067);
	//LCDDR18=0b11111111;
	while(1)
	{
		//LCD567_number(1234567);
	//	LCD567_number(30067);

//		LCD567_segment(SEG_8K,0);
//		LCD567_segment(SEG_T1,1);
//		LCD567_segment(SEG_H5,0);
//		LCD567_segment(SEG_H0,1);
//		_delay_ms(6000);
//
//		LCD567_number(9999999);
//
//		LCD567_segment(SEG_T1,0);
//		LCD567_segment(SEG_T2,1);
//		LCD567_segment(SEG_H0,0);
//		LCD567_segment(SEG_H1,1);
//		_delay_ms(6000);
//
//		LCD567_number(-72005);
//
//		LCD567_segment(SEG_T2,0);
//		LCD567_segment(SEG_T3,1);
//		LCD567_segment(SEG_H1,0);
//		LCD567_segment(SEG_H2,1);
//		_delay_ms(6000);
//
//		LCD567_number(10000001); // error
//
//		LCD567_segment(SEG_T3,0);
//		LCD567_segment(SEG_8J,1);
//		LCD567_segment(SEG_H2,0);
//		LCD567_segment(SEG_H3,1);
//		_delay_ms(6000);
//
//		LCD567_number(-1000000); // error
//
//		LCD567_segment(SEG_8J,0);
//		LCD567_segment(SEG_8K,1);
//		LCD567_segment(SEG_H3,0);
//		LCD567_segment(SEG_H4,1);
//		_delay_ms(6000);
//
//		LCD567_number(0);
//
//		LCD567_segment(SEG_8K,0);
//		LCD567_segment(SEG_T1,1);
//		LCD567_segment(SEG_H4,0);
//		LCD567_segment(SEG_H5,1);
//		_delay_ms(6000);

		for (uint16_t i=0;i<999999;i++)
		{
			LCD567_number(i);

			LCD567_segment(SEG_C,(i/50)%2);
			piec();

			_delay_ms(40);
		}
	}



	//DDRF = 0xFF;
/*	while(1)
	{
		LCDDR0 = 0xFF;
		LCDDR5 = 0xFF;
		LCDDR10 = 0x00;
		LCDDR15 = 0x00;

		//PORTF = 0xFF;
		//_delay_ms(1000);

		LCDDR0 = 0x00;
		LCDDR5 = 0x00;
		LCDDR10 = 0xFF;
		LCDDR15 = 0xFF;

		//PORTF = 0x00;
		//_delay_ms(1000);

		LCD567_segment(SEG_M3,1);

		for(xx1=0;xx1<160;xx1++)
		{
			LCD567_segment(xx1,1);
			_delay_ms(300);
		}

		for(xx1=160;xx1!=0;xx1--)
		{
			LCD567_segment(xx1-1,0);
			_delay_ms(300);
		}
	}*/

	return 0;
}
//=====================================================================================================
