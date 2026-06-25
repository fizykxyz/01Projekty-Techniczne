/*================================================================
==================================================================
=	Definicje dla pliku glownego programu testowego ATmega169 + LCD
=	wersja = 1.0
=	Copywright - LiveLights
==================================================================	
================================================================*/

#ifndef _AVR_ATMEGA169_LCD_TEST_H_
#define _AVR_ATMEGA169_LCD_TEST_H_

#include <avr/io.h>
#include <avr/pgmspace.h>


#define OSC				11059200
#define FOSC 			OSC/1000000								// czestotliwosc kwarcu w MHz
//#define F_CPU 			11059200UL


#define NOP() 			{asm volatile("nop"::);}				// NOP
#define PGM				pgm_read_byte_near


//================================================================
//===== definicje bitów w rejestrach ATMega8 =====================
//================================================================
typedef struct _bit_struct
{
	unsigned char bit0: 1;
	unsigned char bit1: 1;
	unsigned char bit2: 1;
	unsigned char bit3: 1;
	unsigned char bit4: 1;
	unsigned char bit5: 1;
	unsigned char bit6: 1;
	unsigned char bit7: 1;
}pole_bitowe;

#define DAJ_BIT(adr) (*((volatile pole_bitowe*) (adr)))

#define _PORTA 0x22											// okreslenie adresow rejestrow
#define _DDRA 0x21											// w przestrzeni adresowej pamieci RAM ATmega169
#define _PINA 0x20

#define _PORTB 0x25
#define _DDRB 0x324
#define _PINB 0x23

#define _PORTC 0x28
#define _DDRC 0x27
#define _PINC 0x26

#define _PORTD 0x2B
#define _DDRD 0x2A
#define _PIND 0x29

#define _PORTE 0x2E
#define _DDRE 0x2D
#define _PINE 0x2C

#define _PORTF 0x31
#define _DDRF 0x30
#define _PINF 0x2F

#define _PORTG 0x34
#define _DDRG 0x33
#define _PING 0x32
//================================================================

#define BT1				DAJ_BIT(_PIND).bit4			// piny wejsciowe klawiatury

#define BT1_DIR			DAJ_BIT(_DDRD).bit4			// kierunek pinow wejsciowych klawiatury


#endif

