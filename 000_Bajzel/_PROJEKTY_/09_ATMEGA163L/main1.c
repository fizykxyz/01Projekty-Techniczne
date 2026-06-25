/*
 * 09_ATMEGA163L main.c
 *
 *  Stworzono: 2015 14 cze 2015
 *      Autor: D¹browski Tomasz
 *     Edycja: 19:39:04
 */

/*
 *
 *
 */

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include <math.h>

#include <avr/pgmspace.h>



#include "ADDS/tools.h"
#include "ADDS/LEDS_MAIN.h"

#include "ADDS/LEDS_EXT.h"

#include "ADDS/LEDS_7SEG.h"

#define DDR(x) _SFR_IO8(_SFR_IO_ADDR(x)-1) // adr. r. kier. PORTx
#define PIN(x) _SFR_IO8(_SFR_IO_ADDR(x)-2) // adr. r. wej. PORTx

int freeRam11 () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

//void TIMER0_INIT(){
//	    // timer0 - konfig
//
//	/* tryb zwyk³y
//	TCCR0 |= (1<<CS02)|(1<<CS00); //prescaler = 1024
//
//		TIMSK |= (1<<TOIE0); // zezwolenie na przerwanie OVF
//		TCNT0 = 256-78; // czàstotliwoŠ 100 Hz w ka¿dym przerwaniu -regulacja
//	*/
//	//tryb ctc
//		TCCR0 |= (1<<PWM01);                            /* tryb CTC */
//        TCCR0 |= (1<<CS01)|(1<<CS00);           /* preskaler = 1024 */
//        OCR0 = 249;                                                     /* dodatkowy podzia³ przez 208 (rej. przepe³nienia) */
//        TIMSK |= (1<<OCIE0);
//
//}

//_SFR_IO8(0x25)=(0);
//
//
//TCCR2=0;
//
//TCCR2=(1<<PWM2);
//
//				|(1<<COM21)|(1<<CTC2);
//


#define DIGITS 4

uint8_t CYFRY[DIGITS][2] = {

						{PD,2},
						{PB,3},
						{PB,1},
						{PB,2}
				};

uint8_t pozc=0;

int main(void) { //rozpoczynamy

	for (uint8_t p=0;p<PORTS;p++){
        DDR(p)=0xFF;
		PORT(p)=0xFF;
		}


	 PORT(PD)&=~(SegA|SegD);

//uint16_t liczba=1531;

#define CASE 0
while(1){ //pêtla nieskoñczona



		for (uint8_t a=0;a<8;a++){
			for (uint8_t t=0;t<10;t++){





                          //     if (pozc>(DIGITS-2)) pozc=0;
pozc=t%3;
                         PORT(PD)=~(pgm_read_byte(&ZNAK_7SEG[CYFRY[pozc][1]]));
//                       PORT(PB)&=~(1<<3);
//PORTB=0;
#if (CASE==0)

                         for (uint8_t i=0;i<3;i++){
                        	  PORT(CYFRY[pozc][0])|=(1<<CYFRY[pozc][1]);
                         }



                        		   PORT(CYFRY[pozc][0])&=~(1<<CYFRY[pozc][1]);



#else
                               switch(pozc) {
                                 case 0:
                                 { Dig_1(1);Dig_2(0);Dig_3(0);
                                 break;}
                                 case 1:
                                 { Dig_1(0);Dig_2(1);Dig_3(0);
                                 break;}
                                 case 2:
                                 {  Dig_1(0);Dig_2(0);Dig_3(1);
                                 break;}
                                 default:
                                { Dig_1(1);Dig_2(0);Dig_3(0);
                                 } }
#endif
                               pozc++;
                delayms(500);
			}
		}
	}
}
