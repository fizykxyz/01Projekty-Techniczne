/*
 * ATMEGA32L_LCD main.c
 *
 *  Stworzono: 2014 17-09-2014
 *      Autor: D¹browski Tomasz
 *     Edycja: 22:35:04
 */


#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <string.h>
//#include "util/atomic.h"

#include "ADDS/tools.h"
#include "ADDS/timers.h"
#include "ADDS/w7seg.h"
#include "ADDS/charsets.h"




#include "PCD8544/pcd8544.h"
//#include "ADDS/logonokia.h"
char liczba [8];
uint16_t obr=30;
volatile uint32_t millis=0;
char *int_to_str(int val, char *str, int8_t fw, char znak_wiodacy);

#define imp (PIND&(1<<7))

uint16_t s1,s2;
uint16_t rpm;

int main(void)
{
    TIMER0_INIT();
	DDRD&=~(1<<7);
	PORTD|=(1<<7);
	DDRD|=(1<<0);
	PCD8544_INIT();
	PCD8544_CLR();
	PCD8544_HOME();

	//PCD8544_GFX2LCD(FLASH,misio,5,0,84,8);
	PCD8544_XY8(5,3);
	PCD8544_TEXT("£oœ œciga siê z ¿ó³wiem",normal);
	//PCD8544_CONTRAST(255);
	PCD8544_XY8(64,0);
	PCD8544_TEXT("o/s",under|whole);
	//_delay_ms(2000);
	//_PCD8544_8BIT(PCD8544_COM, 0x14 );
	//_delay_ms(1000);
	sei();
	s1=millis/1000;
	_delay_ms(1000);

while(1)
{
for (uint8_t i=255;i>0;i--)
{
	PORTD^=(1<<0);
	//PCD8544_CONTRAST(i);
	if imp
	{
		//_delay_ms(5);
		 obr++;
	}

	_delay_ms(10);

	s2=millis/1000;
	if (s2!=s1)
	{
		rpm=obr/(s2-s1);
		obr=0;
		s1=s2;
	}

	PCD8544_XY8(0,0);
		//itoa(obr,liczba,10);
		PCD8544_TEXT(int_to_str(rpm,liczba,sizeof(liczba)-1,'0'),bold|trail2);

	PCD8544_XY8(0,1);
	//itoa(obr,liczba,10);
	PCD8544_TEXT(int_to_str(millis/1000,liczba,sizeof(liczba)-1,'0'),bold|trail2);
	PCD8544_XY8(0,2);
		//itoa(obr,liczba,10);
	PCD8544_TEXT(int_to_str(obr,liczba,sizeof(liczba)-1,'0'),bold|trail2);
}

}
}

char *int_to_str(int val, char *str, int8_t fw, char znak_wiodacy) {
	char *strp = str;
	uint8_t subzero = 0;

	if(val<0) {			// jeœli liczba jest ujemna
		val = ~val+1;	// zaneguj i koryguj
		subzero=1;		// ustaw znacznik na 1
		fw--;
	}

   do{
      div_t divmod = div(val, 10);   	// opracja dzielenia oraz modulo - wynik do struktury ldiv_t
      //*strp++ = divmod.rem + '0';   	// wstawianie cyfr od najmniej znacz¹cej do ³añcucha

      if((val == 0) && (strp != str)) {
         *strp++ = znak_wiodacy;
      } else {
         *strp++ = divmod.rem + '0';
      }


      val = divmod.quot;            	// wartoœæ zmniejsza siê o jednostki, dziesi¹tki, setki itd
      fw--;                     		// zmniejszenie licznika szerokoœci formatowanego pola
      // wykonuj pêtlê do momentu sprawdzenia ostatniej cyfry znacz¹cej lub zajêtoœci ca³ego pola
   } while ( (fw>0)  );

   if(subzero) *strp++ = '-';			// jeœli by³a to liczba ujemna, wstaw znak minus
   *strp = 0;                     		// zakoñcz ³añcuch zerem

   // w zwi¹zku z tym, ¿e w ³añcuchu jest odwrócona kolejnoœæ cyfr
   // wykonaj ich zamianê
   strrev(str);

   // zwróæ wskaŸnik do pocz¹tku ³añucha z liczb¹
   return str;
}

ISR( TIMER0_COMP_vect ) {

	++millis;

}

