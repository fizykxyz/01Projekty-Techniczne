/*                  e-gadget.header
 * timers.c
 *
 *  Created on: 2015-12-06
 *    Modyfied: 2015-12-06 18:08:13
 *      Author: D¹browski Tomasz
 *
 * Project name: "11_M32_MULTILCD"
 *
 *          MCU: ATmega32
 *        F_CPU: 8 000 000 Hz
 *
 *    Flash: 4 250 bytes   [ 13,0 % ]
 *      RAM:  170 bytes   [ 8,3 % ]
 *   EEPROM:  0 bytes   [ 0,0 % ]
 *
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

