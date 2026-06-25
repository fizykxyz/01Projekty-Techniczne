




#include <avr/io.h>
#include <util/delay.h>
#include "ADDS/pins.h"
#include "U74xx595/U74xx595.h"
#include <stdio.h>
#include <avr/interrupt.h>


/*  maksymalna szerokoœæ listingu powinna wynosiæ 80 znaków
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
 */

/*
 * Wielopoziomowe mapowanie wyœwietlaczy (na razie 7/8 segmentowych)
 * 0) Warstwa poœrednia, wirtualna wartoœæ bajtowa (octagonalna), gdzie:
 * 		a) MSB (7bit = kropka (ang.Point) - SegP lub SegH (il. faktycznych s.)
 * 			- (w 16/18 segmentowych LCD konflikt nazwy z prawdziwym segmentem)
 * 		b) LSB (0bit = segment A zawsze SegA i w górê do 6bit=SegG
 * 		czyli wysy³aj¹c rozkaz DISPLAY_xSEG(uint8_t OCTET); (OCTET=0b10011000),
 * 		wyœwietlamy SegD, SegE oraz kropkê, przy aktywnym 1, bo   PGFEDCBA.
 * 		Przydatne bêdzie takie podejœcie bo kropkê dostawiamy 0x80|OCTET,
 * 		a z regu³y 128 znaków to nadto na takie wyœwietlacze
 * 	1) Mapowanie na rzeczywist¹ kolejnoœæ segmentów (np. upsidedown), przy czym
 * 	ka¿dy segment mo¿e byæ pod³¹czony do dowolnego pinu na poziomie cia³a funkcji
 * 	DISPLAY_xSEG(uint8_t OCTET):
 * 		a) pozycja wirt. segmentu w bitach (obecnie zaimplementowane jak w pkt.0):
 *
 * 		enum Virt_seg {vSegA,vSegB,vSegC,vSegD,vSegE,vSegF,vSegG,vSegH};
 * 		enum Real_seg {rSegD,rSegC,rSegH,rSegB,rSegG,rSegA,rSegF,rSegE};
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
 * 		b) póŸniej mo¿na wykonaæ implementacjê mapowanie rzeczywistej kolejnoœci,
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
 * 		uint8_t mapSegVR[8]={rSegA,rSegB,rSegC,rSegD,rSegE,rSegF,rSegG,rSegH},
 * 		ale zu¿ycie ram
 * 		for (uint8_t i=0;i++;i<8)
 * 		{
 * 		if (OCTET & Seg(i)) mapSEG|=Seg(mapSegVR[i]);
 * 		}
 *
 *
 *
 *
 */

#include "fonts/font_7seg.h"


#define DG0 G2O(PA,0)
#define DG1 G2O(PA,1)
#define DG2 G2O(PA,2)
#define DG3 G2O(PA,3)


void SEG7DIG_INIT(void)
{
	OUTPUT(DG0);
	OUTPUT(DG1);
	OUTPUT(DG2);
	OUTPUT(DG3);

}

void SEG7DIG_SEL(uint8_t DIGIT)
{

	if (DIGIT & 1) HIGH(DG0); else LOW(DG0);
	if (DIGIT & 2) HIGH(DG1); else LOW(DG1);
	if (DIGIT & 4) HIGH(DG2); else LOW(DG2);
	if (DIGIT & 8) LOW(DG3); else HIGH(DG3);

}


//
//uint8_t mapSegVR[8]={rSegA,rSegB,rSegC,rSegD,rSegE,rSegF,rSegG,rSegH};
//
//
//#define pSegA G2O(PB,0)
//#define pSegB G2O(PB,1)
//#define pSegC G2O(PB,2)
//#define pSegD G2O(PB,3)
//#define pSegE G2O(PB,4)
//#define pSegF G2O(PB,5)
//#define pSegG G2O(PB,6)
//#define pSegH G2O(PB,7)
//
//#define pDig0 G2O(PC,0)
//
//#define pDig(nr) G2O(PC,(nr))
//
//
//void SEG7_INIT(void)
//{
//	OUTPUT(pSegA);
//	OUTPUT(pSegB);
//	OUTPUT(pSegC);
//	OUTPUT(pSegD);
//	OUTPUT(pSegE);
//	OUTPUT(pSegF);
//	OUTPUT(pSegG);
//	OUTPUT(pSegH);
//	OUTPUT(pDig(0));
//	OUTPUT(pDig(1));
//	OUTPUT(pDig(2));
//	OUTPUT(pDig(3));
//	OUTPUT(pDig(4));
//	OUTPUT(pDig(5));
//	OUTPUT(pDig(6));
//	OUTPUT(pDig(7));
//
//}
//
//void _SEG7_W8BIT(uint8_t OCTET)
//{
//	uint8_t mapSeg=OCTET;
//	if (mapSeg & Seg(vSegA)) HIGH(pSegA); else LOW(pSegA);
//	if (mapSeg & Seg(vSegB)) HIGH(pSegB); else LOW(pSegB);
//	if (mapSeg & Seg(vSegC)) HIGH(pSegC); else LOW(pSegC);
//	if (mapSeg & Seg(vSegD)) HIGH(pSegD); else LOW(pSegD);
//	if (mapSeg & Seg(vSegE)) HIGH(pSegE); else LOW(pSegE);
//	if (mapSeg & Seg(vSegF)) HIGH(pSegF); else LOW(pSegF);
//	if (mapSeg & Seg(vSegG)) HIGH(pSegG); else LOW(pSegG);
//	if (mapSeg & Seg(vSegH)) HIGH(pSegH); else LOW(pSegH);
//
//}
//
//void SEG7_SEGMENT(uint8_t OCTET)
//{
//
//	uint8_t mapSeg=0;
//	for (uint8_t i=0;i<8;i++)
//{
//if (OCTET & Seg(i)) mapSeg|=Seg(mapSegVR[i]);
//}
//
//mapSeg=~mapSeg;
//_SEG7_W8BIT(mapSeg);
//}


//char *napis="PIEC248\x7F\xF0";
char *napis="123.45";
//char  wynik[17];      //bufor LCD
uint16_t ADC_pomiar;

//----- PRZERWANIE Z ADC   -----------------------------------------


void ADC_INIT()
{

	// Initialise the ADC.
	// Select Vref -> AVcc with external
	// capacitor at the AREF pin.
	ADMUX |= _BV(REFS0);

	// Select the prescaler of 128
	ADCSRA |= _BV(ADPS2) | _BV(ADPS1) |
			_BV(ADPS0);
	// Enable the ADC.
	ADCSRA |= _BV(ADEN);

}


uint16_t ADC_READ(uint8_t ADCport)
{

	ADCport=ADCport&0b00000111;
	ADMUX|=ADCport;

	// Start the single conversion mode process.
	ADCSRA |= _BV(ADSC);

	// Wait for the conversion to finish.

	// The ADC signals that it has completed by
	// automatically clearing the ADSC bit.

	// Wait in a while loop until the bit
	// clears.
	while( ADCSRA & _BV(ADSC) );

	return(ADC);
}




int main()
{
	ADC_INIT();
	U74xx595_INIT();
	SEG7DIG_INIT();

	//	SEG7_INIT();
	//    SEG7_SEGMENT(FONT_7SEG[5]);
	//
	//    _delay_ms(900);

	uint8_t digit = 0;
	uint8_t olddigit = 0;


	while (1)
	{


		//napis[0] = "V";
		ADC_pomiar=ADC_READ(7);
		itoa(ADC_pomiar,napis,10);
		if (digit==5)
		{
			U74xx595_W8BIT(~(FONT_7SEG[napis[digit]-32+4]|0x80));
		}
		else
		{
			U74xx595_W8BIT(~(FONT_7SEG[napis[digit]-32+4]));
		}
		SEG7DIG_SEL(digit);
		//U74xx595_W8BIT(~digit);
		//			LOW(pDig(digit));
		olddigit=digit;
		digit++;
		if (digit>8)  digit=0;
		_delay_ms(20);
		//			HIGH(pDig(olddigit));



	}
}

