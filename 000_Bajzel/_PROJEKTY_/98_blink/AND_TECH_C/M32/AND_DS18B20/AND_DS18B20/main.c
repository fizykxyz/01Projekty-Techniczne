/*
 * AND_DS18B20.c
 *
 * Created: 2014-02-13 00:40:18
 *  Author: Piotr Rzeszut
 *
 * Description: Demonstracja obs³ugi termometru DS18B20
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "dallas_one_wire.h"
#include "HD44780.h"
#include "crc8.h"

int main(void)
{
	LCD_Initalize();
	LCD_GoTo(0,0);
	LCD_WriteText("  DS18B20 Demo  ");
	LCD_WriteData(0b11011111);
	
	uint8_t buffer[9];
	uint8_t ok;
	uint16_t measure;
	uint8_t subzero;
	uint8_t temp_int;
	uint16_t temp_fract;
	
    while(1)
    {
        ok=dallas_reset();//reset magistrali 1-wire
		if(!ok){//jeœli nie odpowiedzia³ ¿aden termometr to wyœwietlamy informacjê
			LCD_GoTo(0,1);
			LCD_WriteText(" NO THERMOMETER ");
			continue;
		}
		dallas_write_byte(SKIP_ROM_COMMAND);//pominiêcie weryfikacji numeru
		dallas_write_byte(CONVERT_T_COMMAND);//zlecamy konwersjê temperatury
		_delay_ms(750);//czekamy 750ms na zakoñczenie konwersji. _delay_ms() to niezbyt elegancki sposób
		//w rzeczysitsoœci wszelkie zale¿noœci czasowe zwi¹zane z pomiarami temperatury powinny byæ oparte na timerach i przerwaniach
		dallas_reset();//reset magistrali 1-wire
		dallas_write_byte(SKIP_ROM_COMMAND);//pominiêcie weryfikacji numeru
		dallas_write_byte(READ_SCRATCHPAD_COMMAND);//zlecamy odczyt danych
		dallas_read_buffer(buffer,9);//odczytujemy dane z termometru
		if(buffer[8]!=crc8(buffer,8)){//sprawdzamy sumê kontroln¹ odczytu
			LCD_GoTo(0,1);
			LCD_WriteText("   CRC8 ERROR   ");
			continue;
		}
		measure=(uint16_t)buffer[0]+(((uint16_t)buffer[1])<<8);//³¹czymy 2 bajty danych o temperaturze
		if(measure&0x8000){//jeœli wynik jest ujemny to zapisujemy informacjê o znaku i konwertujemy liczbê kodu U2 na dodatni¹
			subzero=1;
			measure ^= 0xFFFF;
			measure += 1;
		}else{
			subzero=0;
		}
		//rozdzielamy liczbê na czêœæ ca³kowit¹ i u³amkow¹
		temp_int=measure>>4;
		temp_fract=(measure&0x000F)*625;
		//wyœwietlamy znak, temperaturê
		LCD_GoTo(0,1);
		LCD_WriteText("T=");
		if(subzero){
			LCD_WriteData('-');
		}else{
			LCD_WriteData(' ');
		}
		sprintf((char*)buffer,"%03d.%04d",temp_int,temp_fract);
		LCD_WriteText((char*)buffer);
		LCD_WriteData(0b11011111);//wyœwietlamy znak stopnia
		LCD_WriteText("C   ");
		
    }
}