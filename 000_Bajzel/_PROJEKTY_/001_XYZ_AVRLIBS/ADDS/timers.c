/*
 * A32_HD44780 timers.c
 *
 *  Stworzono: 2014 22-09-2014
 *      Autor: D¹browski Tomasz
 *     Edycja: 18:19:09
 */



#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include "timers.h"

void TIMER0_INIT(){
	    // timer0 - konfig

	/* tryb zwyk³y
	TCCR0 |= (1<<CS02)|(1<<CS00); //prescaler = 1024

		TIMSK |= (1<<TOIE0); // zezwolenie na przerwanie OVF
		TCNT0 = 256-78; // czàstotliwoŠ 100 Hz w ka¿dym przerwaniu -regulacja
	*/
	//tryb ctc
		TCCR0 |= (1<<WGM01);                            /* tryb CTC */
        TCCR0 |= (1<<CS01)|(1<<CS00);           /* preskaler = 1024 */
        OCR0 = 249;                                                     /* dodatkowy podzia³ przez 208 (rej. przepe³nienia) */
        TIMSK |= (1<<OCIE0);

}
