// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.

#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "gfx.h"    // Core graphics library
#include "tft_lcd.h" // Hardware-specific library
//#include "touch.h"



void testText(void);

int main(void) {

	tft_reset();
//	uint16_t identifier = tft_readID();
	tft_init();//(0x9341);

	tft_setRotation(3);

	testText();

	while (1) {


//		_delay_ms(2000);
	}
}

void testText(void) {
	tft_fillScreen(BLACK);
	tft_setCursor(0, 0);
	tft_setTextColor(WHITE);
	tft_setTextSize(1);
	tft_String("Jakistam tekst bardzo mala czcionka rozmiar 1\n");
	tft_setTextColor(YELLOW);
	tft_setTextSize(2);
	tft_String("czcionka numer 2\n");
	tft_setTextColor(RED);
	tft_setTextSize(3);
	tft_String("Czcionka numer 3\n");
	tft_write('\n');
	tft_setTextColor(GREEN);
	tft_setTextSize(4);
	tft_String("czcionka 4\n");
	tft_setTextSize(1);

	tft_Int(5);
	tft_write(',');
	tft_write(6);//kod ascii !!!
	tft_write('\n');


//	tft_String("5: 123abc\n");
//	tft_setTextSize(6);
//	tft_String("6: Aa");
//	tft_setTextSize(1);


	tft_String("-----------------------------------------------\n");
	tft_String("`1234567890-= ~!@#$%^&*()_+\n");
	tft_String("qwertyuiop[]asdfghjkl;'zxcvbnm,./\n");
	tft_String("QWERTYUIOP{}|ASDFGHJKL:ZXCVBNM<>?");
}
