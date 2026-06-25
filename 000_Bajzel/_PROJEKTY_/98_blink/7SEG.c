/*
 * _7SEG.c
 *
 * Created: 2014-02-08 17:38:47
 *  Author: Piotr
 */ 
#include "7SEG.h"

volatile uint8_t LED_data[4]={0,0,0,0};
	
//wzory cyfr	
const uint8_t PROGMEM digits[16]={
	(1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F),//0
	(1<<SEG_B)|(1<<SEG_C),//1
	(1<<SEG_A)|(1<<SEG_B)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_G),//2
	(1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_G),//3
	(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_F)|(1<<SEG_G),//4
	(1<<SEG_A)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_F)|(1<<SEG_G),//5
	(1<<SEG_A)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G),//6
	(1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C),//7
	(1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G),//8
	(1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_F)|(1<<SEG_G),//9
	(1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G),//A
	(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G),//B
	(1<<SEG_A)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F),//C
	(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_G),//D
	(1<<SEG_A)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G),//E
	(1<<SEG_A)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)//F
};

//inicjalizacja wyœwietlaczy
void LED_init(void){
	SEGMENTS_PORT = 0xFF;//wy³¹czam wszystkie segmenty	
	SEGMENTS_DDR = 0xFF;//wszystkie piny steruj¹ce segmentami jako wyjœcia
	COM_PORT |= (1<<COM1)|(1<<COM2)|(1<<COM3)|(1<<COM4);//wy³¹czam wszystkie wyœwietlacze
	COM_DDR |= (1<<COM1)|(1<<COM2)|(1<<COM3)|(1<<COM4);//ustawiam piny jako wyjœcia
	
	TCCR0 |= (1<<WGM01);//tryb CTC dla timera
	OCR0 = 38;//ustawiam zliczanie 0-38, dziêki temu czêstotliwoœæ przerwania wyniesie 16MHz/1024(prescaler)/39=~400Hz
	TCCR0 |= (1<<CS00)|(1<<CS02);//uruchamiam timer taktuj¹c go z prescalera 1024
	TIMSK |= (1<<OCIE0);
}

//wyœwietlanie cyfry w zapisie 10.
void LED_dec(uint16_t number, uint8_t dot){
	for(uint8_t i=0;i<4;i++){
		LED_data[i]=pgm_read_byte(&digits[number%10]);
		number /=10;
	}
	//zapalenie kropek
	if(dot&(1<<0))LED_data[0]|=(1<<SEG_DP);
	if(dot&(1<<1))LED_data[1]|=(1<<SEG_DP);
	if(dot&(1<<2))LED_data[2]|=(1<<SEG_DP);
	if(dot&(1<<3))LED_data[3]|=(1<<SEG_DP);
}

//wyœwietlanie cyfry w zapisie 16.
void LED_hex(uint16_t number, uint8_t dot){
	for(uint8_t i=0;i<4;i++){
		LED_data[i]=pgm_read_byte(&digits[number%16]);
		number /=16;
	}
	//zapalenie kropek
	if(dot&(1<<0))LED_data[0]|=(1<<SEG_DP);
	if(dot&(1<<1))LED_data[1]|=(1<<SEG_DP);
	if(dot&(1<<2))LED_data[2]|=(1<<SEG_DP);
	if(dot&(1<<3))LED_data[3]|=(1<<SEG_DP);
}


//przerwanie odœwierzaj¹ce wyœwietlacze
ISR(TIMER0_COMP_vect){	
	static uint8_t act_led=0;//zmienna przechowuj¹ca aktualnie zapalony wyœwietlacz
	act_led=(act_led+1)%4;//zaznaczam kolejny wyœwietlacz, ale dbam by zmienna nie mia³a zbyt du¿ej wartoœci
	
	COM_PORT |= (1<<COM1)|(1<<COM2)|(1<<COM3)|(1<<COM4);//gaszê wszystkie wyœwietlacze
	
	SEGMENTS_PORT = ~LED_data[act_led];
	
	//zapalê teraz wybrany wyœwietlacz
	switch(act_led){
		case 0: COM_PORT &= ~(1<<COM4); break;
		case 1: COM_PORT &= ~(1<<COM3); break;
		case 2: COM_PORT &= ~(1<<COM2); break;
		case 3: COM_PORT &= ~(1<<COM1); break;
	}
	
	//przy okazji odmierzamy czas - z powodu tego, i¿ przrwanie nie wystêpuje dok³adnie z czêstotliwoœci¹ 400Hz
	//czas ten bêdzie przybli¿ony, ale wystarcza do prostych celów
	if(LED_timer>0)LED_timer--;
}