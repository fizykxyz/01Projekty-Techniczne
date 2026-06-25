//Uwaga!
//Je¿eli po zaprogramowaniu mikrokontrolera wyœwietlacz nie chce ruszyæ poprawnie
//wy³¹cz zasilanie i w³¹cz ponownie
//

#include <avr/io.h>
#include <util/delay.h>
#include "hd44780.h"
#include "T6963C.h"
#include "graphic.h"


typedef struct
{ uint8_t secs;
  uint8_t mins;
  uint8_t hours;
} t_time;

typedef struct
{ uint8_t days;
  uint8_t month;
  uint16_t year;
  } t_date;

typedef struct
{ t_time times;
  t_date dates;
} t_datetime;


t_datetime dataczas;



int main(void)
{
   dataczas.dates.year=2016;
	GLCD_Initalize(); // Initalize LCD

	GLCD_ClearText(); // Clear text area
	GLCD_ClearCG(); // Clear character generator area
	GLCD_ClearGraphic(); // Clear graphic area

	GLCD_TextGoTo(0,0);// set text coordinates
	GLCD_WriteString("Dziala Wyswietlacz"); // write text
	GLCD_TextGoTo(0,2);
	GLCD_WriteString("012345678901234567890");

	unsigned char i;
	unsigned char kierunek = 0;
    
	char napis[] = "LCD 3 piny " ;
	char text[]  = "74HC595 ";

	lcd_init();

	LCD_CLEAR;
uint8_t licznik=0x20;

	while(1)
	{
        licznik++;
        GLCD_TextGoTo(0,3);
        GLCD_WriteChar(licznik);
		_delay_ms(500);
		LCD_LOCATE(5,1);
		lcd_puts(napis);
		    
    
		_delay_ms(500);
		LCD_LOCATE(8,0);
		lcd_puts(text);

		_delay_ms(500);
		LCD_LOCATE(5,3);
		lcd_puts(text);

		_delay_ms(500);
		LCD_LOCATE(3,2);
		lcd_puts(napis);

		_delay_ms(1000);

		//chowamy zawartoœæ wyœwietlacza w jedn¹ ze stron
		for(i=0; i<16; i++){

		  if(kierunek)
		  	LCD_SHIFT_DISPLAY(LCDLEFT);
		  else
		  	LCD_SHIFT_DISPLAY(LCDRIGHT);

		  _delay_ms(300);
		}

		LCD_CLEAR;

		kierunek ^= 1;	//zmieñ kierunek na przeciwny

		//migamy diodami kilka razy
		for(i=0; i<255; i++){

			//LED na pinie Q7		
			if(i & 1) Q71;	//zgaœ 	gdy i jest nieparzyste
			else 	  Q70;	//zapal gdy i jest parzyste

			//LED na pinie Q6
			if(i & 1) Q61;				//zgaœ 	gdy i jest nieparzyste
			else 	  Q60;				//zapal gdy i jest parzyste

		  _delay_ms(30);
		}

    }
  
}
