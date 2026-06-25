/*
 * main.c
 *
 *  Created on: 24-05-2013
 *      Author: Piotr Rzeszut
 *  
 * Description: Przyk³ad obs³ugi 3 kana³ów PWM do sterowania diod¹ RGB w zestawie EvB5.1 v5
 */
#include <avr/io.h>
#include <util/delay.h>


int main(void){
	
	DDRD |= (1<<PD4)|(1<<PD5)|(1<<PD7);//ustawiam piny OC1A, OC1B i OC2 jako wyjœcia

	TCCR1A |= (1<<COM1A0)|(1<<COM1A1)|(1<<COM1B0)|(1<<COM1B1)|(1<<WGM10);
	//Ustawiamy odpwiedni tryb pracy wyjœæ OC dziêki temu wartoœæ 255 bêdzie oznacza³a maksymaln¹ jasnoœæ
	//Wybieramy tryb Pahse Correct PWM

	TCCR1B |= (1<<CS10);//w³¹czamy timer z preskalerem 1



	TCCR2 |= (1<<WGM20)|(1<<COM20)|(1<<COM21)|(1<<CS20);
	//Ustawiamy odpwiedni tryb pracy wyjœæ OC dziêki temu wartoœæ 255 bêdzie oznacza³a maksymaln¹ jasnoœæ
	//Wybieramy tryb Pahse Correct PWM z rozdzielczoœci¹ 8-bit
	//w³¹czamy timer z preskalerem 1

	OCR1A=255;
	OCR1B=128;
	OCR2=0;

	while(1){
		OCR1A=((OCR1A+1)&0x00FF);
		OCR1B=((OCR1B+1)&0x00FF);
		OCR2++;
		_delay_ms(10);
	}
	return 0;
}
