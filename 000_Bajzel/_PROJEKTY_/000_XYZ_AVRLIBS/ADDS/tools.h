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


#define _STRING(a)  #a
#define STRINGX(a) _STRING(a)





#endif /* TOOLS_H_ */
