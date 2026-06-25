/*
 * twi.c
 *
 *  Created on: 23-05-2013
 *      Author: Rados³aw Kwiecieñ
 *  
 * Description: Funkcje do obs³ugi interfejsu TWI zaczerpniête ze strony: http://radzio.dxp.pl/twi/
 */
#include "twi.h"

// procedura transmisji sygna³u START
void twistart(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

// procedura transmisji sygna³u STOP
void twistop(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	while ((TWCR & (1<<TWSTO)));
}

// procedura transmisji bajtu danych
void twiwrite(char data)
{
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

//procedura odczytu bajtu danych
char twiread(char ack)
{
	TWCR = ack
	? ((1 << TWINT) | (1 << TWEN) | (1 << TWEA))
	: ((1 << TWINT) | (1 << TWEN)) ;
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}
