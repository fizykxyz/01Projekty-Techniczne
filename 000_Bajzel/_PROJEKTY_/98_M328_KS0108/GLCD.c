#include <avr\io.h>
#include "KS0108.h"
#include "graphics.h"

int main()
{
	GLCD_init();
//	color=1;
	GLCD_cls();
	for(uint8_t r=0;r<32;r++) GLCD_Circle(64, 32, r);
	GLCD_goto(2,2);

	GLCD_puttext("Test LCD!!");
	GLCD_puttext_P(PSTR("Napis we FLASH"));
	GLCD_Line(0,0, 127,63);
	GLCD_Line(0,63, 127, 0);
}
