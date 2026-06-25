/*
 * RC5_send.c
 *
 *  Created on: 27-05-2013
 *      Author: Piotr Rzeszut
 *  
 * Description: Biblioteka do obs³ugi nadawania w standardzie RC5
 * Inormations: http://www.sbprojects.com/knowledge/ir/rc5.php
 */
#include "RC5_send.h"

volatile uint8_t RC5_bitcount=0;
volatile uint16_t RC5_data;

void RC5_send_init(void){
	TCCR0 |= (1<<WGM01);//tryb CTC
	OCR0 = 222;//OCR ustawiam na 222, dziêki czemu przy 16MHz osi¹gam ok. 72kHz
	//wartoœæ wyliczamy korzystaj¹c ze wzoru:
	//F_CPU/preskaler/72000
	TCCR0 |= (1<<CS00);//preskaler 1
	
	//pin nadawczy konfigurujemy jako wyjœcie
	RC5_PORT |= (1<<RC5_PN);
	RC5_DDR |= (1<<RC5_PN);
}

void RC5_send(uint8_t toggle, uint8_t address, uint8_t command){
	RC5_bitcount=14;//ustawiamy iloœæ bitów do nadania (14) i tworzymy 14-sto bitow¹ ramkê RC5
	RC5_data=0b11000000000000|(((uint16_t)(toggle&0b00000001))<<11)|(((uint16_t)(address&0b00011111))<<6)|(((uint16_t)(command&0b00111111))<<0);
	//aktywujemy przerwanie co rozpoczyna nadawanie
	TIMSK |= (1<<OCIE0);
	//czekamy na zakoñczenie nadawania (sygnalizowane wy³¹czeniem przerwania)
	while(TIMSK&(1<<OCIE0));
	//wy³¹czamy diodê RC5
	RC5_PORT |= (1<<RC5_PN);
}

//w przerwaniu timera wywo³ywanym z czêstotliwoœci¹ 72kHz
ISR(TIMER0_COMP_vect){
	static uint8_t clk_counter=0;
	if(RC5_bitcount){//jeœli mamy jeszcze jakieœ bity do nadania
		clk_counter++;//zwiêkszamy licznik zbocz
		if(RC5_data&0b10000000000000){//i w zale¿noœci od wartoœci aktualnie nadawanego bitu
			if(clk_counter<64){//nadajemy 1 w standardzie manchester
				RC5_PORT |= (1<<RC5_PN);
			}else{
				RC5_PORT ^= (1<<RC5_PN);
			}
		}else{
			if(clk_counter<64){//nadajemy 0 w standardzie manchester
				RC5_PORT ^= (1<<RC5_PN);
			}else{
				RC5_PORT |= (1<<RC5_PN);
			}
		}
		if(clk_counter==128){//jeœli nadaliœmy ca³y bit to
			clk_counter=0;//resetujemy licznik zbocz
			RC5_bitcount--;//zmniejszamy licznik bitów pozosta³ych do nadania
			RC5_data<<=1;//przesuwamy dane o 1 bit, ¿eby nadaæ kolejny bit w nastêpnym cyklu
		}

	}else{//jeœli nadaliœmy wszystkie bity
		TIMSK &= ~(1<<OCIE0);//to wy³¹czamy przerwanie
		clk_counter=0;//i resetujemy konieczne zmienne
	}
		
}
