#include <avr/io.h>
#include <util/delay.h>

#include "T6963C.h"
#include "graphic.h"


#define DEBUG 1

#if DEBUG == 1
#define BlinkLed PORTB^=(1<<5)
#else
#define BlinkLed
#endif



int main(void)
{
DDRB=0xFF;
BlinkLed;
GLCD_Initalize(); // Initalize LCD
BlinkLed;
GLCD_ClearText(); // Clear text area
GLCD_ClearCG(); // Clear character generator area
GLCD_ClearGraphic(); // Clear graphic area

GLCD_TextGoTo(0,0);// set text coordinates
GLCD_WriteString("Dziala Wyswietlacz"); // write text
GLCD_TextGoTo(0,2);
GLCD_WriteString("012345678901234567890");
GLCD_Circle(32,32,20); // draw circle
GLCD_Circle(108,32,20); // draw circle
//
//GLCD_Rectangle(8,8,124, 48); // draw rectangle
//_delay_ms(1000);
GLCD_Rectangle(0,0,128, 64); // draw rectangle
GLCD_Rectangle(105,0,2, 64); // draw rectangle
uint8_t los=0;
GLCD_Line(0,31,127,31);
_delay_ms(5000);
GLCD_ClearText(); // Clear text area
_delay_ms(500);
GLCD_ClearCG(); // Clear character generator area
_delay_ms(500);
GLCD_ClearGraphic(); // Clear graphic area

_delay_ms(500);
while(1){


	//GLCD_ClearGraphic();
	BlinkLed;
	_delay_ms(50);
	los++;
	GLCD_SetPixel((los*2)%128,(los*3)%64,1);

	GLCD_TextGoTo(los%16,los/16);
	GLCD_WriteChar((los+30)%256);
	GLCD_Line(0,los%64,127,los%64);
}

return 0;
}
