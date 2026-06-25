/*
 * main.c
 *
 *  Created on: 25 lis 2014
 *      Author: Tomek
 */
#include <avr/io.h>
#include <util/delay.h>
#include "PCD8544/pcd8544.h"
#include "ADDS/tools.h"

#define LED D,0

int main(void){
	OUTPUT(LED);
PCD8544_INIT();
PCD8544_TEXT("No to ¿ó³wiki",under|trail1|whole);
	while(1){
for (uint8_t i=0;i<255;i++){
		PCD8544_XY8(10,4);
		PCD8544_CHAR(48+i%10,bold|lead1);
        _delay_ms(500);
        TOGGLE(LED);
	}
}
}
