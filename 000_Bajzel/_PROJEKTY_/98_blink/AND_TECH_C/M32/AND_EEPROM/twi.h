/*
 * twi.h
 *
 *  Created on: 23-05-2013
 *      Author: Rados³aw Kwiecieñ
 *  
 * Description: Funkcje do obs³ugi interfejsu TWI zaczerpniête ze strony: http://radzio.dxp.pl/twi/
 */
#ifndef TWI_H
#define TWI_H

#include <avr/io.h>

#define ACK 1
#define NACK 0

// procedura transmisji sygna³u START
void twistart(void);
// procedura transmisji sygna³u STOP
void twistop(void);
// procedura transmisji bajtu danych
void twiwrite(char data);
//procedura odczytu bajtu danych
char twiread(char ack);

#endif //TWI_H
