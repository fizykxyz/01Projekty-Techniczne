/*
 * virtualOLED.c
 *
 *      Author: artur85
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "virtualOLED.h"
#include "virtualOLED_UART.h"


void vOLED_Init()
{
	USART_Init(__UBRR);

}


void vOLED_Pixel(int x, int y, uint8_t bw)
{
	if ((x<0)|| (x>OLED_WIDTH) || (y<0) || (y>OLED_HEIGHT)) return;
		if(bw)
			USART_Transmit(127+2);
		else
			USART_Transmit(127+3);
		USART_Transmit(x);

		USART_Transmit(y);
}

void vOLED_cls()
{
	USART_Transmit(127+5);

}

void vOLED_display()
{
	USART_Transmit(127+4);
	_delay_ms(50);

}

void vOLED_bitmap(int x, int y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color)
{
	int i, j, byteWidth =(w+7)/8;
	for (j=0;j<h;j++){
		for(i=0;i<w;i++){
			if (pgm_read_byte(bitmap + j * byteWidth + i /8 ) & (128>>(i&7))){
				vOLED_Pixel(x+i, y+j, color);
			}
		}
	}
}

