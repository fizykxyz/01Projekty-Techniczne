/*
 * main.c
 *
 *  Created on: 20 lis 2014
 *      Author: Tomek
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

//#include "font_small_4x5.h"
uint8_t i,j=0;

#define MIN(A,B)  (((A)<(B)) ? (A) : (B) )
#define MAX(A,B)  (((A)>(B)) ? (A) : (B) )
#define PUT_IN_RANGE(V,VMIN,VMAX) MAX(VMIN,MIN(VMAX,V))
#define MAP_TO_RANGE(V,VMIN0,VMAX0,VMIN1,VMAX1) ( (VMIN1) +  ( (V) – (VMIN0) ) * ( (VMAX1) – (VMIN1) ) / ( (VMAX0) – (VMIN0) ) )


/* inne - lepsze definicje */
// MACROS FOR EASY PIN HANDLING FOR ATMEL GCC-AVR
//these macros are used indirectly by other macros , mainly for string concatination
#define _SET(type,name,bit)            type ## name  |= _BV(bit)
#define _CLEAR(type,name,bit)        type ## name  &= ~ _BV(bit)
#define _TOGGLE(type,name,bit)        type ## name  ^= _BV(bit)
#define _GET(type,name,bit)            ((type ## name >> bit) &  1)
#define _PUT(type,name,bit,value)    type ## name = ( type ## name & ( ~ _BV(bit)) ) | ( ( 1 & (unsigned char)value ) << bit )

//these macros are used by end user
#define OUTPUT(pin)            _SET(DDR,pin)
#define INPUT(pin)            _CLEAR(DDR,pin)
#define HIGH(pin)            _SET(PORT,pin)
#define LOW(pin)            _CLEAR(PORT,pin)
#define TOGGLE(pin)            _TOGGLE(PORT,pin)
#define READ(pin)            _GET(PIN,pin)
#define PUT(pin,value)      _PUT(PORT,pin,(value))
/*
    BASIC STAMPS STYLE COMMANDS FOR ATMEL GCC-AVR

    Usage Example:
    ———————————————–
    #define pinLed B,5  //define pins like this

    OUTPUT(pinLED);     //compiles as DDRB |= (1<<5);
    HIGH(pinLed);         //compiles as PORTB |= (1<<5);
    ———————————————–
*/




//definicje portów umo¿liwiaj¹ce dzia³anie w formie numerycznej np.PORT(0)=PORT(PD)=PORTD
#define PORTS 4
//#define PD 0
//#define PC 1
//#define PB 2
//#define PA 3


#define PA 0
#define PB 1
#define PC 2
#define PD 3


volatile uint8_t *myports[] = { &PORTA, &PORTB, &PORTC,&PORTD };


#define _DDR(x) _SFR_IO8(_SFR_IO_ADDR(*(myports[x]))-1) // adr. r. kier. PORTx
#define _PORT(x) _SFR_IO8(_SFR_IO_ADDR(*(myports[x]))) // adr. r. wyj. PORTx
#define _PIN(x) _SFR_IO8(_SFR_IO_ADDR(*(myports[x]))-2) // adr. r. wej. PORTx

//#define PIN(x) (*(&x - 2))      /* address of PIN register of port x */
//#define DDR(x) (*(&x - 1))      /* address of data direction (DDR) register of port x */



#define DDR(p) _DDR(p)
#define PORT(p) _PORT(p)
#define PIN(p) _PIN(p)

//#define DDR(p) _SFR_IO8(0x11+p*3)
//#define PORT(p) _SFR_IO8(0x12+p*3)
//#define PIN(p) _SFR_IO8(0x10+p*3)


//makro zmiany opisu Px,y np PD,5 na BAJT w formacie xxxxxyyy czyli port 0-31, pin 0-7 i vice-versa
#define w2b(port,pin) (((port)<<3)|(pin))
#define b2p(bajt) ((bajt)>>3) //bajt to port
#define b2b(bajt) ((bajt)&(0x07)) //bajt to bity

//zmiana zapisu A na PA
#define p2p(x,y)  (x)##(y)

//makra obs³ugi pinów w formacie LOW_A(w2b(PA,3))
#define OUTPUT_A(pin)            (DDR(b2p(pin))  |= (1 << (b2b(pin))))
#define INPUT_A(pin)            (DDR(b2p(pin))  &= ~ (1 << (b2b(pin))))
#define HIGH_A(pin)            (PORT(b2p(pin))  |= (1 << (b2b(pin))))
#define LOW_A(pin)            (PORT(b2p(pin))  &= ~ (1 << (b2b(pin))))
#define TOGGLE_A(pin)          (PORT(b2p(pin)) ^= (1 << (b2b(pin))))
#define READ_A(pin)            ((PIN(b2p(pin)) >> (b2b(pin))) &  1)
#define PUT_A(pin,value)      (PORT(b2p(pin)) =  (PORT(b2p(pin)) & ( ~ _BV(b2b(pin))) ) | ( ( 1 & (unsigned char)value ) << b2b(pin)) )





#define DIGITS 4
uint8_t CYFRY[] = {

						w2b(PC,7),
						w2b(PC,5),
						w2b(PC,4),
						w2b(PD,3),
						w2b(PD,6),
						w2b(PD,4)
				};


volatile uint16_t millis;

				uint8_t SCREEN[6] ={0xE5,0xE0,                          // char 0  (48)
		    0xFB,0xC0,                          // char 1  (49)
		    0x69,0x90                           // char 2  (50)
};
/*
				uint8_t SCREEN[6] ={0x01,0x03,                          // char 0  (48)
				   0x04,0x08,                          // char 1  (49)
		           0x0A,0x0F                           // char 2  (50)
};
*/

enum PINY {OE1,OE2,OE3,OE4,W1,W2,W3,W4,K1,K2,K3,K4};
enum LOHI {LO,HI};


#define KOLUMN 12

uint8_t PINS[]={w2b(PC,2),w2b(PC,3),w2b(PC,4),w2b(PC,5), //Aktywator modu³u OE1-OE4 [0-3]
			    w2b(PA,0),w2b(PA,1),w2b(PA,2),w2b(PA,3), //Wiersz W1-W4 [4-7]
				w2b(PA,4),w2b(PA,5),w2b(PA,6),w2b(PA,7)  //Kolumna modu³u K1-K4 [8-11]
				};

#define led D,0
#define led1 D,1


#define PRG 2


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

void poczekaj(){
	_delay_ms(10);
}

int main(void){
//	TIMER0_INIT();
	//cli();

#if (PRG == 1)
	DDRA=0xFF;
	PORTA=0xFF;
	while(1){
		TOGGLE_A(PINS[W4]);
		_delay_ms(1000);
	}
	#elif (PRG == 2)
	for (i=0;i<KOLUMN;i++){
		OUTPUT_A(PINS[i]);
		PUT_A(PINS[i],HI);
	}
	while(1){


		for (i=0;i<KOLUMN;i++){
			for ( j=0;j<12;j++){
				        				//OUTPUT_A(PINS[i]);
				        				PUT_A(PINS[j],HI);
				        }


			uint8_t bajt=SCREEN[(i%6)];
			PUT_A(PINS[((i/4)+OE2)],LO); //OE dla grupy kolumn
			poczekaj();
	        PUT_A(PINS[((i%4)+K1)],LO); //kolumna w grupie
	        poczekaj();
                 bajt=(bajt>>(4*(i%2)));
	        if(bajt & 0x01) PUT_A(PINS[W4],LO); else PUT_A(PINS[W4],HI);
	        if(bajt & 0x02) PUT_A(PINS[W3],LO); else PUT_A(PINS[W3],HI);
	        if(bajt & 0x04) PUT_A(PINS[W2],LO); else PUT_A(PINS[W2],HI);
	        if(bajt & 0x08) PUT_A(PINS[W1],LO); else PUT_A(PINS[W1],HI);




//	        PUT_A(PINS[W1],(bajt & 0x08)); //stan W1 na podstawie bufora ekranu
//	        poczekaj();
//	        PUT_A(PINS[W2],(bajt & 0x04)); //stan W2 na podstawie bufora ekranu
//	        poczekaj();
//	        PUT_A(PINS[W3],(bajt & 0x02)); //stan W3 na podstawie bufora ekranu
//	        poczekaj();
//	        PUT_A(PINS[W4],(bajt & 0x01)); //((bajt>>((i%2)*4)) & 0x08)); //stan W4 na podstawie bufora ekranu
//	        poczekaj();
	        PUT_A(PINS[OE1],LO); //W³¹czenie wierszy
	        poczekaj();
	        _delay_ms(200);


		}




	}


#endif

}

ISR( TIMER0_COMP_vect ) {

	++millis;
//	static uint8_t kolumna=0;

//++kolumna;
//		if (kolumna > KOLUMN) kolumna=0;
}

