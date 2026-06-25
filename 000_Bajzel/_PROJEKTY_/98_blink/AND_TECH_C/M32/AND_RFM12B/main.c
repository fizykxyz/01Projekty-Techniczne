/*
 * main.c
 *
 *  Created on: 10-06-2013
 *      Author: Piotr Rzeszut
 *  
 * Description: Przyk³ad obs³ugi modu³ów RFM12B na p³ytkach EvB5.1 v5
 */
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "RFM12B.h"
#include "uart.h"

char bufor[65];
//zakomentowanie poni¿szej linii spowoduje kompilacjê kodu dla uk³adu master
//#define SLAVE

#ifdef SLAVE
//-------------------------------------------
//to jest kod dla uk³adu podrzêdnego - SLAVE
//-------------------------------------------
int main(void){

	Rfm_spi_init();//inicjalizacja magistrali SPI
	
	Rfm_xmit(SW_RESET);
	_delay_ms(250);//resetujê uk³ad i czekam nieco na zakoñczenie tej operacji.

	//inicjalizacja RFM12B
	//ustawienie pasma 868MHz, konfiguracja FIFO
	Rfm_xmit(CONFIGURATION|EN_DATA_REG|EN_FIFO|BAND_868|CAP_12_0);
	//w³¹czenie oscylatora
	Rfm_xmit(POWER|EN_OSC|DIS_CLKO);
	//ustawienie pasma (musi byæ takie samo w nadajniku i odbiorniku)
	Rfm_xmit(FREQUENCY|RF12_FREQUENCY_CALC_868(868000000UL));
	//ustawienie prêdkoœci transmisji danych (musi byæ takia sama w nadajniku i odbiorniku)
	Rfm_xmit(BAUD|BAUD_4800);
	//ustawienie pasma 134kHz i parametrów odbiornika
	Rfm_xmit(RECEIVER|P20_VDI|BW134|LNA_0|RSSI_103);
	//ustawienie cyfroiwego filtra danych i opcji odzyskiwania zegara
	Rfm_xmit(DATA_FILTER|AUTO_CR|DIGITAL_F|DQD_4);
	//reset bufora FIFO, konfiguracja synchronizacji za pomoc¹ 2 bajtów
	Rfm_xmit(FIFO_RST|FIFO_IT_8|FIFO_SYNC|HS_RST_DIS);
	
	//ustawienie konfigurowalnego bajta synchronizacji na D0 (bêdzie on "adresem" naszego uk³adu)
	Rfm_xmit(SYNC_PATTERN|0xD0);

	//konfiguracja kontrolera czêstotliwoœci
	Rfm_xmit(AFC|KEEP_RX|REST_OFF|EN_FOFFSET|EN_AFC);
	//konfiguracja nadajnika i jego mocy (na ustawienie maksymalne)
	Rfm_xmit(TRANSMITER|FSK_PHASE_0|FSK_DF_90K|OUT_P_0);
	//konfiguracja pêtli PLL
	Rfm_xmit(PLL|PLL_DH_DIS|SEL_CLK_2_5|MAX_BAUD_256);

	//wy³¹czenie timera wakeup
	Rfm_xmit(WAKEUP_TIM|WUT_X_2_0|0);
	//wy³¹czenie oszczêdzania energii
	Rfm_xmit(LOW_DC|LOW_DC_DIS);
	//ustawienie monitora napiêcia na 2,2V
	Rfm_xmit(BOD_CLK|CLK_5_00|BOD_2_2);

	uint8_t i=0;

	while(1){
		Rfm_read_str(bufor,64);//odczytujê dane przesy³ane przez inny uk³ad
		if(strcmp(bufor,"Siemka")==0){//jesli dosta³em powitanie
			i++;//to przygotowujê arcyciekaw¹ informacjê
			sprintf(bufor,"Slave wita po raz %03d",i);
			Rfm_write_str(bufor, 0xD4);
			//i wysy³am j¹ na adres uk³adu nadrzêdnego (ma on zdefiniowany "adres" 0xD4)
		}
	}

	return 0;
}

#else
//-------------------------------------------
//to jest kod dla uk³adu nadrzêdnego - MASTER
//-------------------------------------------
int main(void){

	Rfm_spi_init();//inicjalizacja magistrali SPI
	
	Rfm_xmit(SW_RESET);
	_delay_ms(250);//resetujê uk³ad i czekam nieco na zakoñczenie tej operacji.
	
	uart_init(9600);//inicjalizacja USART
	sei();//w³¹czamy przerwania do obs³ugi uart

	uart_puts("RFM12B - MASTER\r\n");//wyœwietlamy powitanie

	//inicjalizacja RFM12B
	//ustawienie pasma 868MHz, konfiguracja FIFO
	Rfm_xmit(CONFIGURATION|EN_DATA_REG|EN_FIFO|BAND_868|CAP_12_0);
	//w³¹czenie oscylatora
	Rfm_xmit(POWER|EN_OSC|DIS_CLKO);
	//ustawienie pasma (musi byæ takie samo w nadajniku i odbiorniku)
	Rfm_xmit(FREQUENCY|RF12_FREQUENCY_CALC_868(868000000UL));
	//ustawienie prêdkoœci transmisji danych (musi byæ takia sama w nadajniku i odbiorniku)
	Rfm_xmit(BAUD|BAUD_4800);
	//ustawienie pasma 134kHz i parametrów odbiornika
	Rfm_xmit(RECEIVER|P20_VDI|BW134|LNA_0|RSSI_103);
	//ustawienie cyfroiwego filtra danych i opcji odzyskiwania zegara
	Rfm_xmit(DATA_FILTER|AUTO_CR|DIGITAL_F|DQD_4);
	//reset bufora FIFO, konfiguracja synchronizacji za pomoc¹ 2 bajtów
	Rfm_xmit(FIFO_RST|FIFO_IT_8|FIFO_SYNC|HS_RST_DIS);
	
	//ustawienie konfigurowalnego bajta synchronizacji na D4 (bêdzie on "adresem" naszego uk³adu)
	Rfm_xmit(SYNC_PATTERN|0xD4);

	//konfiguracja kontrolera czêstotliwoœci
	Rfm_xmit(AFC|KEEP_RX|REST_OFF|EN_FOFFSET|EN_AFC);
	//konfiguracja nadajnika i jego mocy (na ustawienie maksymalne)
	Rfm_xmit(TRANSMITER|FSK_PHASE_0|FSK_DF_90K|OUT_P_0);
	//konfiguracja pêtli PLL
	Rfm_xmit(PLL|PLL_DH_DIS|SEL_CLK_2_5|MAX_BAUD_256);

	//wy³¹czenie timera wakeup
	Rfm_xmit(WAKEUP_TIM|WUT_X_2_0|0);
	//wy³¹czenie oszczêdzania energii
	Rfm_xmit(LOW_DC|LOW_DC_DIS);
	//ustawienie monitora napiêcia na 2,2V
	Rfm_xmit(BOD_CLK|CLK_5_00|BOD_2_2);

	while(1){
		Rfm_write_str("Siemka", 0xD0);//wysy³am do naszego uk³adu slave powitanie
		Rfm_read_str(bufor,64);//odczytujê dane przesy³ane przez inny uk³ad
		uart_puts(bufor);//i wyœwietlamy je w terminalu RS232
		uart_puts("\r\n");
		_delay_ms(500);//czekamy nieco, ¿eby slave siê nie spoci³ :D
	}

	return 0;
}

#endif
