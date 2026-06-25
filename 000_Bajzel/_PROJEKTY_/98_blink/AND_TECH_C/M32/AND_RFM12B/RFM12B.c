/*
 * RFM12B.c
 *
 *  Created on: 10-06-2013
 *      Author: Piotr Rzeszut
 *  
 * Description: Funkcje do obs³ugi uk³adu RFM12B z poziomu procesora AVR
 */
#include "RFM12B.h"

void Rfm_spi_init(void){
	DDRB |= (1<<PB7)|(1<<PB5)|(1<<PB4);//konfiguracja kierunku i podcigania linii SPI
	PORTB |= (1<<PB6);
	//uwaga - pomimo i¿ tu nie u¿ywamy lini SS jako lini CS dla uk³adu RFM
	//to do poprawnej pracy modu³u SPI w procesorze linia SS musi byæ ustawiona jako wyjœcie

	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);//SPI w trybie master z podzia³em zegara przez 64 
	

	PORTC |= (1<<PC5);//dezaktywacja uk³adu RFM12B
	DDRC |= (1<<PC5);//konfiguracja kierunku linii CS
}

uint16_t Rfm_xmit(uint16_t data){
	//na pocz¹tek rodzielamy dane na 2 bajty
	uint8_t msb, lsb;
	lsb = data;
	msb = data>>8;
	PORTC &= ~(1<<PC5);//aktywujemy liniê CS uk³adu
	//teraz wysy³amy 2 bajty jednoczeœnie odbieraj¹c 2 bajty z uk³adu
	SPDR = msb;
	while(!(SPSR&(1<<7)));
	msb = SPDR;
	SPDR = lsb;
	while(!(SPSR&(1<<7)));
	lsb = SPDR;
	//_delay_ms(1);
	PORTC |= (1<<PC5);//dezaktywujemy liniê CS uk³adu
	//i zwracamy odebrane dane
	return( (((uint16_t)msb)<<8)+((uint16_t)lsb) );
}

void Rfm_ready_wait(void){
	//aby sprawdziæ gotowoœæ uk³adu do odbioru komend z procesora
	//aktywujemy jego liniê CS
	PORTC &= ~(1<<PC5);
	//czekamy a¿ uk³ad na to zareaguje
	_delay_us(1);
	//i oczekujemy a¿ linia danych zostanie zwolniona, co oznacza wykonanie poprzednich poleceñ
	/*
	w wersji docelowej ta instrukcja powinna byæ wykonywana do okreœlonego czasu, po którym
	procesor powinien uznaæ, ¿e uk³ad siê zawiesi³ lub nie odpowiada z innych przyczyn
	i nale¿y podj¹æ odpowiednie dzia³ania (np. informacja, ¿e ¿adne dane nie sa do nas wysy³ane lub
	¿e nalezy zreseowaæ uk³ad)
	*/
	while(!(PINB&(1<<PB6)));
}

void Rfm_write_str(char* s, uint8_t address){
	//instrukcja nadaj¹ca ci¹g znaków
	char c;//zmienna pomocnicza
	//w³¹czamy nadajnik
	Rfm_xmit(POWER|EN_TRANSMISSION|EN_SYNTH|EN_OSC|DIS_CLKO);
	//czekamy nieco d³uzej na jego aktywacjê
	_delay_ms(5);

	//nadajemy 3 bajty synchronizacji (0xAA)
	Rfm_ready_wait();
	Rfm_xmit(TX_WRITE|0xAA);
	Rfm_ready_wait();
	Rfm_xmit(TX_WRITE|0xAA);
	Rfm_ready_wait();
	Rfm_xmit(TX_WRITE|0xAA);
	//nastêpnie kolejny bajt synchrnoizacji (0x2D)
	Rfm_ready_wait();
	Rfm_xmit(TX_WRITE|0x2D);
	Rfm_ready_wait();
	//oraz definiowalny bajt synchronizacji (czyli nasz adres)
	Rfm_xmit(TX_WRITE|address);
	Rfm_ready_wait();

	//wysy³amy ca³y ³añcuch znaków
	while((c=*s++)){
		Rfm_xmit(TX_WRITE|c);
		Rfm_ready_wait();
	}
	//i bajt równy 0 koñcz¹cy ci¹g znaków
	Rfm_xmit(TX_WRITE|0);
	Rfm_ready_wait();
	//oraz 3 bajty koñcz¹ce transmisjê
	Rfm_xmit(TX_WRITE|0xAA);
	Rfm_ready_wait();
	Rfm_xmit(TX_WRITE|0xAA);
	Rfm_ready_wait();
	Rfm_xmit(TX_WRITE|0xAA);
	Rfm_ready_wait();
	//nastêpnie wy³¹czamy nadajnik
	Rfm_xmit(POWER|DIS_CLKO);
}

void Rfm_read_str(char* c, uint8_t max_size){
	//w³¹czamy odbiornik
	Rfm_xmit(POWER|EN_RX|EN_BASEBAND|EN_OSC|DIS_CLKO);
	_delay_ms(5);
	//para komend powoduj¹ca w efekcie dzia³ania reset synchronizacji odbiornika
	Rfm_xmit(FIFO_RST|FIFO_IT_8|HS_RST_DIS);
	Rfm_xmit(FIFO_RST|FIFO_IT_8|EN_AFT_SYNC|HS_RST_DIS);
	
	uint8_t i=0;
	do{
		//oczekujemy a¿ uk³ad bêdzie gotowy (a¿ bajt bêdzie w buforze)
		Rfm_ready_wait();
		//i odczytujemy ten bajt
		*c=(char)Rfm_xmit(FIFO_READ);
		i++;
		if(i>=max_size)*(++c)=0;
	}while(*c++!=0);

	//nastêpnie wy³¹czamy odbiornik
	Rfm_xmit(POWER|DIS_CLKO);
}
