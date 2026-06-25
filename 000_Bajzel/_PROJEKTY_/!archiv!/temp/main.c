/*
 * 01_ARRAY_PINS main.c
 *
 *	    Autor: Dπbrowski Tomasz
 *  Stworzono: 2016-08-01
 *     Edycja: 08:28:46
 */



#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>

#include "ADDS/tools.h"
#include "HD44780/hd44780.h"
#include "ADDS/memcheck.h"
#include "onewire.h"
//#include "ds18x20.h"

//
//
////************************** PROGRAM W£AåCIWY - WYKORZYSTANIE ******************
//
//
//
//
THD44780 wysw[]=
{
//			{16,2,G2O(PA,7)},
//			{16,2,G2O(PA,6)},
//			{16,4,G2O(PA,5)},
//			{20,4,G2O(PA,4)},
			{16,2,G2O(PA,7)}
};

uint8_t nrLed;
uint16_t memo;
char buf[10];

#define blinkLED G2O(PD,7)
#define T_LEDON 300
#define T_BLINK 200  // czas pomiÍdzy przejúciami pÍtli g≥Ûwnej/miganiem diody
#define T_INTRO 500 // czas pomiÍdzy ekranami intro

//
//#define debug
//
const PROGMEM uint8_t HD44780_CHARSPLW[64]   =
{
	0x0E, 0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x02, /* \0 = • */
	0x02, 0x0E, 0x15, 0x10, 0x10, 0x11, 0x0E, 0x00, /* \1 = ∆ */
	0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F, 0x02, /* \2 =   */
	0x10, 0x12, 0x14, 0x18, 0x10, 0x10, 0x1F, 0x00, /* \3 = £ */
	0x02, 0x15, 0x19, 0x15, 0x13, 0x11, 0x11, 0x00, /* \4 = — */
	0x02, 0x0E, 0x15, 0x11, 0x11, 0x11, 0x0E, 0x00, /* \5 = ” */
	0x04, 0x0E, 0x10, 0x0E, 0x01, 0x11, 0x0E, 0x00, /* \6 = å */
	0x04, 0x1F, 0x09, 0x02, 0x04, 0x08, 0x1F, 0x00  /* \7 =èØ */
/*	0x1F, 0x01, 0x02, 0x1F, 0x08, 0x10, 0x1F, 0x00    \7 = Ø */
/*	0x04, 0x1F, 0x09, 0x02, 0x04, 0x08, 0x1F, 0x00,        è */
};




uint8_t znak[]  =
		  {
				  0b00100,
				  0b01010,
				  0b01010,
				  0b01110,
				  0b01110,
				  0b11111,
				  0b11111,
				  0b01110
		  };


uint8_t ids_count=0;
uint8_t ID[8];
uint8_t DS[8]={0x28,0x59,0x2C,0xDC,0x02,0x00,0x00,0x23};

//uint8_t  a, b, tt;

// Model IDs
#define DS18S20MODEL 0x10
#define DS18B20MODEL 0x28
#define DS1822MODEL  0x22

// OneWire commands
#define STARTCONERT     0x44  // Tells device to take a temperature reading and put it on the scratchpad
#define COPYSCRATCH     0x48  // Copy EEPROM
#define READSCRATCH     0xBE  // Read EEPROM
#define WRITESCRATCH    0x4E  // Write to EEPROM
#define RECALLSCRATCH   0xB8  // Reload from last known
#define READPOWERSUPPLY 0xB4  // Determine if device needs parasite power
#define ALARMSEARCH     0xEC  // Query bus for devices with an alarm condition

// Scratchpad locations
#define TEMP_LSB        0
#define TEMP_MSB        1
#define HIGH_ALARM_TEMP 2
#define LOW_ALARM_TEMP  3
#define CONFIGURATION   4
#define INTERNAL_BYTE   5
#define COUNT_REMAIN    6
#define COUNT_PER_C     7
#define SCRATCHPAD_CRC  8

// Device resolution
#define TEMP_9_BIT  0x1F //  9 bit
#define TEMP_10_BIT 0x3F // 10 bit
#define TEMP_11_BIT 0x5F // 11 bit
#define TEMP_12_BIT 0x7F // 12 bit

#define DS18X20_OK                0x00
#define DS18X20_ERROR             0x01
#define DS18X20_START_FAIL        0x02
#define DS18X20_ERROR_CRC         0x03

// Error Codes
#define DEVICE_DISCONNECTED -127

uint8_t DS18xxx_PARASITE(uint8_t *ID)
{
	  uint8_t PARASITE = 0;
	  ow_reset();
	  //OW_RESETPULSE();
	  ow_command(READPOWERSUPPLY,ID);
	  //OW_COMMAND(ID, READPOWERSUPPLY, 0);
	  if ( ow_bit_io(1) == 0) PARASITE = 1;
//	  if (_OW_IO1BIT(1, 0) == 0) PARASITE = 1;
	  ow_reset();
//	  OW_RESETPULSE();
	  return PARASITE;
}

uint8_t DS18xxx_MEASURE(uint8_t *ID, uint8_t PARASITE)
{
	uint8_t ret;
	ow_reset();
//	OW_RESETPULSE();
//	if( READ(OW_PIN))
//	{ // only send if bus is "idle" = high
			ow_command_with_parasite_enable(STARTCONERT,ID);
	//		OW_COMMAND(ID, STARTCONERT, PARASITE);
		ret = 0; //DS18X20_OK;
//	}
//	else
//	{
////		uart_puts_P_verbose( "DS18X20_start_meas: Short Circuit!\r" );
//		ret = DS18X20_START_FAIL;
//	}

	return ret;
}

uint8_t DS18xxx_READSCRATCH(uint8_t *ID, uint8_t PARASITE)
{
	uint16_t temp;
	ow_reset();
	ow_command_with_parasite_enable(READSCRATCH,ID);
//	OW_COMMAND(ID, READSCRATCH, PARASITE);
	temp = ow_byte_rd(); //_OW_R8BIT();
	temp = (ow_byte_rd()<<8)|temp;
	temp = temp*0.0625;
	return (uint8_t) temp;
}




int8_t readtemp(uint8_t *ID, uint8_t PARASITE)
{
	DS18xxx_MEASURE(ID, PARASITE);

//uint16_t tconv=75;
//while (tconv--)
//{
//	uint8_t tmp=0; //zmienna tymczasowa do rolowania pionowego
//	tmp=znak[0];
//	for(uint8_t i=0;i<8;i++)
//	{
////		znak[i]=(znak[i]>>1)|(znak[i]<<4); //scroll poziomy z lewa na prawo
//		znak[i]=znak[i+1]; //scroll pionowy
//	}
//	znak[7]=tmp;
//		HD44780_CUSTOMCHAR(0,znak);
//	_delay_ms(10);
//}
   _delay_ms(950);
 	return DS18xxx_READSCRATCH(ID, PARASITE);
 }




uint8_t lcd,x,y;

void movecursor()
{
	y++;
	if (y>=wysw[lcd].LIN)
		{
		y=0;
		if ((31+x)<(wysw[lcd].KOL))
		{
		x+=16;
		}
		else
		{
		lcd++;
		if (lcd>=SIZE(wysw)) lcd=0;
		HD44780_NR(lcd,wysw);
			HD44780_CUSTOMCHAR(0,znak);
		x=0;
		}

		}
HD44780_XY(x,y);
}

int main(void)  //rozpoczynamy
{
	OUTPUT(blinkLED);
	LOW(blinkLED);
//
//	DDRD=0xff;
//	PORTD=0xff;

	HD44780_START(wysw,SIZE(wysw));
	HD44780_NR(0,wysw);
	HD44780_CUSTOMCHARSET(HD44780_CHARSPLW);
	HD44780_XY(2,0);

//	HD44780_XY(0,0);
//	HD44780_TEXT("01234567890123456789");
//  HD44780_XY(0,1);
//	HD44780_TEXT("01234567890123456789");
//	HD44780_XY(0,2);
//	HD44780_TEXT("01234567890123456789");
//	HD44780_XY(0,3);
//	HD44780_TEXT("01234567890123456789");
//	while(1);
//



	HD44780_TEXT("uC-");
    HD44780_TEXT(strupr(STRING(MCU))); //HD44780_CHAR('0'+strlen(STRING(MCU)));
    HD44780_XY(2,1);
    HD44780_TEXT("@ ");
	HD44780_TEXT(STRING(F_CPU));
	HD44780_TEXT(" Hz");
	_delay_ms(T_INTRO);
	HD44780_XY(0,2);
	HD44780_TEXT("RAM:");
	itoa(RAMEND-RAMSTART,buf,10);
	HD44780_TEXT(buf);
	HD44780_TEXT(" bytes");
	HD44780_XY(0,3);
	HD44780_CHAR('0'+SIZE(wysw));
	HD44780_TEXT(strupr(" wy\x06wietlacz|ey"));
//	itoa(get_mem_unused(),buf,10);
//	itoa(RAMEND-RAMSTART,buf,10);
//	HD44780_TEXT(buf);
	_delay_ms(T_INTRO);
	HD44780_CLR();
	HD44780_CHAR('v');
	HD44780_TEXT(STRING(__DATE__));
	HD44780_XY(0,1);
	HD44780_TEXT(STRING(__TIME__));
	_delay_ms(T_INTRO);
	HD44780_NR(0,wysw);
    HD44780_CLR();
    HD44780_XY(10,0);
    HD44780_TEXT(itoa(readtemp(DS,1),buf,10));

	ow_set_bus(&PINA,&PORTA,&DDRA,PA2);
    ow_reset();
    ow_command_with_parasite_enable( STARTCONERT, DS );
        _delay_ms( 750 );
         ow_reset();
        ow_command( READSCRATCH, DS );
        uint8_t a=ow_byte_rd();
        uint8_t b=ow_byte_rd();
        uint16_t w=b<<8|a;
        w=w>>4;
        itoa(w,buf,10);
        HD44780_TEXT(buf);

    _delay_ms(2000);
    while(1) //pÍtla nieskoÒczona
	{

    	ow_reset();
    	    ow_command_with_parasite_enable( STARTCONERT, DS );
    	        _delay_ms( 750 );
    	       //  ow_reset();
    	        ow_command( READSCRATCH, DS );
    	        uint8_t a=ow_byte_rd();
    	        uint8_t b=ow_byte_rd();
    	        HD44780_XY(0,1);
    	   	 HD44780_CHAR(NIBBLE2HEX(b>>4));
    	     HD44780_CHAR(NIBBLE2HEX(b&0x0F));
    	     HD44780_CHAR(NIBBLE2HEX(a>>4));
    	     HD44780_CHAR(NIBBLE2HEX(a&0x0F));
    	        uint16_t w=b<<8|a;
    	        w=w>>4;
    	        itoa(w,buf,10);
    	        HD44780_HOME();
    	        HD44780_TEXT(buf);






//		ids_count=0;
//		OW_RESETSEARCH();
//			 while(OW_SEARCH(ID) == OW_OK)
//			  {
//				 int8_t temp=readtemp(ID,0);
//				 itoa(temp,buf,10);
//				 ids_count++;
//			   for (uint8_t i=0;i<8;i++)
//			   		 {
//				   	   	 HD44780_CHAR(NIBBLE2HEX(ID[i]>>4));
//			   			 HD44780_CHAR(NIBBLE2HEX(ID[i]&0x0F));
//			   		 }
//
//			   movecursor();
//			   HD44780_CHAR(0);
//			   HD44780_TEXT("Temp");
//			   HD44780_CHAR('0'+ids_count);
//			   HD44780_TEXT(":");
//			   HD44780_TEXT(buf);
//			   HD44780_TEXT(" PAR:");
//			   HD44780_CHAR('0'+DS18xxx_PARASITE(ID));
//			   movecursor();

			  }


//		 HD44780_TEXT("A");
	_delay_ms(T_BLINK);
//PORTD^=0xff;
					   TOGGLE(blinkLED);





//		for(nrLed=0;nrLed<SIZE(wysw);nrLed++)
//			{
//			HD44780_NR(nrLed,wysw);
//			HD44780_XY(3,0);
//			HD44780_TEXT(("WY\x06WIETLACZ NR "));
//			HD44780_CHAR('0'+nrLed);
//			HD44780_XY(0,1);
//			HD44780_CHAR('0'+nrLed);
//			HD44780_XY(0,2);
//			HD44780_CHAR('0'+nrLed);
//			HD44780_XY(0,3);
//			HD44780_CHAR('0'+ids_count);
//			HD44780_HOME();
//			int8_t temp=readtemp();
//			//uint8_t temp= OW_RESETPULSE();
//			itoa(temp,buf,10);
//			HD44780_TEXT(buf);
////			HD44780_CHAR('0'+OW_RESETPULSE());
//			_delay_ms(500);
//			}


//	}
}


