/*
 * main.c
 *
 * Created: 2014-02-12 19:50:21
 *  Author: Piotr
 *
 * Description: Program demonstruj¹cy odbiór danych z pilota RC5
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "HD44780.h"
#include "RC5.h"

int main(void)
{
	
	LCD_Initalize();
	RC5_init();
	sei();
	
	LCD_GoTo(0,0);
	LCD_WriteText("  RC5 Receiver  ");
	
	char buffer[33];
	uint8_t addr, comm, tog;
	
    while(1)
    {
        if(RC5_get(&addr,&comm,&tog)){//sprawdzamy, czy system odebra³ jak¹œ ramkê RC5
			//jeœli tak to j¹ wyœwietlamy
			sprintf(buffer," A:%02d C:%02d T:%02d ",addr,comm,tog);
			LCD_GoTo(0,1);
			LCD_WriteText(buffer);
		}
    }
}