/////////////////////////////////////////////////////////////////
// LedTutorial.c - plik g³ówny tutoriala LED
//
// Autor: Ravender Kompilator: WinAVR
// Strona: www.wkretak.pl
/////////////////////////////////////////////////////////////////

#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdio.h> 
#include "pins.h" //mój plik nag³ówkowy
#include "font7seg.h"

/************  maksymalna szerokoœæ listingu powinna wynosiæ 80 znaków *********
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

/* zestawy s³ownikowe
         1         2
12345678901234567890123456
ABCDEFGHIJKLMNOPQRSTUVWXYZ
abcdefghijklmnopqrstuvwxyz
AEIOUY
aeiouy
0123456789
ACELNOSZZ
acelnoszz
*/


/*           segmenty 7seg
	  ***A***
	 *       *
	 F       B
	 *       *
	  ***G***
	 *       *
	 E       C
	 *       * _
	  ***D***  *
	  
	    up side:ABCDEFG_
	  down side:DEFABCG_
	  
	  todo: dorobiæ lustrzane odbicie do np. HUD
*/

/*
 * Wielopoziomowe mapowanie wyœwietlaczy (na razie 7/8 segmentowych)
 * 0) Warstwa poœrednia, wirtualna wartoœæ bajtowa (octagonalna), gdzie:
 * 		a) MSB (7bit = kropka (ang.Point) - Seg_ lub SegP/SegH (il. fakt. s)
 * 		- (w 16/18 segmentowych LCD konflikt nazwy z prawdziwym segmentem)
 * 		b) LSB (0bit = segment A zawsze SegA i w górê do 6bit=SegG,
 * 		czyli wysy³aj¹c rozkaz DISPLAY_xSEG(uint8_t OCTET); (OCTET=0b10011000),
 * 		wyœwietlamy SegD, SegE oraz kropkê, przy aktywnym 1, bo   _GFEDCBA.
 * 		Przydatne bêdzie takie podejœcie bo kropkê dostawiamy 0x80|OCTET,
 * 		a z regu³y 128 znaków to nadto na takie wyœwietlacze
 * 1) Mapowanie na rzeczywist¹ kolejnoœæ segmentów (np. up/down-side), przy czym
 * ka¿dy segment mo¿e byæ pod³¹czony do dowolnego pinu na poziomie cia³a funkcji
 * DISPLAY_xSEG(uint8_t OCTET):
 * 		a) pozycja wirt. segmentu w bitach (teraz zaimplementowane jak w pkt.0):
 *        enum Virt_seg {vSegD,vSegE,vSegF,vSegA,vSegB,vSegC,vSegG,vSeg_};
 * 		enum Virt_seg {vSegA,vSegB,vSegC,vSegD,vSegE,vSegF,vSegG,vSeg_};
 * 		enum Real_seg {rSegD,rSegC,rSeg_,rSegB,rSegG,rSegA,rSegF,rSegE};
 *
 * 		#define Seg(W_segment) (1<<(W_segment))
 *
 * 		//zastêpuje:
 *
 * 		//#define SegA (1<<0)
 * 		//#define SegB (1<<1)
 * 		//#define SegC (1<<2)
 * 		//#define SegD (1<<3)
 * 		//#define SegE (1<<4)
 * 		//#define SegF (1<<5)
 * 		//#define SegG (1<<6)
 * 		//#define SegH (1<<7) // tu kropka
 *
 * 		b) póŸniej mo¿na wykonaæ implementacjê mapowanie rzeczywistej kolejnoœci
 * 		co skomplikuje i zwolni funkcjê void DISPLAY_xSEG(uint8_t OCTET):
 *
 * 		#define rSegA (1<<5) //realSegmentA
 * 		#define rSegB (1<<3)
 * 		#define rSegC (1<<1)
 * 		#define rSegD (1<<0)
 * 		#define rSegE (1<<7)
 * 		#define rSegF (1<<6)
 * 		#define rSegG (1<<4)
 * 		#define rSegH (1<<2) // tu kropka
 * 		uint8_t mapSEG=0;
 * 		if (OCTET & segA) mapSEG|=rSegA
 * 		if (OCTET & segB) mapSEG|=rSegB
 *
 * 		//itd.nic nie da tablicowoœæ, tsia by by³o zak³adaæ prawdziw¹ tablicê
 * 		uint8_t mapSecVR[8]={rSegA,rSegB,rSegC,rSegD,rSegE,rSegF,rSegG,rSetH},
 * 		ale zu¿ycie ram
 * 		for (uint8_t i=0;i++;i<8)
 * 		{
 * 		if (OCTET & Seg(i) mapSEG|=Seg(mapSecVR[i])
 * 		}
 *
 *
 *
 *  #define SegA G2O(PA,7)
 *  #define SegB G2O(PA,7)
 *  #define SegC G2O(PA,7)
 *  #define SegD G2O(PA,7)
 *  #define SegE G2O(PA,7)
 *  #define SegF G2O(PA,7)
 *  #define SegG G2O(PA,7)
 *  uint8_t segPins[8]=[G2O(PA,7),G2O(PA,7),G2O(PA,7),G2O(PA,7),G2O(PA,7) ]
 */

enum Virt_seg {vSegA,vSegB,vSegC,vSegD,vSegE,vSegF,vSegG,vSeg_};
enum Real_seg {rSegD,rSegC,rSeg_,rSegB,rSegG,rSegA,rSegF,rSegE};

#define SegA P(B,0)
#define SegB P(B,1)
#define SegC P(B,2)
#define SegD P(B,3)
#define SegE P(B,4)
#define SegF P(B,5)
#define SegG P(B,6)
#define SegH P(B,7)


#define Dig1 P(D,0)
#define Dig2 P(D,1)
#define Dig3 P(D,2)
#define Dig4 P(D,3)
#define Dig5  P(D,4)
#define Dig6  P(D,5)
#define Dig7  P(D,6)
#define Dig8  P(D,7)


const uint8_t segPins[]={SegA, SegB, SegC, SegD, SegE, SegF, SegG, SegH};

const uint8_t digPins[]={Dig1, Dig2, Dig3, Dig4, Dig5, Dig6, Dig7, Dig8 };

#define digActive 0
#define segActive (!digActive)



volatile uint8_t wysw=sizeof digPins; // zmienna oznaczaj¹ca numer aktywnego wyœwietlacza
uint8_t liczba = 0;

//const uint8_t segm[]={0,1,2,3};
const uint8_t segm[]={7,0,1,2,3,4,5,6};
volatile uint16_t millis;
char napis[] ="PIEC7*C";

void transform(char* slowo, int czas)
{
    sprintf(napis,"%s",slowo);
  _delay_ms(czas);
}




int main(void)
{


uint8_t i;
for (i=0;i<sizeof digPins;i++)
    {
//DDRD |= (1<<segm[i]); //ustawiena portów segmentów równie¿ jako wyjœcia
OUTPUT(digPins[i]);PUT(digPins[i],!digActive);
    }
    
    for (i=0;i<sizeof segPins;i++)
    {
//DDRD |= (1<<segm[i]); //ustawiena portów segmentów równie¿ jako wyjœcia
OUTPUT(segPins[i]);PUT(segPins[i],segActive);
    }
// timer0 - konfig
	TCCR0 |= (1<<CS02);//|(1<<CS00); //prescaler = 1024
	TIMSK |= (1<<TOIE0); // zezwolenie na przerwanie OVF

	

//_delay_ms(300);
sei(); // Zezwolenie globalne na przerwania
    
   
sprintf(napis,"%s","ABCDEFGH");
_delay_ms(2000);
    transform("IJKLMNO",2000);
//napis="IJKLMNO";
//_delay_ms(1000);
//napis="QRSTUVWZ";
//_delay_ms(1000);
//napis="Z0123456";
//_delay_ms(1000);
//napis="789-+=.,";
//_delay_ms(1000);
    
while(1)
{
	//liczba =(millis/10)%10000;
//dziesiatki = liczba / 10;
//jednosci = liczba % 10;
//_delay_ms(500); // przerwa miêdzy liczeniem
//if (liczba == 99) {liczba = 0;} else {liczba++;} //jeœli doliczy do 99 to licznik siê zeruje

	sprintf(napis,"%08u",(millis));

}

return 0;
}


ISR( TIMER0_OVF_vect ) {
	TCNT0 = 256-78; // czêstotliwoœæ 100 Hz
	++millis;
    uint8_t i,segSet;
	
    PUT(digPins[wysw],!digActive); //HIGH(digPins[i]); //wy³¹cza poprzedni znak
   
    
   
        //  PUT(digPins[wysw],digActive); //LOW(digPins[i]); //w³¹cza aktualny
    wysw++;
	if (wysw > (sizeof segm)) wysw=0;
   
    for (i=0;i<sizeof segPins;i++)
        {

		
			
            if ((napis[wysw]<42) && (napis[wysw]>122))
            {
            segSet = !segActive;
            }
            else
            {
			segSet = !!(pgm_read_byte(&znak7[napis[wysw]-42])&(1<<i))-digActive;
            }
            PUT(segPins[i],segSet); //LOW(digPins[i]); //PORTD |= (1 << segm[i]); //zeruje bit 0 portu D - wy³¹cza wyœwietlacz nr 1    
            //PORTB = pgm_read_byte(&znak7[napis[wysw]-42]);}
        }
	
    
    
    
    
    
    /*
     if ((napis[wysw]<42) && (napis[wysw]>122))
        {
            PORTB = 255;
        }
            else
        {
			PORTB = pgm_read_byte(&znak7[napis[wysw]-42]);
		}
        
    */
     PUT(digPins[wysw],digActive); //LOW(digPins[i]); //w³¹cza aktualny
        //  _delay_ms(100);  
	}

