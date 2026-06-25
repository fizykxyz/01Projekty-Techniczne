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

#define GLUE(a, b)     a##b
#define GET(/* PIN, */ x) ((GET_(x)==1) ? 1:0)



//u?ywa? podwojnego ! czyli !! by zamiast np (var>>i) & 1 by?o !!(val & (1<<i) czyli z jakiego? niezerowego zrobi 1 a z zerowego 1



// EPOCH = Jan 1 1970 00:00:00
#define	EPOCH_YR	1970
//(24L * 60L * 60L)
#define	SECS_DAY	86400UL
#define	LEAPYEAR(year)	(!((year) % 4) && (((year) % 100) || !((year) % 400)))
#define przestepny(year) ((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0))
#define	YEARSIZE(year)	(LEAPYEAR(year) ? 366 : 365)
#define YEARDAYS(month)	(((month)==12) ? 365 : (()) )
#define MONTHDAYS(month) (YEARDAYS(month)-YEARDAYS(month-1))



/* inne - lepsze definicje */
// MACROS FOR EASY PIN HANDLING FOR ATMEL GCC-AVR
//these macros are used indirectly by other macros , mainly for string concatination
#define _SET(type,name,bit)            type ## name  |= _BV(bit)
#define _CLEAR(type,name,bit)        type ## name  &= ~ _BV(bit)
#define _TOGGLE(type,name,bit)        type ## name  ^= _BV(bit)
#define _GET(type,name,bit)            ((type ## name >> bit) &  1)
#define _PUT(type,name,bit,value)    type ## name = ( type ## name & ( ~ _BV(bit)) ) | ( ( 1 & (unsigned char)value ) << bit )


#define SET(what, x) _SET(what, x)
#define CLR(what, x) _CLEAR(what, x)
//#define GET(x) (_GET(PIN,x))





//these macros are used by end user
#define OUTPUT(pin)            _SET(DDR,pin)
#define INPUT(pin)            _CLEAR(DDR,pin)
#define HIGH(pin)            _SET(PORT,pin)
#define LOW(pin)            _CLEAR(PORT,pin)
#define TOGGLE(pin)            _TOGGLE(PORT,pin)
#define READ(pin)            _GET(PIN,pin)

/*
    BASIC STAMPS STYLE COMMANDS FOR ATMEL GCC-AVR

    Usage Example:
    ———————————————–
    #define pinLed B,5  //define pins like this

    OUTPUT(pinLED);     //compiles as DDRB |= (1<<5);
    HIGH(pinLed);         //compiles as PORTB |= (1<<5);
    ———————————————–
*/


//definicja delams

#define XTAL 16000000UL
/*
void delayms( uint16_t ms )
{
for(; ms>0; ms--){
uint16_t i;
for(i=(1000/(8000000/XTAL)); i>0; i--){
__asm__ __volatile__ ("nop");
__asm__ __volatile__ ("nop");
__asm__ __volatile__ ("nop");
__asm__ __volatile__ ("nop");
}
}
}

*/

//definicje portów umo¿liwiaj¹ce dzia³anie w formie numerycznej np.PORT(0)=PORT(PD)=PORTD
#define PORTS 4
#define PA 0
#define PB 1
#define PC 2
#define PD 3

volatile uint8_t *myports[] = { &PORTA, &PORTB, &PORTC,&PORTD };


#define _DDR(x) _SFR_IO8(_SFR_IO_ADDR(*(myports[x]))-1) // adr. r. kier. PORTx
#define _PORT(x) _SFR_IO8(_SFR_IO_ADDR(*(myports[x]))) // adr. r. wyj. PORTx
#define _PIN(x) _SFR_IO8(_SFR_IO_ADDR(*(myports[x]))-2) // adr. r. wej. PORTx
#define DDR(p) _DDR(p)
#define PORT(p) _PORT(p)
#define PIN(p) _PIN(p)

#endif /* TOOLS_H_ */
