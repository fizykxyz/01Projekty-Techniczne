/*
 * ATMEGA32L_LCD isrt0c_w7seg.h
 *
 *  Stworzono: 2014 05-10-2014
 *      Autor: D¹browski Tomasz
 *     Edycja: 07:20:41
 */

#ifndef ISRT0C_W7SEG_H_
#define ISRT0C_W7SEG_H_

ISR( TIMER0_COMP_vect ) {

	++millis;
	static uint8_t wysw=0;
for (uint8_t i=0;i<sizeof segm;i++){
	if (wysw == i) {
		SEG7PORTSEG &= ~(1 << segm[i]);
		if ((BUF_7SEG[wysw]<42) || (BUF_7SEG[wysw]>127)){
			SEG7PORT = 255;}
		else{
			SEG7PORT = ~pgm_read_byte(&ZNAK_7SEG[BUF_7SEG[i]-42]);
		if ((millis/500)%2 && (wysw == 1)) SEG7PORT &= ~(1<<7);
    }
	}
	else{
		SEG7PORTSEG |= (1 << segm[i]);
	}
}
++wysw;
		if (wysw > sizeof segm) wysw=0;
}


#endif /* ISRT0C_W7SEG_H_ */
