//ledmatrix7219d88 output example

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "ledmatrix7219d88/ledmatrix7219d88.h"
//#define FDATE (char const[]){ __DATE__[7], __DATE__[8], ..., ' ', ... , '\0' } //Znaki pl

/*
 * main function
 */
int main(void) {
	//init ledmatrix
	ledmatrix7219d88_init();


	uint8_t ledmatrix = 0;

	//display test rows
	uint8_t rows[8] = {
			0b10000001,
			0b01000010,
			0b00100100,
			0b00011000,
			0b00011000,
			0b00100100,
			0b01000010,
			0b10000001
	};
	for(int i=0;i<5;i++){

		max7219_intensity(i,1);
	ledmatrix = i;
	ledmatrix7219d88_setmatrix(i, rows);
	_delay_ms(2000);
	}
	//display test rows
	ledmatrix = 0;
	ledmatrix7219d88_setrow(ledmatrix, 0, 0b11111111);
	ledmatrix7219d88_setrow(ledmatrix, 1, 0b01000000);
	ledmatrix7219d88_setrow(ledmatrix, 2, 0b00100000);
	ledmatrix7219d88_setrow(ledmatrix, 3, 0b00010000);
	ledmatrix7219d88_setrow(ledmatrix, 4, 0b00001000);
	ledmatrix7219d88_setrow(ledmatrix, 5, 0b00000100);
	ledmatrix7219d88_setrow(ledmatrix, 6, 0b00000010);
	ledmatrix7219d88_setrow(ledmatrix, 7, 0b00000001);
	ledmatrix = 1;
	ledmatrix7219d88_setrow(ledmatrix, 0, 0b10101010);
	ledmatrix7219d88_setrow(ledmatrix, 1, 0b01010101);
	ledmatrix7219d88_setrow(ledmatrix, 2, 0b10101010);
	ledmatrix7219d88_setrow(ledmatrix, 3, 0b01010101);
	ledmatrix7219d88_setrow(ledmatrix, 4, 0b10101010);
	ledmatrix7219d88_setrow(ledmatrix, 5, 0b01010101);
	ledmatrix7219d88_setrow(ledmatrix, 6, 0b10101010);
	ledmatrix7219d88_setrow(ledmatrix, 7, 0b01010101);
	_delay_ms(2000);
	ledmatrix7219d88_resetmatrix(0);
	ledmatrix7219d88_resetmatrix(1);

	//test loop
	uint8_t p = 1;
	for(;;) {

		int8_t led = 0;
		//loop every matrix
		for(ledmatrix=0; ledmatrix<LEDMATRIX7219D88_MAXLEDMATRIX; ledmatrix++){
			//loop every led
			for(led=0; led<64; led++) {
				if(p)
					ledmatrix7219d88_setledon(ledmatrix, led);
				else
					ledmatrix7219d88_setledoff(ledmatrix, led);
				_delay_ms(50);
			}
			//ledmatrix7219d88_resetmatrix(ledmatrix);
		}

		if(p)
			p = 0;
		else
			p = 1;

		_delay_ms(1000);
	}
}
