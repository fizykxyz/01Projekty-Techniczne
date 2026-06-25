/*================================================================
==================================================================
=	Definicje dla biblioteki obslugi LCD JHK567 dla AVR ATmega169
=	Autor: Filip Rus
=	wersja = 1.0
=	Copywright - LiveLights
==================================================================	
================================================================*/
#ifndef _AVR_LCD_JHK567_H_
#define _AVR_LCD_JHK567_H_

#include <avr/pgmspace.h>


#define PGM pgm_read_byte_near			// skrotowa nazwa funkcji dostepu do pamieci kodu

#define LCD_DR_BASE			0xEC		// adres podstawowy rejestrow danych LCD (DR0=0xEC - DR18=0xFE)


// === Definicje segmentow LCD zgodne z mapowaniem w MCU (numer bitu w rejestrach LCDDRx) ===

#define SEG_8K				0			// segment / 7seg nr 8 (maly lewy)
#define SEG_8F				40			// segment F 7seg nr 8 (maly lewy)
#define SEG_8D				80			// segment D 7seg nr 8 (maly lewy)
#define SEG_8E				120			// segment E 7seg nr 8 (maly lewy)
#define SEG_8A				1			// segment A 7seg nr 8 (maly lewy)
#define SEG_8B				41			// segment B 7seg nr 8 (maly lewy)
#define SEG_8C				81			// segment C 7seg nr 8 (maly lewy)
#define SEG_8G				121			// segment G 7seg nr 8 (maly lewy)
#define SEG_8J				2			// segment J 7seg nr 8 (maly lewy)

#define SEG_7F				42			// segment F 7seg nr 7 (maly prawy)
#define SEG_7D				82			// segment D 7seg nr 7 (maly prawy)
#define SEG_7E				122			// segment E 7seg nr 7 (maly prawy)
#define SEG_7A				3			// segment A 7seg nr 7 (maly prawy)
#define SEG_7B				43			// segment B 7seg nr 7 (maly prawy)
#define SEG_7C				83			// segment C 7seg nr 7 (maly prawy)
#define SEG_7G				123			// segment G 7seg nr 7 (maly prawy)

#define SEG_E				4			// E
#define SEG_T1				44			// termometr zape³niony (dodatni)
#define SEG_T3				84			// kran
#define SEG_T2				124			// termometr pusty z minusem (ujemny)
#define SEG_H5				5			// kreski nad i pod 7seg nr 1

#define SEG_6F				45			// segment F 7seg nr 6
#define SEG_6D				85			// segment D 7seg nr 6
#define SEG_6E				125			// segment E 7seg nr 6 
#define SEG_6A				6			// segment A 7seg nr 6
#define SEG_6B				46			// segment B 7seg nr 6
#define SEG_6C				86			// segment C 7seg nr 6
#define SEG_6G				126			// segment G 7seg nr 6

#define SEG_H4				7			// kreski nad i pod 7seg nr 0

#define SEG_5F				47			// segment F 7seg nr 5
#define SEG_5D				87			// segment D 7seg nr 5
#define SEG_5E				127			// segment E 7seg nr 5
#define SEG_5A				8			// segment A 7seg nr 5
#define SEG_5B				48			// segment B 7seg nr 5
#define SEG_5C				88			// segment C 7seg nr 5
#define SEG_5G				128			// segment G 7seg nr 5

#define SEG_H3				9			// kropka pomiêdzy 7seg nr 3 i 4

#define SEG_4F				49			// segment F 7seg nr 4
#define SEG_4D				89			// segment D 7seg nr 4
#define SEG_4E				129			// segment E 7seg nr 4
#define SEG_4A				10			// segment A 7seg nr 4
#define SEG_4B				50			// segment B 7seg nr 4
#define SEG_4C				90			// segment C 7seg nr 4
#define SEG_4G				130			// segment G 7seg nr 4

#define SEG_H2				11			// kreski nad i pod 7seg nr 2, kropka pomiêdzy 7seg nr 2 i 3

#define SEG_3F				51			// segment F 7seg nr 3
#define SEG_3D				91			// segment D 7seg nr 3
#define SEG_3E				131			// segment E 7seg nr 3
#define SEG_3A				12			// segment A 7seg nr 3
#define SEG_3B				52			// segment B 7seg nr 3
#define SEG_3C				92			// segment C 7seg nr 3
#define SEG_3G				132			// segment G 7seg nr 3

#define SEG_H1				13			// kropka pomiêdzy 7seg nr 1 i 2

#define SEG_2F				53			// segment F 7seg nr 2
#define SEG_2D				93			// segment D 7seg nr 2
#define SEG_2E				133			// segment E 7seg nr 2
#define SEG_2A				14			// segment A 7seg nr 2
#define SEG_2B				54			// segment B 7seg nr 2
#define SEG_2C				94			// segment C 7seg nr 2
#define SEG_2G				134			// segment G 7seg nr 2

#define SEG_H0				15			// kropka pomiêdzy 7seg nr 0 i 1

#define SEG_1F				55			// segment F 7seg nr 1
#define SEG_1D				95			// segment D 7seg nr 1
#define SEG_1E				135			// segment E 7seg nr 1
#define SEG_1A				16			// segment A 7seg nr 1
#define SEG_1B				56			// segment B 7seg nr 1
#define SEG_1C				96			// segment C 7seg nr 1
#define SEG_1G				136			// segment G 7seg nr 1

#define SEG_C				17			// stopnie C

#define SEG_0F				57			// segment F 7seg nr 0
#define SEG_0D				97			// segment D 7seg nr 0
#define SEG_0E				137			// segment E 7seg nr 0
#define SEG_0A				18			// segment A 7seg nr 0
#define SEG_0B				58			// segment B 7seg nr 0
#define SEG_0C				98			// segment C 7seg nr 0
#define SEG_0G				138			// segment G 7seg nr 0

#define SEG_M3				19			// m3 (metry szescienne)
#define SEG_L				59			// l (litry)
#define SEG_K				99			// k (kilo)
#define SEG_W				139			// W (waty)
#define SEG_H				20			// /h ("na godzine")
#define SEG_M				60			// M (mega)
#define SEG_G				100			// G (giga)
#define SEG_J				140			// J (dzule)

// ==========================================================================================



// === Definicje dla cyfr 7seg => koncepcja wybrana po wielu przemysleniach =================

#define _7SEG_A				(1<<0)		// przypisanie nr bitu do nazwy segmentu
#define _7SEG_B				(1<<1)
#define _7SEG_C				(1<<2)
#define _7SEG_D				(1<<3)
#define _7SEG_E				(1<<4)
#define _7SEG_F				(1<<5)
#define _7SEG_G				(1<<6)

#define _7SEG_CLEAR			0xFF		// wartosc wymuszajaca czyszczenie 7seg

// tablica z wygladem cyfr 7seg
const static unsigned char lcd_7seg_dig[10] PROGMEM =
{
	_7SEG_A + _7SEG_B + _7SEG_C  +_7SEG_D + _7SEG_E + _7SEG_F,					// 0
	_7SEG_B + _7SEG_C,															// 1
	_7SEG_A + _7SEG_B + _7SEG_D + _7SEG_E + _7SEG_G,							// 2
	_7SEG_A + _7SEG_B + _7SEG_C  +_7SEG_D + _7SEG_G,							// 3
	_7SEG_B + _7SEG_C + _7SEG_F + _7SEG_G,										// 4
	_7SEG_A + _7SEG_C + _7SEG_D + _7SEG_F + _7SEG_G,							// 5
	_7SEG_A + _7SEG_C + _7SEG_D + _7SEG_E + _7SEG_F + _7SEG_G,					// 6
	_7SEG_A + _7SEG_B + _7SEG_C,												// 7
	_7SEG_A + _7SEG_B + _7SEG_C + _7SEG_D + _7SEG_E + _7SEG_F + _7SEG_G,		// 8
	_7SEG_A + _7SEG_B + _7SEG_C + _7SEG_D + _7SEG_F + _7SEG_G,					// 9
};

// tablica z adresami segmentow dla poszczegolnych pol 7seg (po przemysleniu ten sposob bedzie prawdopodobnie najwydajniejszy i najprostszy w uzyciu a takze elastyczny)
const static unsigned char lcd_7seg_disp[7*9] PROGMEM =
{
	SEG_0A, SEG_0B, SEG_0C, SEG_0D, SEG_0E, SEG_0F, SEG_0G,						// disp 0
	SEG_1A, SEG_1B, SEG_1C, SEG_1D, SEG_1E, SEG_1F, SEG_1G,						// disp 1
	SEG_2A, SEG_2B, SEG_2C, SEG_2D, SEG_2E, SEG_2F, SEG_2G,						// disp 2
	SEG_3A, SEG_3B, SEG_3C, SEG_3D, SEG_3E, SEG_3F, SEG_3G,						// disp 3
	SEG_4A, SEG_4B, SEG_4C, SEG_4D, SEG_4E, SEG_4F, SEG_4G,						// disp 4
	SEG_5A, SEG_5B, SEG_5C, SEG_5D, SEG_5E, SEG_5F, SEG_5G,						// disp 5
	SEG_6A, SEG_6B, SEG_6C, SEG_6D, SEG_6E, SEG_6F, SEG_6G,						// disp 6
	SEG_7A, SEG_7B, SEG_7C, SEG_7D, SEG_7E, SEG_7F, SEG_7G,						// disp 7
	SEG_8A, SEG_8B, SEG_8C, SEG_8D, SEG_8E, SEG_8F, SEG_8G						// disp 8
};

// ==========================================================================================



void LCD567_init(void);
void LCD567_digit(unsigned char, unsigned char);
void LCD567_segment(unsigned char, unsigned char);
void LCD567_number(long);
void LCD567_small_number(unsigned char);


#endif


/*
#define _7SEG_A				SEG_8A				// adresy bazowe dla poszczegolnych segmentow wyswietlacza 7seg
#define _7SEG_B				SEG_8B
#define _7SEG_C				SEG_8C
#define _7SEG_D				SEG_8D
#define _7SEG_E				SEG_8E
#define _7SEG_F				SEG_8F
#define _7SEG_G				SEG_8G

#define _7SEG_0				SEG_0A-_7SEG_A		// ofsety dla poszczegolnych wyswietlaczy 7seg
#define _7SEG_1				SEG_1A-_7SEG_A
#define _7SEG_2				SEG_2A-_7SEG_A
#define _7SEG_3				SEG_3A-_7SEG_A
#define _7SEG_4				SEG_4A-_7SEG_A
#define _7SEG_5				SEG_5A-_7SEG_A
#define _7SEG_6				SEG_6A-_7SEG_A
#define _7SEG_7				SEG_7A-_7SEG_A
#define _7SEG_8				SEG_8A-_7SEG_A
*/

/*
// === Definicje segmentow LCD zgodne z mapowaniem w MCU ===

#define SEG_8K				0			// segment / 7seg nr 8 (maly lewy)
#define SEG_8F				100			// segment F 7seg nr 8 (maly lewy)
#define SEG_8D				200			// segment D 7seg nr 8 (maly lewy)
#define SEG_8E				300			// segment E 7seg nr 8 (maly lewy)
#define SEG_8A				1			// segment A 7seg nr 8 (maly lewy)
#define SEG_8B				101			// segment B 7seg nr 8 (maly lewy)
#define SEG_8C				201			// segment C 7seg nr 8 (maly lewy)
#define SEG_8G				301			// segment G 7seg nr 8 (maly lewy)
#define SEG_8J				2			// segment J 7seg nr 8 (maly lewy)

#define SEG_7F				102			// segment F 7seg nr 7 (maly prawy)
#define SEG_7D				202			// segment D 7seg nr 7 (maly prawy)
#define SEG_7E				302			// segment E 7seg nr 7 (maly prawy)
#define SEG_7A				3			// segment A 7seg nr 7 (maly prawy)
#define SEG_7B				103			// segment B 7seg nr 7 (maly prawy)
#define SEG_7C				203			// segment C 7seg nr 7 (maly prawy)
#define SEG_7G				303			// segment G 7seg nr 7 (maly prawy)

#define SEG_E				4			// E
#define SEG_T1				104			// termometr zape³niony (dodatni)
#define SEG_T3				204			// kran
#define SEG_T2				304			// termometr pusty z minusem (ujemny)
#define SEG_H5				5			// kreski nad i pod 7seg nr 1

#define SEG_6F				105			// segment F 7seg nr 6
#define SEG_6D				205			// segment D 7seg nr 6
#define SEG_6E				305			// segment E 7seg nr 6 
#define SEG_6A				6			// segment A 7seg nr 6
#define SEG_6B				106			// segment B 7seg nr 6
#define SEG_6C				206			// segment C 7seg nr 6
#define SEG_6G				306			// segment G 7seg nr 6

#define SEG_H4				7			// kreski nad i pod 7seg nr 0

#define SEG_5F				107			// segment F 7seg nr 5
#define SEG_5D				207			// segment D 7seg nr 5
#define SEG_5E				307			// segment E 7seg nr 5
#define SEG_5A				8			// segment A 7seg nr 5
#define SEG_5B				108			// segment B 7seg nr 5
#define SEG_5C				208			// segment C 7seg nr 5
#define SEG_5G				308			// segment G 7seg nr 5

#define SEG_H3				9			// kropka pomiêdzy 7seg nr 3 i 4

#define SEG_4F				109			// segment F 7seg nr 4
#define SEG_4D				209			// segment D 7seg nr 4
#define SEG_4E				309			// segment E 7seg nr 4
#define SEG_4A				10			// segment A 7seg nr 4
#define SEG_4B				110			// segment B 7seg nr 4
#define SEG_4C				210			// segment C 7seg nr 4
#define SEG_4G				310			// segment G 7seg nr 4

#define SEG_H2				11			// kreski nad i pod 7seg nr 2, kropka pomiêdzy 7seg nr 2 i 3

#define SEG_3F				111			// segment F 7seg nr 3
#define SEG_3D				211			// segment D 7seg nr 3
#define SEG_3E				311			// segment E 7seg nr 3
#define SEG_3A				12			// segment A 7seg nr 3
#define SEG_3B				112			// segment B 7seg nr 3
#define SEG_3C				212			// segment C 7seg nr 3
#define SEG_3G				312			// segment G 7seg nr 3

#define SEG_H1				13			// kropka pomiêdzy 7seg nr 1 i 2

#define SEG_2F				113			// segment F 7seg nr 2
#define SEG_2D				213			// segment D 7seg nr 2
#define SEG_2E				313			// segment E 7seg nr 2
#define SEG_2A				14			// segment A 7seg nr 2
#define SEG_2B				114			// segment B 7seg nr 2
#define SEG_2C				214			// segment C 7seg nr 2
#define SEG_2G				314			// segment G 7seg nr 2

#define SEG_H0				15			// kropka pomiêdzy 7seg nr 0 i 1

#define SEG_1F				115			// segment F 7seg nr 1
#define SEG_1D				215			// segment D 7seg nr 1
#define SEG_1E				315			// segment E 7seg nr 1
#define SEG_1A				16			// segment A 7seg nr 1
#define SEG_1B				116			// segment B 7seg nr 1
#define SEG_1C				216			// segment C 7seg nr 1
#define SEG_1G				316			// segment G 7seg nr 1

#define SEG_C				17			// stopnie C

#define SEG_0F				117			// segment F 7seg nr 0
#define SEG_0D				217			// segment D 7seg nr 0
#define SEG_0E				317			// segment E 7seg nr 0
#define SEG_0A				18			// segment A 7seg nr 0
#define SEG_0B				118			// segment B 7seg nr 0
#define SEG_0C				218			// segment C 7seg nr 0
#define SEG_0G				318			// segment G 7seg nr 0

#define SEG_M3				19			// m3 (metry szescienne)
#define SEG_L				119			// l (litry)
#define SEG_K				219			// k (kilo)
#define SEG_W				319			// W (waty)
#define SEG_H				20			// /h ("na godzine")
#define SEG_M				120			// M (mega)
#define SEG_G				220			// G (giga)
#define SEG_J				320			// J (dzule)

// =========================================================
*/
