/*
 * main.c
 *
 *  Created on: 28-05-2013
 *      Author: Piotr Rzeszut
 *  
 * Description: Przyk³ad komunikacji z wykorzystaniem magistrali RS485
 */

#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"

//zakomentuj poni¿sz¹ linijkê, aby otrzymaæ kod dla uk³adu SLAVE
#define MASTER

//definiujemy adres dla uk³adu slave - mo¿emy do sieci podpi¹æ wiele uk³adów slave
//i ka¿demu nadaæ inny adres - mo¿ecie z tym poeksperymentowaæ
#define SLAVE_ADDR 'A'

//definicja pinu steruj¹cego kierunkiem transmisji
//podpinamy go do pinu RE na naszej p³ytce
//RxD (PD0) podpinamy do pinu RO
//TxD (PD1) podpinamy do pinu DI
#define DIR_DDR DDRD
#define DIR_PORT PORTD
#define DIR PD2

#define DIR_NAD() DIR_PORT |= (1<<DIR)
#define DIR_ODB() DIR_PORT &= ~(1<<DIR)

//kompilujemy odpowiedni kod w zale¿noœci od wyboru
#ifdef MASTER
/*****************************************************************
kod dla uk³adu master
*****************************************************************/

//LCD pod³¹czopny zgodnie z ustawieniami w includowanym pliku
#include "HD44780.h"

char odebrany;

int main(void){
	
	//inicjalizujemy konieczne uk³ady
	DIR_DDR |= (1<<DIR);

	DIR_NAD();
	_delay_ms(2);

	LCD_Initalize();
	uart_init(9600);
	sei();

	while(1){
		UCSRA |= (1<<TXC);//resetujemy flagê zakoñczenia nadawania

		uart_putc(27);//esc - reset (taki kod przyj¹³em w swojej implementacji za kod resetu transmisji)
		uart_putc(SLAVE_ADDR);//adres uk³adu slave
		uart_putc('2');//pierwsza dana
		uart_putc('3');//druga dana

		while(UCSRB &(1<<UDRIE));
		while(!(UCSRA &(1<<TXC)));//czekamy na zakoñczenie nadawania (opró¿nienie bufora nadawczego
		//oraz zakoñczenie nadawania ostatniego bajtu

		DIR_ODB();//dopiero potem prze³¹czamy siê na odbiór danych
		for(uint8_t i=0;i<200;i++){//czekamy tylko okreœlony czas
		//w przeciwnym razie znaczy, ze nikt nie chce nam odpowiedzieæ na zapytanie
			_delay_ms(1);
			if((odebrany=uart_getc())!=0)break;
		}
		DIR_NAD();//jeœli odebraliœmy dan¹ (1 bajt) to spowrotem prze³¹czamy w tryb nadawania
		LCD_Clear();//i potem kasujemy lcd i
		if(odebrany!=0){//w zale¿noœci czy ktoœ nam odpowiedzia³ wyœwietlamy komunikat
			LCD_WriteData(odebrany);
		}else{
			LCD_WriteText("Timeout");
		}
		//kolejna próba po³¹czenia z uk³adem slave za 1 sekundê
		_delay_ms(1000);
		
	}
	
	return 0;
}

#else
/*****************************************************************
kod dla uk³adu slave
*****************************************************************/

char odebrany;
uint8_t a,b;

int main(void){
	
	//inicjalizacja sprzêtu
	DIR_DDR |= (1<<DIR);

	DIR_ODB();
	_delay_ms(2);

	uart_init(9600);
	sei();

	while(1){
		if(uart_getc()==27){//jeœli odebraliœmy kod ESC to znaczy, ¿e ktoœ chce nas o coœ zapytaæ
			for(uint8_t i=0;i<200;i++){//czekamy tylko pewien okreœlony timeout - mo¿e ktoœ siê pomyli³
				_delay_ms(1);
				if((odebrany=uart_getc())!=0)break;
			}
			if(odebrany==SLAVE_ADDR){//tylko jeœli nadaj¹ do nas
				for(uint8_t i=0;i<200;i++){//czekamy tylko pewien okreœlony timeout
					_delay_ms(1);
					if((odebrany=uart_getc())!=0)break;
				}
				a=odebrany-'0';
				for(uint8_t i=0;i<200;i++){//czekamy tylko pewien okreœlony timeout
					_delay_ms(1);
					if((odebrany=uart_getc())!=0)break;
				}
				b=odebrany-'0';//odczytaliœmy 2 bajty danych - 2 cyfry
				DIR_NAD();
				_delay_ms(1);

				UCSRA |= (1<<TXC);//resetujemy flagê zakoñczenia nadawania

				uart_putc(a*b+'0');//wykonujemy jakieœ proste obliczenia i odsy³amy ich wynik jako odpowiedŸ

				while(UCSRB &(1<<UDRIE));//czekamy na zakoñczenie nadawania
				while(!(UCSRA &(1<<TXC)));//czekamy na zakoñczenie nadawania

				DIR_ODB();//w trybie oczekiwania pozostajemy w trybie odbioru.
				_delay_ms(1);
			}
		}
	}
	
	return 0;
}

#endif
