/*
 * ADC.c
 *
 * Created: 2014-02-12 16:46:07
 *  Author: Piotr
 */ 
#include "ADC.h"

void ADC_init(void){
	ADMUX |= (1<<REFS0);//ustawiam napiêcie odniesienia na AVCC (napiêcie zasilania uk³adu ~5V)
	ADCSRA |= (1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);//ustawienie prescalera na 128 = czêstotliwoœæ taktowania 125kHz
	ADCSRA |= (1<<ADEN);//w³¹czenie przetwornika
	
}
uint16_t ADC_get(uint8_t channel){
	ADMUX=(ADMUX&0b11100000)|(channel&0b00011111);//ustawiam kana³
	ADCSRA |= (1<<ADSC);//rozpoczynam konwersjê
	while(ADCSRA&(1<<ADSC));//czekamy na zakoñczenie tej konwersji
	return ADC;
}
