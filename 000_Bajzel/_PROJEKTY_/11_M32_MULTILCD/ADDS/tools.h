/*
 * ATMEGA32L_LCD tools.h
 *
 *  Stworzono: 2014 17-09-2014
 *      Autor: D¹browski Tomasz
 *     Edycja: 22:40:10
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <avr/io.h>

#define PINARRAY 1


#define HI 1
#define LO 0

//definicja delams


/*
void delayms( uint16_t ms )
{
for(; ms>0; ms--){
uint16_t i;
for(i=(1000/(16000000UL/F_CPU)); i>0; i--){
__asm__ __volatile__ ("nop");
__asm__ __volatile__ ("nop");
}
}
}

*/
#define NIBBLE2HEX(NIBBLE) ((NIBBLE)>9) ? ((NIBBLE)+55) : ((NIBBLE)+48);

#define SIZE(ARRAY) ((sizeof ARRAY)/(sizeof ARRAY[0]))
#define MIN(A,B)  (((A)<(B)) ? (A) : (B) )
#define MAX(A,B)  (((A)>(B)) ? (A) : (B) )
#define SWAP(a,b)({a ^= b; b ^= a; a ^= b;})
#define PUT_IN_RANGE(V,VMIN,VMAX) MAX(VMIN,MIN(VMAX,V))
#define MAP_TO_RANGE(V,VMIN0,VMAX0,VMIN1,VMAX1) ( (VMIN1) +  ( (V) – (VMIN0) ) * ( (VMAX1) – (VMIN1) ) / ( (VMAX0) – (VMIN0) ) )

#define TOSTRING(a) STRINGX (a)
#define STRINGX(a)  #a

/* inne - lepsze definicje */
// MACROS FOR EASY PIN HANDLING FOR ATMEL GCC-AVR
//these macros are used indirectly by other macros , mainly for string concatination
#define _SET(type,name,bit)            type ## name  |= (1 << (bit))
#define _CLEAR(type,name,bit)        type ## name  &= ~ (1 << (bit))
#define _TOGGLE(type,name,bit)        type ## name  ^= (1 << (bit))
//#define _GET(type,name,bit)            ((type ## name >> bit) &  1)
#define _GET(type,name,bit)        (type ## name & (1 << (bit)))
#define _PUT(type,name,bit,value)    type ## name = ( type ## name & ( ~ (1 << (bit))) ) | ( ( 1 & (unsigned char)value ) << bit )





//these macros are used by end user
#define OUTPUT(pin)            (_SET(DDR,pin))
#define INPUT(pin)            (_CLEAR(DDR,pin))
#define HIGH(pin)            (_SET(PORT,pin))
#define LOW(pin)            (_CLEAR(PORT,pin))
#define TOGGLE(pin)            (_TOGGLE(PORT,pin))
#define READ(pin)            (_GET(PIN,pin))
#define PUT(type,name,bit,value) _PUT(type,name,bit,value)
/*
    BASIC STAMPS STYLE COMMANDS FOR ATMEL GCC-AVR

    Usage Example:
    ———————————————–
    #define pinLed B,5  //define pins like this

    OUTPUT(pinLED);     //compiles as DDRB |= (1<<5);
    HIGH(pinLed);         //compiles as PORTB |= (1<<5);
    ———————————————–
*/


// Zmienna opisuj¹ca ekran jako KOL-szerokoœæ, LIN -wysokoœæ i liniê steruj¹c¹ E

typedef struct {
	uint8_t KOL;//il.kolumn
	uint8_t LIN;//il.linii
    uint8_t EN;//linia sygna³owa Enable
} TSCREEN;


//definicje portów umo¿liwiaj¹ce dzia³anie w formie numerycznej np.PORT(0)=PORT(PD)=PORTD


#if PINARRAY
extern volatile uint8_t *portyIO[];
#define PORTS 4
#define PA 0
#define PB 1
#define PC 2
#define PD 3

#define DDR(x) _SFR_IO8(_SFR_IO_ADDR(*(portyIO[x]))-1) // adr. r. kier. PORTx
#define PORT(x) _SFR_IO8(_SFR_IO_ADDR(*(portyIO[x]))) // adr. r. wyj. PORTx
#define PIN(x) _SFR_IO8(_SFR_IO_ADDR(*(portyIO[x]))-2) // adr. r. wej. PORTx

//makro zmiany opisu Px,y np PD,5 na BAJT w formacie xxxxxyyy czyli port 0-31, pin 0-7 i vice-versa
#define w2b(port,pin) (((port)<<3)|(pin))
#define b2p(bajt) ((bajt)>>3) //bajt to port
#define b2b(bajt) ((bajt)&(0x07)) //bajt to bity

//zmiana zapisu A na PA
#define p2p(x,y)  (x)##(y)

//makra obs³ugi pinów w formacie LOW_A(w2b(PA,3))
//makra obs³ugi pinów w formacie LOW_A(w2b(PA,3))
#define OUTPUT_A(pin)            (DDR(b2p(pin))  |= (1 << (b2b(pin))))
#define INPUT_A(pin)            (DDR(b2p(pin))  &= ~ (1 << (b2b(pin))))
#define HIGH_A(pin)            (PORT(b2p(pin))  |= (1 << (b2b(pin))))
#define LOW_A(pin)            (PORT(b2p(pin))  &= ~ (1 << (b2b(pin))))
#define TOGGLE_A(pin)          (PORT(b2p(pin)) ^= (1 << (b2b(pin))))
#define READ_A(pin)            ((PIN(b2p(pin)) >> (b2b(pin))) &  1)

#endif






#endif /* TOOLS_H_ */
