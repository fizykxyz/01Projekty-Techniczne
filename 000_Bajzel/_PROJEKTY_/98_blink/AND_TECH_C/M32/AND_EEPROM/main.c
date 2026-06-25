/*
 * main.c
 *
 *  Created on: 23-05-2013
 *      Author: Piotr Rzeszut
 *  
 * Description: Przyk³ad obs³ugi pamiêci EEPROM AT24C128 na p³ytce EvB5.1 v5
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "twi.h"
#include "uart.h"

void EEPROM_write(uint16_t address, uint8_t data){
	twistart();//inicjalizacja magistrali
	twiwrite(0b10100110);//wysy³amy adres naszej pamiêci do zapisu
	twiwrite(address>>8);//wysy³amy 2 bajty zawieraj¹ce adres komórki pamiêci
	twiwrite(address);
	twiwrite(data);//wysy³amy dane do zapisania
	twistop();//sygna³ zakoñczenia kominikacji
	_delay_ms(10);//czekamy 10ms - maksymalny czas realizacji zapisu
}

uint8_t EEPROM_read(uint16_t address){
	twistart();//inicjalizacja magistrali
	twiwrite(0b10100110);//wysy³amy adres naszej pamiêci do zapisu
	twiwrite(address>>8);//wysy³amy 2 bajty zawieraj¹ce adres komórki pamiêci
	twiwrite(address);
	twistart();//sygna³ start
	twiwrite(0b10100111);//adres uk³adu do odczytu
	uint8_t data=twiread(NACK);//odczytujemy pojedynczy bajt (bez potwierdzenia)
	twistop();//sygna³ zakoñczenia komunikacji
	return data;
}

int main(void){
	
	TWSR |= (1<<TWPS0);
	TWBR = 72;//czêstotliwoœæ TWI ustawiamy na oko³o 50kHz (zgodnie z informacjami w nocie katalogowej)

	uart_init(9600);//inicjalizacja uart'u
	sei();	

	uint8_t dane=EEPROM_read(1234);
	uart_puts("Pod adresem 1234 mamy: ");
	uart_putint(dane,10);
	dane++;
	uart_puts("\r\nZapiszemy tam: ");
	uart_putint(dane,10);
	EEPROM_write(1234,dane);
	uart_puts("\r\nKoniec zabawy\r\n\r\n");

	while(1);

	return 0;
}
