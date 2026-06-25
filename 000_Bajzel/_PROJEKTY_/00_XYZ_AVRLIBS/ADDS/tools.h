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
#include <util/delay.h>

#define HI 1
#define LO 0

#define RAM 0
#define FLASH 1
#define EEPROM 2

#define _GETDATA(mem,data) ((mem)==RAM) ? (data) : (pgm_read_byte(&##data))
#define GETDATA(mem,data) _GETDATA(mem,data)


//definicja delams



#define _NIBBLE2HEX(NIBBLE) ((NIBBLE)>9) ? ((NIBBLE)+55) : ((NIBBLE)+48)
#define NIBBLE2HEX(NIBBLE) _NIBBLE2HEX(NIBBLE)

#define MSBNIBBLE(OCTET) (((OCTET) >> 4) &0x0F)
#define LSBNIBBLE(OCTET) ((OCTET) & 0x0F)

#define _SIZE(ARRAY) ((sizeof (ARRAY))/(sizeof (ARRAY[0])))
#define SIZE(ARRAY) _SIZE(ARRAY)

#define MIN(A,B)  (((A)<(B)) ? (A) : (B) )
#define MAX(A,B)  (((A)>(B)) ? (A) : (B) )
#define SWAP(A,B) ({(A) ^= (B); (B) ^= (A); (A) ^= (B);})

#define PUT_IN_RANGE(V,VMIN,VMAX) MAX((VMIN),MIN((VMAX),(V)))
#define OVERFLOW_RANGE(V,VMIN,VMAX)) (V) //dopisaæ aby przechodzi³ >VMAX->VMIN, <VMIN->VMAX
#define MAP_TO_RANGE(V,VMIN0,VMAX0,VMIN1,VMAX1) ( (VMIN1) +  ( (V) – (VMIN0) ) * ( (VMAX1) – (VMIN1) ) / ( (VMAX0) – (VMIN0) ) )


#define _STRING(A)  #A
#define STRING(A) _STRING(A)

#define LogError(p) kprintf("%s: %s:%s:%d\n", (#p), __FILE__, __FUNCTION__, __LINE__)

//#define delayms(MS) {while((MS)--) {_delay_ms(1);}}}


void delayms(uint16_t ms)
{
	while(ms--)
	{
		_delay_ms(1);
	}
}


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

#endif /* TOOLS_H_ */
