/*
 * main.c
 *
 * Created: 2014-02-08 17:33:14
 *  Author: Piotr
 *
 * Description: Przyk³ad obs³ugi wyœwietlacza 7-segmentowego
 */ 


#include <avr/io.h>
#include "7SEG.h"


int main(void)
{
	LED_init();//konfiguracja wyœwietlania
	sei();//w³¹czenie systemu przerwañ
	
	uint16_t liczba=0;
    while(1)
    {
		LED_dec(liczba,0b00000010);
		liczba=(liczba+1)%10000;
		LED_timer=40;
		asm volatile ("nop");//czekamy jeden cykl procesora, aby nie dosz³o do b³êdu przy porównaniu
		while(LED_timer>0);
    }
}