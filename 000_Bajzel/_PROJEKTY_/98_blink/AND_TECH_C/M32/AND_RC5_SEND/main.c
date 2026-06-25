/*
 * main.c
 *
 *  Created on: 27-05-2013
 *      Author: Piotr Rzeszut
 *  
 * Description: Przyk³ad nadawania w standardzie RC5
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "RC5_send.h"

int main(void){
	//inicjalizacja sprzêtu i zezwolenie na przerwania
	RC5_send_init();
	sei();
	
	//zmienne lokalne
	uint8_t tog=0,adr=0,dat=0;
	
	while(1){
		//nadajemy dane
		RC5_send(tog,adr,dat);
		dat++;//zwiêkszamy numer komendy i adresy dbaj¹c o zachowanie maksymalnych danych
		if(dat==64){
			dat=0;
			adr++;
			if(adr==32) adr=0;
		}
		tog=!tog;//przy ka¿dej transmisji, jak w prawidziwym pilocie, zmieniamy togglebit
		//musimy pamiêtaæ o tym, aby miêdzy kolejnymi transmisjami zachowaæ odstêp minimum 144ms
		_delay_ms(1000);
	}	

	return 0;
}
