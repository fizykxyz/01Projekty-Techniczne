#ifndef ADDS_SEGxx_H_
#define ADDS_SEGxx_H_

/*
 * 000_XYZ_AVRLIBS pins.h
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 2016-08-09   
 *     Edycja: 08:57:02
 */

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



//#include <avr/io.h>
//#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdio.h> 


#include "pins.h"
#include "font7seg.h"


enum Virt_seg {vSegA,vSegB,vSegC,vSegD,vSegE,vSegF,vSegG,vSeg_};
enum Real_seg {rSegD,rSegC,rSeg_,rSegB,rSegG,rSegA,rSegF,rSegE};

#define SegA P(B,0)
#define SegB P(B,1)
#define SegC P(B,2)
#define SegD P(B,3)
#define SegE P(B,4)
#define SegF P(B,5)
#define SegG P(B,6)
#define Seg_ P(B,7)


#define Dig1 P(D,0)
#define Dig2 P(D,1)
#define Dig3 P(D,2)
#define Dig4 P(D,3)
#define Dig5  P(D,4)
#define Dig6  P(D,5)
#define Dig7  P(D,6)
#define Dig8  P(D,7)

#define digActive 0
#define segActive (!digActive)

#define digNumbers 8


const uint8_t segPins[]={SegA, SegB, SegC, SegD, SegE, SegF, SegG, Seg_};

const uint8_t digPins[]={Dig1, Dig2, Dig3, Dig4, Dig5, Dig6, Dig7, Dig8 };




volatile uint8_t digNow = 0;
volatile uint8_t digMax = sizeof digPins; // zmienne oznaczaj¹ce numer aktywnego i max wyœwietlacza
char digBuf[digNumbers] ;

void initSEGxx()
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
}

void updSEGxx()
{
    
    uint8_t i,segSet;
	
    PUT(digPins[digNow],!digActive); //HIGH(digPins[i]); //wy³¹cza poprzedni znak
   
    
   
        //  PUT(digPins[wysw],digActive); //LOW(digPins[i]); //w³¹cza aktualny
    digNow++;
	if (digNow > digMax) digNow=0;
   
    for (i=0;i<sizeof segPins;i++)
        {

		
			
            if ((digBuf[digNow]<startCode) && (digBuf[digNow]>127))
            {
            segSet = !segActive;
            }
            else
            {
			segSet = !!(pgm_read_byte(&font7seg[digBuf[digNow]-startCode])&(1<<i))-digActive;
            }
            PUT(segPins[i],segSet); //LOW(digPins[i]); //PORTD |= (1 << segm[i]); //zeruje bit 0 portu D - wy³¹cza wyœwietlacz nr 1    
            //PORTB = pgm_read_byte(&znak7[napis[wysw]-42]);}
        }
	
    PUT(digPins[digNow],digActive); //LOW(digPins[i]); //w³¹cza aktualny
}

#endif /* ADDS_SEGxx_H_ */


