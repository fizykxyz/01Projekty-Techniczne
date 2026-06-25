/*
 * main.c
 *
 *  Created on: 23-05-2013
 *      Author: Piotr Rzeszut
 *  
 * Description: Przyk³ad obs³ugi zegara PCF8563
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "twi.h"
#include "uart.h"

uint8_t dec2bcd(uint8_t dane){
	return (((dane/10)<<4)|(dane%10));
}

uint8_t bcd2dec(uint8_t dane){
	return (((dane>>4)*10)+(dane&0x0F));
}

const char wd_names[7][4]={
	" Nd",
	"Pon",
	" Wt",
	" Sr",
	"Czw",
	" Pt",
	" Sb"
};

volatile uint8_t h,i,s=255,d,wd,m,y,s_old=255;

int main(void){

	TWSR |= (1<<TWPS0);
	TWBR = 72;//czêstotliwoœæ TWI ustawiamy na oko³o 50kHz (zgodnie z informacjami w nocie katalogowej)

	uart_init(9600);//inicjalizacja uart'u

	DDRD &= ~(1<<PD2);
	PORTD |= (1<<PD2);//PD2 - wejœcie + pull-up

	MCUCR |= (1<<ISC01);//zbocze opadaj¹ce wyzwala INT0

	sei();

	twistart();
	twiwrite(0xA2);//adres zegara do zapisu
	twiwrite(0x0D);//rejestr konfiguracji wyjœcia CLKOUT
	twiwrite(0b10000011);//wyjœcie aktywne, 1Hz
	twistop();

	//jeœli trzeba to pobieramy ustawienia i zapisujemy je

	uart_puts("Ustawic inny czas? (0-NIE   1-TAK): ");
	h=uart_getint();
	uart_puts("\r\n");
	if(h!=0){
		
		uart_puts("Rok (00-99): ");
		y=dec2bcd(uart_getint());
		uart_puts("\r\n");

		uart_puts("Miesiac: ");
		m=dec2bcd(uart_getint());
		uart_puts("\r\n");

		uart_puts("Dzien: ");
		d=dec2bcd(uart_getint());
		uart_puts("\r\n");

		uart_puts("Dzien tygodnia\r\n");
		for(uint8_t i=0;i<7;i++){
			uart_putint(i,10);
			uart_puts("->");
			uart_puts((char*)wd_names[i]);
			uart_puts("\r\n");
		}
		uart_puts("Podaj liczbe z zakresu (0-6): ");
		wd=uart_getint();
		uart_puts("\r\n");

		uart_puts("Godzina: ");
		h=dec2bcd(uart_getint());
		uart_puts("\r\n");

		uart_puts("Minuta: ");
		i=dec2bcd(uart_getint());
		uart_puts("\r\n");

		uart_puts("Sekunda: ");
		s=dec2bcd(uart_getint());
		uart_puts("\r\n");

		_delay_ms(10);
		
		twistart();
		twiwrite(0xA2);//adres zegara
		twiwrite(0x02);//adres rejestru sekund
		twiwrite(s);
		twiwrite(i);
		twiwrite(h);
		twiwrite(d);
		twiwrite(wd);
		twiwrite(m);
		twiwrite(y);
		twistop();

		uart_puts("Czas zapisany!!!\r\n");

	}
	s_old=s;

	GICR |= (1<<INT0);//zezwolenie na przerwanie INT0

	while(1){
		
		if(s!=s_old){
			s_old=s;

			//do wyœwietlania u¿ywamy tricku - wyœwietlamy liczbê kodowan¹ jako BCD 
			//tak jakby by³a to liczba w zapisie heksadecymalnym
			//dziêki temu odpowiednie liczby l¹duj¹ na swoich miejscach
			//bez koniecznoœci u¿ywania instrukcji bcd2dec
			if(h<0x10)uart_putc('0');
			uart_putint(h,16);
		
			uart_putc(':');

			if(i<0x10)uart_putc('0');
			uart_putint(i,16);
		
			uart_putc(':');
		
			if(s<0x10)uart_putc('0');
			uart_putint(s,16);
		
			uart_putc(' ');

			uart_puts((char*)wd_names[wd]);

			uart_putc(' ');

			if(d<0x10)uart_putc('0');
			uart_putint(d,16);
		
			uart_putc('-');

			if(m<0x10)uart_putc('0');
			uart_putint(m,16);
		
			uart_putc('-');
		
			if(y<0x10)uart_putc('0');
			uart_putint(y,16);
		
			uart_puts("\r\n");
		}		

	}

	return 0;
}

//obs³uga przerwania INT0
ISR(INT0_vect){
	twistart();
		twiwrite(0xA2);//adres zegara do zapisu
		twiwrite(0x02);//adres rejestru sekund
		twistart();
		twiwrite(0xA3);//adres zegara do odczytu
		s=twiread(ACK)&0b01111111;
		i=twiread(ACK)&0b01111111;
		h=twiread(ACK)&0b00111111;
		d=twiread(ACK)&0b00111111;
		wd=twiread(ACK)&0b00000111;
		m=twiread(ACK)&0b00011111;
		y=twiread(NACK);
		twistop();
}
