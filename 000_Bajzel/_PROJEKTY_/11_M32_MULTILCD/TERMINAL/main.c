/*
 * main.c
 *
 *  Created on: 30-04-2013
 *      Author: Miros³aw Kardaœ
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "MKUART/mkuart.h"
#include "MK_TERMINAL/mk_term.h"

void rysuj_menu( void );
void show_adc( void ) ;
void show_counter( void );

uint8_t licznik;

int main( void ) {

	char c;

	USART_Init( __UBRR );

	sei();

	tr_cls(0);
	uart_puts("****** start *****");
	_delay_ms(1000);

	rysuj_menu();


	while(1) {

		c = uart_getc();
		if( c=='1') show_adc();
		if( c=='2') show_counter();
		if( c=='3') {
			tr_cls(0);
			uart_puts("wy³¹cz urz¹dzenie lub naciœnij reset ");
			while(1);
		}

	}
}


void rysuj_menu( void ) {

	tr_cls(0);

	tr_attr( 0, BLACK, YELLOW );
	tr_locate(2,5); uart_puts("    M E N U    ");

	tr_attr( 0, CYAN, BLACK );
	tr_locate(4,5); uart_puts("1. poka¿ wartoœæ ADC");
	tr_locate(5,5); uart_puts("2. poka¿ licznik");
	tr_locate(6,5); uart_puts("3. zakoñcz program");

	tr_attr( 0, WHITE, BLACK );
	tr_locate(8,5); uart_puts("wybierz opcjê (1-3): ");

}


void show_adc( void ) {

	char c;

	tr_cls(1);

	tr_attr( 0, WHITE, BLACK );
	tr_locate(2,5);
	uart_puts("   Napiêcie ADC0 ");
	tr_brush_color(BLUE);
	tr_locate(3,5); fill_line(' ',21);
	tr_locate(5,5); fill_line(' ',21);

	tr_attr( 0, YELLOW, BLACK );
	tr_locate(4,7);
	uart_puts("Wartoœæ:");
	tr_locate(4,17);
	tr_attr( BOLD, RED, BLACK );
	uart_puts(" 2,54 V ");
	tr_attr( 0, YELLOW, BLACK );

	while(1) {

		c = uart_getc();
		if( c=='q') {
			rysuj_menu();
			break;
		}

	}

}


void show_counter( void ) {

	char c;

	tr_cls(1);

	tr_attr( 0, WHITE, BLACK );
	tr_locate(2,5);
	uart_puts(" Licznik - ATmega32");
	tr_locate(3,5); fill_line('-',21);
	tr_locate(5,5); fill_line('-',21);

	while(1) {

		tr_attr( 0, YELLOW, BLACK );
		tr_locate(4,7);
		uart_puts("Wartoœæ:");
		tr_locate(4,17);
		tr_attr( BOLD, RED, BLUE );
		uart_puts(" ");
		uart_putint( licznik++, 10 );
		uart_puts(" ");
		tr_attr( 0, YELLOW, BLACK );
		uart_puts("    ");
		_delay_ms(100);

		c = uart_getc();
		if( c=='q') {
			rysuj_menu();
			break;
		}

	}
}
