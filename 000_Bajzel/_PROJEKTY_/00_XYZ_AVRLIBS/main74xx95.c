#include <avr/io.h>
#include <util/delay.h>

#include "ADDS/pins.h"
#include "U74xx595/U74xx595.h"

TU74xx595 shifter[]={{G2O(PC,0)},{G2O(PA,6)}};

void legacy(uint8_t OCTET)
{
	PORTD=OCTET;
}



//void LED (void (*function)(uint8_t), uint8_t c) {
//  (*function)(c);
//}

void (*LED)(uint8_t c);

void register_LED (void (*function) (uint8_t c))
{
LED = function;
}

int main()
{
     DDRD=255;
	 U74xx595_INIT();
	 register_LED(legacy);
	 LED(0x55);

	 _delay_ms(1000);

	 LED(0x0F);
	 register_LED(U74xx595_W8BIT);
	 _delay_ms(1000);
	 LED(0xF0);


	while(1)
	{




	}
}
