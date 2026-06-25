/*================================================================
==================================================================
=	Plik obs³ugi interfejsu USART
=	CPU = ATMega169
=	Copywright - LiveLights
==================================================================	
================================================================*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "ATmega169_LCD_test.h"
#include <util/delay.h>
#include "uart.h"



//================================================================
//===== Obsluga przerwania od RX UART ===========================
//================================================================
/*ISR(USART_RXC_vect)
{
	unsigned char znak;

	znak = UDR;														// odczytaj odebrany znak
	//trans = 0;														// czysc flage transmisji na kosmiczny przypadek

	uart_char(znak);													// echo			
}*/
//================================================================



//================================================================
//===== Inicjalizacja USART1 =====================================
//================================================================
void uart_init(unsigned long baudrate)
{
	unsigned int dzielnik=0;

	// obliczenia dla double speed -> U2X0 = 1 
	dzielnik = (unsigned int)((OSC / (baudrate << 3)) - 1);

	UBRRH = (dzielnik >> 8) & 0x0F;					// 8(4) starczych bitow dzielnika       
  	UBRRL = (unsigned char)dzielnik;				// 8 mlodszych bitow dzielnika

  	UCSRA = 0x02;        							// double speed U2X = 1
  	UCSRC = 0x06;        							// asynchronicznie, 8n1,
  	//UCSRB = 0x98;        							// RX i TX enabled, RX INT enabled
	UCSRB = 0x18;        							// RX i TX enabled

  //	sei();               							// Globalne wlaczenie przerwan
}
//================================================================



//================================================================
//===== Transmisja ===============================================
//================================================================
void uart_char(unsigned char znak) 
{
   	while(UDRE_ == 0); 								// czekaj na oczyszczenie bufora
    
	UDR = znak;										// wyslij znak
}


// tablica danych, ile danych wyslac
void uart_tab(unsigned char *data1, unsigned int ile) 
{
	unsigned int len = 0;

	if(ile == 0)
		ile = strlen((char*)data1);

	for (len = 0; len < ile; len++) 				// wyslij zadana ilosc znakow
	{
		while(UDRE_ == 0); 							// czekaj na oczyszczenie bufora
		UDR = data1[len];							// wyslij znak
	}
}


// zwraca odebrany znak
unsigned char uart_get(void)	
{
	unsigned char a1=0;

	while(RXC_ == 0);								// czekaj na znak

	a1 = UDR;										// odczyt znaku

	return a1;										// zwroc znak
}


// zwraca tablice odebranych danych
unsigned char uart_get_tab(unsigned char* data1, unsigned int ile)
{
	unsigned int x1=0;
	unsigned char crc=0;
	
	for(x1=0;x1<ile;x1++)							// odbierz zadana ilosc znakow
	{
		while(RXC_ == 0);							// czekaj na znak
		data1[x1] = UDR;							// odczyt znaku

		crc ^= data1[x1];							// przelicz sume kontrolna
	}
	return crc;										// zwroc sume kontrolna
}
//================================================================


//================================================================
//===== Transmisja danych z pamieci kodu =========================
//================================================================
// parametr "tab_p" - wskaznik na tablice w pamieci kodu
// parametr "ile" musi byc poprawny !!!
/*void uart_tab_pgm(PGM_P tab_p, unsigned char ile)
{
	unsigned int x1 = 0;

	for (x1 = 0; x1 < ile; x1++) 				// wyslij zadana ilosc znakow z tabicy w pamieci kodu
    	uart_znak(PGM(&tab_p[x1]));
}*/
//================================================================





