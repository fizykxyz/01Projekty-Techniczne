/*
 * main.c
 *
 * Created: 2014-02-12 16:16:13
 *  Author: Piotr
 *
 * Description: Program demonstruj¹cy dzia³anie przetwornika ADC i wyœwietlacza LCD
 */ 


#include <avr/io.h>
#include <stdio.h>

#include "HD44780.h"
#include "ADC.h"


int main(void)
{
	LCD_Initalize();//inicjalizacja LCD
	
	ADC_init();
	
	LCD_GoTo(0,0);//ustawiamy siê w pierwszej kolumnie i pierwszym wierszu
	LCD_WriteText("LCD ADC display");
	
	LCD_GoTo(0,1);//ustawiamy siê w pierwszej kolumnie i drugim wierszu
	LCD_WriteText("A0=     A1=    ");
	
	char buffer[6];
	uint16_t val;
	
    while(1)
    {
        val=ADC_get(0);//pobieram odczyt
        sprintf(buffer,"%04d",val);//konwertujê go na ci¹g znaków i dopisujê zera
        LCD_GoTo(3,1);//ustawiam siê na pocz¹tku pola, w którym ma byæ widoczny wynik
        LCD_WriteText(buffer);//wyœwietlam go (nadpisujê poprzedni)
		
		val=ADC_get(1);
		sprintf(buffer,"%04d",val);
		LCD_GoTo(11,1);
		LCD_WriteText(buffer);
    }
}