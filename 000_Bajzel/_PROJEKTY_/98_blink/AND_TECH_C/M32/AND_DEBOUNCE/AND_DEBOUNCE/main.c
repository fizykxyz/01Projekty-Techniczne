/*
 * main.c
 *
 * Created: 2014-02-15 14:13:47
 *  Author: Piotr
 *
 * Description: Program demonstruj¹cy eliminacjê drgañ styków z wykorzystaniem timera dla uk³adów joysticka itp. 
 *              (tylko jeden aktywny w tym samym czasie przycisk)
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "debounce.h"
#include "HD44780.h"


//definiujê funkcjê odczytu stanu przycisków
uint8_t menu_read(void){
	if(!(PIND&(1<<PD5)))return 1;
	if(!(PIND&(1<<PD6)))return 2;
	if(!(PIND&(1<<PD7)))return 3;
	return 0;
}

//inicjalizacja hardware do przycisków
void menu_hw_init(void){
	DDRD &= ~((1<<PD7)|(1<<PD6)|(1<<PD5));//piny jako wejœcia
	PORTD |= ((1<<PD7)|(1<<PD6)|(1<<PD5));//podci¹ganie
	
	TCCR0 |= (1<<WGM01);//tryb CTC dla timera 0
	OCR0 = F_CPU/1024/100;//wyliczamy wartoœæ OCR dla osi¹gniêcia przerwania o okresie oko³o 10ms
	TCCR0 |= (1<<CS00)|(1<<CS02);//preskaler 1024
	TIMSK |= (1<<OCIE0);//zezwolenie na przerwanie od OCR0A
}

//definiujê odpowiednie funkcje dla przytrzymania i klikania przycisków
void click_1(void){
	static uint8_t count=0;
	char buffer[17];
	count++;
	LCD_GoTo(0,1);
	sprintf(buffer,"Click 1:%03d",count);
	LCD_WriteText(buffer);
}

void hold_1(void){
	static uint8_t count=0;
	char buffer[17];
	count++;
	LCD_GoTo(0,1);
	sprintf(buffer,"Hold 1 :%03d",count);
	LCD_WriteText(buffer);
}


void click_2(void){
	static uint8_t count=0;
	char buffer[17];
	count++;
	LCD_GoTo(0,1);
	sprintf(buffer,"Click 2:%03d",count);
	LCD_WriteText(buffer);
}

void hold_2(void){
	static uint8_t count=0;
	char buffer[17];
	count++;
	LCD_GoTo(0,1);
	sprintf(buffer,"Hold 2 :%03d",count);
	LCD_WriteText(buffer);
}


void click_3(void){
	static uint8_t count=0;
	char buffer[17];
	count++;
	LCD_GoTo(0,1);
	sprintf(buffer,"Click 3:%03d",count);
	LCD_WriteText(buffer);
}

void hold_3(void){
	static uint8_t count=0;
	char buffer[17];
	count++;
	LCD_GoTo(0,1);
	sprintf(buffer,"Hold 3 :%03d",count);
	LCD_WriteText(buffer);
}

int main(void)
{
	menu_init();//inicjalizacja systemów menu
	menu_hw_init();
	
	LCD_Initalize();
	sei();//w³¹czenie globalne przerwañ
	LCD_GoTo(0,0);
	LCD_WriteText("  Debounce demo ");
	
	//przypisujemy funkcje do odpowiednich dzia³añ
	click[1]=click_1;
	hold[1]=hold_1;
	click[2]=click_2;
	hold[2]=hold_2;
	click[3]=click_3;
	hold[3]=hold_3;
	
	//w pêtli g³ównej nie robimy zupe³nie nic!!!
    while(1){
		//pustki - procesor wiêkszoœæ czasu mo¿e robiæ co innego!!!
	}
}

ISR(TIMER0_COMP_vect){
	//w przerwaniu mo¿emy wykonywaæ te¿ wiele innych czynnoœci, a jeœli przerwanie wystêpuje czêœciej ni¿ co oko³o 10ms
	//mo¿emy wywo³ywaæ tê funkcjê co któreœ przerwanie.
	menu_check();
}