/*
 * ATMEGA32L_LCD main.c
 *
 *  Stworzono: 2014 17-09-2014
 *      Autor: D¹browski Tomasz
 *     Edycja: 22:35:04
 */


#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <string.h>
//#include "util/atomic.h"

#include "ADDS/tools.h"
#include "ADDS/timers.h"
#include "ADDS/w7seg.h"
#include "ADDS/charsets.h"



#include "UART/uart.h"
#include "HD44780/hd44780.h"

#include "PCD8544/pcd8544.h"
#include "ADDS/logonokia.h"

//#include "DS18B20/ds18b20.h"

//#include "1WIRE/1wire.h"
#include "ONEWIRE/onewire.h"
#include "PCD8544/font_verdana_14x14.h"
//#include "ADDS/serwo.h"

#include "PCF8583/pcf8583.h"

#include "I2C/i2c.h"
#define dtyg(y,m,d) (d+=m<3?y--:y-2,23*m/9+d+4+y/4-y/100+y/400)%7

uint8_t godz, min, sek, hsek;
uint8_t dzien, miesiac;
uint16_t rok;
char napis[16];
char* dnityg[]=
              {"Niedziela","Poniedzia³ek","Wtorek","Œroda","Czwartek","Pi¹tek","Sobota"};
#define NET 0
//**********************************LAN PRELOAD***************************

#if NET == 1
#include <stdlib.h>
#include "ENC28J60/network.h"
#include "ENC28J60/enc28j60.h"
#include "ENC28J60/enc28j60cfg.h"

	static uint8_t mymac[6] = {0x62,0x5F,0x70,0x72,0x61,0x79};
	static uint8_t myip[4] = {192,168,0,26};
	static uint16_t mywwwport = 80;

	#define BUFFER_SIZE 900
	uint8_t buf[BUFFER_SIZE+1],browser;
	uint16_t plen;

	void LAN_INIT(void)
	{
		//to by³o globalne poza procedurami

		//to by³o w main przed pêtl¹ nieskoñczonoœci
		    //CLKPR = (1<<CLKPCE);
		    //CLKPR = 0;
		    _delay_loop_1(50);
		    ENC28J60_Init(mymac);
		    ENC28J60_ClkOut(2);
		    _delay_loop_1(50);
		    ENC28J60_PhyWrite(PHLCON,0x0476);
		    _delay_loop_1(50);
		    init_network(mymac,myip,mywwwport);

	}

	void testpage(void) {
	    plen=make_tcp_data_pos(buf,0,PSTR("HTTP/1.0 200 OK\r\n"
	    "Content-Type: text/html\r\n"
	    "Pragma: no-cache\r\n\r\n"
	    "<meta http-equiv='refresh' content='2'/>"
	    "<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />"
	    "<html><head><title>Serwer temperatur</title></head>"
	    "<body>"
	    "<h3> Dzisiaj jest $S, "
	    " $S, czas:  $S, dzieññ œróba roku: $S dni do Sylwestra</h3>"
	    "<h4> Lokalne IP-$S, MAC=$S </h4>"
	    "<style type='text/css'>"
	    "/* <![CDATA[ */"
	    "td { font-size: 32; color: red; font-family: 'courier'}"
	    "/* ]]> */"
	    "</style>"
	    "<table border='3'>"
	    "<th>Nr</th><th>Identyfikator termometru</th><th>Temp.</th><th>Lokalizacja</th><th>Odczucie</th>"));
	    plen=make_tcp_data_pos(buf,plen,PSTR("<h1>It Works! i siurdaki lecom</h1></td></body></html>"));
	}

	void sendpage(void) {
	    tcp_ack(buf);
	    tcp_ack_with_data(buf,plen);
	}


// ***********************************LAN NEXT ***********************
#endif


//#define F_CPU 8000000 //ustawienie oscylatora na 8MHz
#define DS18S20_FAMILY_CODE       0x10
#define DS18B20_FAMILY_CODE       0x28
#define DS1822_FAMILY_CODE        0x22


#define DS_TMPINT(TMPBITS) (TMPBITS>>4)
#define DS_TMPFRAC(TMPBITS) ((TMPBITS & 0B1111)*625)

uint8_t pomiar[][8]={
 {0x28, 0xB4, 0x90, 0x1B, 0x03, 0x00, 0x00, 0x3A}
,{0x28, 0x24, 0x9E, 0x1B, 0x03, 0x00, 0x00, 0x29}
,{0x28, 0x49, 0xA0, 0x1B, 0x03, 0x00, 0x00, 0x86}
,{0x28, 0x9B, 0xAB, 0x1B, 0x03, 0x00, 0x00, 0x5C}
,{0x28, 0x59, 0x2C, 0xDC, 0x02, 0x00, 0x00, 0x23}
,{0x28, 0xFA, 0x91, 0x1B, 0x03, 0x00, 0x00, 0x91}
,{0x28, 0x76, 0x8F, 0x8B, 0x02, 0x00, 0x00, 0x92}
};

char* gdzieTemp[]={"28B4901B0300003A|Korytarz||","28249E1B03000029|Zewn¹trz|-5|30","2849A01B03000086|Piwnica||"
		,"289BAB1B0300005C|Piec||","28592CDC02000023|Gêœ||","28FA911B03000091|Holik||","28768F8B02000092|Sterownik||"};

char rom_code[16]; //bufor na hex znaku romcode
uint16_t last_temp[sizeof pomiar/sizeof pomiar[0]];
char trend;
#define interwal 360

uint16_t DS18X20_TMPBITS(uint8_t *nDSID)
{
	uint16_t tempc=85;


	// Wybierz urz¹dzenie do komunikacji

		// Wyœlij RESET
		if(!ONEWIRE_RESET())
		{
			// wyœlij ROM code
			ONEWIRE_WRITE(0x55,0);
			for(uint8_t i=0;i<8;i++){
				ONEWIRE_WRITE(nDSID[i],0);
			}

	// Wyœlij ¿¹danie konwersji
	ONEWIRE_WRITE(0x44,0);

	// Czekaj na zakoñczenie konwersji
	uint8_t wait = 255;
	while(wait)
	{
		wait --;
		_delay_us(3);

	}
	// Wybierz urz¹dzenie do komunikacji

			// Wyœlij RESET
			if(!ONEWIRE_RESET())
			{
				// wyœlij ROM code
				ONEWIRE_WRITE(0x55,0);
				for(uint8_t i=0;i<8;i++){
					ONEWIRE_WRITE(nDSID[i],0);
				}
	// Wyœlij ¿¹danie odczytu pamiêci podrêcznej
	ONEWIRE_WRITE(0xBE,0);
	// odczytaj 2 pierwsze bajty (rejestr temperatury)
	tempc = ONEWIRE_READ();
	tempc = tempc + (ONEWIRE_READ()<<8);
	//Konwersja kodów ujemnej temperatury
	if(tempc & 0x8000) tempc=1-(tempc ^ 0xFFFF);
}
		}
		return tempc;

}




void SEG7_INIT(void);

volatile uint32_t millis=0;
uint8_t szer=60;
uint8_t wys8=3;
//uint8_t gfxbuf[szer*wys8];
//uint8_t gfxbuf[180];

uint16_t now=0; //zmienna do odliczenia czasu aktualizacji wykresu
uint8_t OCTET;

char linia2[16];
char linia1[16];
uint8_t id[8];
char nap[2];
int16_t temp; //odczytana temperatura binarnie - do przetworzenia
uint8_t miarkownik=0;
//   inicjuje wybrane  urz¹dzenia
#define OW_SEARCH_FIRST 0xFF        // start new search
#define OW_PRESENCE_ERR 0xFF
#define OW_DATA_ERR     0xFE
#define OW_LAST_DEVICE  0x00        // last device found
#define OW_ROMCODE_SIZE 8


void INIT_DEVS(){
	UART_INIT();
	SEG7_INIT();
	HD44780_INIT();
	PCD8544_INIT();
	TIMER0_INIT();
	ONEWIRE_INIT();
	//SERWO1_INIT();
	sei();
}
// ************************  start programu **************************************

//void no_main1(void) { //rozpoczynamy
int main(void) { //rozpoczynamy

INIT_DEVS(); //   inicjuje wybrane  urz¹dzenia
HD44780_XY(0,0);HD44780_TEXT("BARDZO DLuuUGA   LINIA1");
HD44780_XY(0,1);HD44780_TEXT("TEZ DLUUUUUUGA LINIA2");
HD44780_XY(0,2);HD44780_TEXT("LINIA3");
HD44780_XY(0,3);HD44780_TEXT("LINIA4");
_delay_ms(3000);

for (uint8_t d=0;d<128;d++){
	HD44780_XY(d%20,2);HD44780_CHAR(d);
	HD44780_XY(d%20,3);HD44780_CHAR(d+128);

	_delay_ms(300);
}

i2cInit();
    PCF8583_init();
//    PCF8583_set_time( 19, 17, 1, 0 );
//PCF8583_set_date( 04, 11, 2014 );

//PCD8544_GFX2LCD(FLASH,misio,0,0,84,6);
HD44780_CUSTOMCHARSET(HD44780_CHARSPLW);
_delay_ms(4000);
PCD8544_CONTRAST(0x7F);
PCD8544_TEXT("HALO!",normal);
HD44780_CLR();
#if NET == 1
//****************************Pocz¹tek LAN w mainie	 *************************
uint16_t dat_p;
   LAN_INIT();
#endif

 // *************************** PÊTLA PROGRAMU *********************************
while(1){
	uint8_t numer=0;

	while (ONEWIRE_SEARCH(id) != 0) {
	 			for (uint8_t i=0;i<8;i++){
				OCTET=id[i]; //odczyt romu bajt po bajcie
				rom_code[i*2]=NIBBLE2HEX((OCTET>>4 & 0x0F));
				rom_code[i*2+1]=NIBBLE2HEX((OCTET & 0x0F));
				}
		// UART_SENDSTR(rom_code);
		// UART_SENDSTR(NL NL "nastepny" NL);
		numer++;
		UART_SEND(numer+48);
	}

	// UART_SENDSTR(NL "No more addresses."NL);
		    ONEWIRE_RESETSEARCH();
		    numer=0;
	// UART_SENDSTR(NL NL);
	_delay_ms(200);

for (uint8_t nr=0;nr<7;nr++){
	temp=DS18X20_TMPBITS(pomiar[nr]);
	if ((millis/1000)%interwal==0){
		last_temp[nr]=temp;
		//now=millis;
	}
	if ((DS_TMPINT(temp)>=DS_TMPINT(last_temp[nr])) && ((DS_TMPFRAC(temp)>DS_TMPFRAC(last_temp[nr]))))
	{
		trend=126;
	}
	else if ((DS_TMPINT(temp)==DS_TMPINT(last_temp[nr])) && ((DS_TMPFRAC(temp)==DS_TMPFRAC(last_temp[nr]))))
	{
		trend='=';
	}
	else
	{
		trend=127;
	}
//	if (nr==3){
//		SERWO1(3*(DS_TMPINT(temp)-20));
//	}

//OCR1B=((millis/30)%1000)*2+500;

	char subbuff[6];
	memcpy( subbuff, &gdzieTemp[nr][17], 5 );
	subbuff[5] = '\0';
sprintf(linia1,"%s.%+03i%c%02i",subbuff,DS_TMPINT(temp),trend,DS_TMPFRAC(temp)/100);
sprintf(BUF_7SEG,"%02i%01i%c",DS_TMPINT(temp),DS_TMPFRAC(temp)/1000,trend);
// UART_SENDSTR(linia1);
// UART_SENDSTR(" st.C " NL);
HD44780_XY(0,nr%HD44780_H);
HD44780_TEXT(linia1);
PCD8544_XY8(0,nr%4);
PCD8544_TEXT(linia1,trail2|bold);
//PCD8544_GFXROT(rot180);
//PCD8544_GFXCHAR(gfxbuf,size1x,font_verdana_14x14,16,8,szer,wys8,'¥',normal);
//PCD8544_GFXTEXT(gfxbuf,size1x,font_verdana_14x14,0,7,szer,wys8,"12.34",normal);
////PCD8544_GFXTEXT(gfxbuf,size1x,font_test_10x16,0,0,szer,wys8," !",normal);
////PCD8544_GFXCHAR(gfxbuf,size3x,FONT_5x8,15,10,szer,wys8,'d',normal);
//
////PCD8544_GFXCHAR(gfxbuf,normal,FONT_5x8,10,13,szer,wys8,'Ê',normal);
//
//PCD8544_GFX2LCD(RAM,gfxbuf,0,0,szer,wys8);
PCF8583_get_time( &godz, &min, &sek, &hsek );  //pobranie czasu
PCF8583_get_date( &dzien, &miesiac, &rok );  //pobranie daty

_delay_ms(700);
//PCD8544_XY8(79,0);
//PCD8544_CHAR(48+godz/10,normal);
//PCD8544_XY8(79,1);
//PCD8544_CHAR(48+godz%10,normal);
//PCD8544_XY8(79,2);
//PCD8544_CHAR(48+min/10,normal);
//PCD8544_XY8(79,3);
//PCD8544_CHAR(48+min%10,normal);
//PCD8544_XY8(79,4);
//PCD8544_CHAR(48+sek/10,normal);
//PCD8544_XY8(79,5);
//PCD8544_CHAR(48+sek%10,normal);
//PCD8544_CHAR(48+hsek/10,normal);
//PCD8544_CHAR(48+hsek%10,normal);

uint8_t dzient=dzien;
uint8_t miesiact=miesiac;
uint16_t rokt=rok;

memcpy( subbuff, &dnityg[dtyg(rok,miesiac,dzien)][0], 4 );
subbuff[4] = '\0';
sprintf(napis,"%04u-%02d-%02d %s",rokt,miesiact,dzient,subbuff);

PCD8544_XY8(0,4);
PCD8544_TEXT(napis,normal);
sprintf(napis,"%02d:%02d:%02d",godz,min,sek);
//for (uint8_t y=0;y<6;y++){
PCD8544_XY8(2,5);
PCD8544_TEXT(napis,trail3|bold|under|whole);
//}

}

#if NET == 1
	//****************************Pocz¹tek LAN w while(1)*************************
	    	plen = ENC28J60_PacketReceive(BUFFER_SIZE,buf);
	        if(plen==0) continue;
	        if(eth_is_arp(buf,plen)) {
	            arp_reply(buf);
	            continue;
	        }
	        if(eth_is_ip(buf,plen)==0) continue;
	        if(buf[IP_PROTO]==IP_ICMP && buf[ICMP_TYPE]==ICMP_REQUEST) {
	            icmp_reply(buf,plen);
	            continue;
	        }
	        if(buf[IP_PROTO]==IP_TCP && buf[TCP_DST_PORT]==0 && buf[TCP_DST_PORT+1]==mywwwport) {
	            if(buf[TCP_FLAGS] & TCP_SYN) {
	                tcp_synack(buf);
	                continue;
	            }
	            if(buf[TCP_FLAGS] & TCP_ACK) {
	                init_len_info(buf);
	                dat_p = get_tcp_data_ptr();
	                if(dat_p==0) {
	                    if(buf[TCP_FLAGS] & TCP_FIN) tcp_ack(buf);
	                    continue;
	                }

	                if(strstr((char*)&(buf[dat_p]),"User Agent")) browser=0;
	                else if(strstr((char*)&(buf[dat_p]),"MSIE")) browser=1;
	                else browser=2;

	                if(strncmp("/ ",(char*)&(buf[dat_p+4]),2)==0){
	//****************************** TU MÓJ PROGRAM/PRZYGOTOWANIE HTML****************



//**************************TU MIESZAÆ ********************************

//**********************PRZESZTAÆ MIESZAÆ !!!************************

	testpage();

//********************************kontynuacja sekcji LAN,wys³anie strony **********
    sendpage();
    continue;
}
}
}
#endif
}
// ************************** KONIEC PÊTLI G£ÓWNEJ *********************************

}

// ************************** KONIEC PROGRAMU **************************************


//do³¹czenie ob³ugi przerwañ dla LED 7 segmentów
#include "ADDS/isrt0c_w7seg.h"

//procedura inicjacji 7SEG
void SEG7_INIT(void){
	SEG7DDR = 255; //ca³y port jako wyjœciowy
	for (uint8_t i=0;i<sizeof segm;i++){
	SEG7DDRSEG |= (1<<segm[i]); //ustawiena portów segmentów równie¿ jako wyjœcia
	}
}

