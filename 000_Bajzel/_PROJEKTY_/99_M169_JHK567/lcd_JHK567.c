/*================================================================
==================================================================
=	Biblioteka obslugi LCD JHK567 dla AVR ATmega169
=	Autor: Filip Rus
=	wersja = 1.0
=	Copywright - LiveLights
==================================================================	
================================================================*/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "lcd_JHK567.h"
//#include "ATmega169_LCD_test.h"
//#include "uart.h"



//================================================================
//===== Inicjalizacja LCD ========================================
//================================================================
void LCD567_init(void)
{
	LCDCRB = (1<<LCD2B) | (1<<LCDMUX0) | (1<<LCDMUX1) | (1<<LCDPM2);												// system clock, 1/3 bias,  1/4 duty, 21 segmentow -> SEG21:SEG24 jako zwykle IO
	
	LCDFRR = (1<<LCDPS2) | (1<<LCDPS1) | (1<<LCDPS0) | (1<<LCDCD2) | (1<<LCDCD1);					// preskaler 4096, podzial przez 7 @ duty 1/4 => 11059200Hz / (4096 * 8 * 8) = 48.2Hz
	
	LCDCCR = (1<<LCDDC1) | (1<<LCDCC3);																// segment drive time 150us, napiecie 3.0 V
	
	LCDCRA = (1<<LCDEN);																			// wlacz LCD, bez przerwania
}
//================================================================



//================================================================
//===== Obsluga LCD ==============================================
//================================================================
// wlacza lub wylacza wybrany segment (1=ON, 0=OFF)
void LCD567_segment(unsigned char segment, unsigned char on_off)
{
	unsigned char* lcd_data_reg;																	// wskaznik na adres wybranego rejestru danych LCDDRx
	unsigned char bit_n;

	lcd_data_reg = LCD_DR_BASE + (segment >> 3);													// wyluskanie adresu rejestru LCDDRx (adres LCDDR0 + (nr segmentu=bitu / 8))
	bit_n = segment & 0x07;																			// wyluskanie numeru bitu w rejestrze LCDDRx

	if(on_off != 0)																					// jezeli segment ma byc wlaczony
		*lcd_data_reg |= 1 << bit_n;																// zapisz 1 do odpowiedniego bitu rejestru LCDDRx
	else																							// w przeciwnym wypadku
		*lcd_data_reg &=~(1 << bit_n);																// zapisz 0 do odpowiedniego bitu rejestru LCDDRx
}


// wyswietla cyfre "digit" na wybranej pozycji "position"
void LCD567_digit(unsigned char digit, unsigned char position)
{
	unsigned char x1, seg_adr, seg_on_off;

	if(position > 8)																				// ograniczenie do 9 pozycji
		position = 8;

	if(digit < 10)																					// ograniczenie do 10 cyfr 0-9
	{
		for(x1=0;x1<7;x1++)																			// dla 7 segmentow
		{
			seg_adr = PGM(&lcd_7seg_disp[(position * 7) + x1]);										// adres segmentu na wybranej pozycji
			seg_on_off = (PGM(&lcd_7seg_dig[digit]) >> x1) & 0x01;									// segment wlaczony/wylaczony zgodnie z wygladem wybranej cyfry

			LCD567_segment(seg_adr, seg_on_off);													// wlacz/wylacz wybrany segment LCD
		}
	}
	else																							// powyzej 9 czysc cala pozycje
	{
		for(x1=0;x1<7;x1++)																			// dla 7 segmentow
		{
			seg_adr = PGM(&lcd_7seg_disp[(position * 7) + x1]);										// adres segmentu na wybranej pozycji
			LCD567_segment(seg_adr, 0);																// wylacz wybrany segment LCD
		}
	}
}


// wyswietla podana liczbe z zakresu <-999999,9999999>
void LCD567_number(long number)
{
	unsigned char cyfra;
	unsigned char znak, not_zero;

	znak = 0;																						// domyslnie liczba dodatnia
	not_zero = 0;																					// domyslnie 0 na pozycji wiodocej
	LCD567_segment(SEG_E,0);																		// zgas segment E = dom,yslnie brak bledu

	if(number > 9999999)																			// ograniczenie zakresu do 7 cyfr
	{
		number = 9999999;
		LCD567_segment(SEG_E,1);																	// zapal segment E = blad
	}

	if(number < 0)																					// jezeli liczba ujemna
	{
		number = (number ^ 0xFFFFFFFF) + 1;															// wartosc bezwzgledna (konwersja z ujemnej na dodatnia w kodzie U2)

		if(number > 999999)																			// ograniczenie do 6 cyfr + znak '-'
		{
			number = 999999;
			LCD567_segment(SEG_E,1);																// zapal segment E = blad
		}

		znak = SEG_6G;																				// flaga dodania znaku '-'
	}

	// === wyluskiwanie poszczegolnych cyfr ===
	cyfra = number / 1000000;																		// liczba czesci 1mln
	number = number % 1000000;																		// pomniejsz o liczbe czesci 1mln
	if(cyfra != 0)																					// bez zer wiodacych
	{
		LCD567_digit(cyfra,6);																		// wyswietl czesci 1mln
		not_zero = 1;																				// flaga nie usuwania wystepujacych dalej zer
	}
	else																							// jezeli zero wiodace
	{
		if(znak != 0)																				// jezeli liczba ujemna
			znak = SEG_6G;																			// flaga dodania znaku '-' na pozycji 6

		LCD567_digit(_7SEG_CLEAR,6);																// wyczysc pozycje 6
	}

	cyfra = number / 100000;																		// liczba czesci 100k
	number = number % 100000;																		// pomniejsz o liczbe czesci 100k
	if((cyfra != 0)	|| (not_zero != 0))																// bez zer wiodacych
	{
		LCD567_digit(cyfra,5);																		// wyswietl czesci 100k
		not_zero = 1;																				// flaga nie usuwania wystepujacych dalej zer
	}
	else																							// jezeli zero wiodace
	{
		if(znak != 0)																				// jezeli liczba ujemna
			znak = SEG_5G;																			// flaga dodania znaku '-' na pozycji 5

		LCD567_digit(_7SEG_CLEAR,5);																// wyczysc pozycje 5
	}

	cyfra = number / 10000;																			// liczba czesci 10k
	number = number % 10000;																		// pomniejsz o liczbe czesci 10k
	if((cyfra != 0)	|| (not_zero != 0))																// bez zer wiodacych
	{
		LCD567_digit(cyfra,4);																		// wyswietl czesci 10k
		not_zero = 1;																				// flaga nie usuwania wystepujacych dalej zer
	}
	else																							// jezeli zero wiodace
	{
		if(znak != 0)																				// jezeli liczba ujemna
			znak = SEG_4G;																			// flaga dodania znaku '-' na pozycji 4

		LCD567_digit(_7SEG_CLEAR,4);																// wyczysc pozycje 4
	}

	cyfra = number / 1000;																			// liczba czesci 1k
	number = number % 1000;																			// pomniejsz o liczbe czesci 1k
	if((cyfra != 0)	|| (not_zero != 0))																// bez zer wiodacych
	{
		LCD567_digit(cyfra,3);																		// wyswietl czesci 1k
		not_zero = 1;																				// flaga nie usuwania wystepujacych dalej zer
	}
	else																							// jezeli zero wiodace
	{
		if(znak != 0)																				// jezeli liczba ujemna
			znak = SEG_3G;																			// flaga dodania znaku '-' na pozycji 3

		LCD567_digit(_7SEG_CLEAR,3);																// wyczysc pozycje 3
	}
	
	cyfra = number / 100;																			// liczba czesci 100
	number = number % 100;																			// pomniejsz o liczbe czesci 100
	if((cyfra != 0)	|| (not_zero != 0))																// bez zer wiodacych
	{
		LCD567_digit(cyfra,2);																		// wyswietl czesci 100
		not_zero = 1;																				// flaga nie usuwania wystepujacych dalej zer
	}
	else																							// jezeli zero wiodace
	{
		if(znak != 0)																				// jezeli liczba ujemna
			znak = SEG_2G;																			// flaga dodania znaku '-' na pozycji 2

		LCD567_digit(_7SEG_CLEAR,2);																// wyczysc pozycje 2
	}
	
	cyfra = number / 10;																			// liczba czesci 10
	number = number % 10;																			// pomniejsz o liczbe czesci 10
	if((cyfra != 0)	|| (not_zero != 0))																// bez zer wiodacych
	{
		LCD567_digit(cyfra,1);																		// wyswietl czesci 10
	}
	else																							// jezeli zero wiodace
	{
		if(znak != 0)																				// jezeli liczba ujemna
			znak = SEG_1G;																			// flaga dodania znaku '-' na pozycji 1

		LCD567_digit(_7SEG_CLEAR,1);																// wyczysc pozycje 1
	}

	LCD567_digit(number,0);																			// wyswietl jednosci (bez usuwania zera)
	
	if(znak != 0)																					// jezeli liczba ujemna
		LCD567_segment(znak,1);																		// wyswietl znak na odpowiednim miejscu
}


// wyswietla podana liczbe z zakresu <0,99> na malym wyswietlaczy 7seg po lewej stronie
void LCD567_small_number(unsigned char number)
{
	unsigned char cyfra;

	if(number > 99)																					// ograniczenie zakresu do 2 cyfr
		number = 99;

	cyfra = number / 10;																			// liczba czesci 10
	number = number % 10;																			// pomniejsz o liczbe czesci 10
	if(cyfra != 0)																					// bez zer wiodacych
		LCD567_digit(cyfra,8);																		// wyswietl czesci 10

	LCD567_digit(number,7);																			// wyswietl jednosci (bez usuwania zera)
}
//================================================================

