/*
 * ATMEGA32L_LCD DS18B20.c
 *
 *  Stworzono: 2014 18-09-2014
 *      Autor: D¹browski Tomasz
 *     Edycja: 21:54:56
 */



#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>

#include "ds18b20.h"

/* Timing issue when using runtime-bus-selection (!OW_ONE_BUS):
   The master should sample at the end of the 15-slot after initiating
   the read-time-slot. The variable bus-settings need more
   cycles than the constant ones so the delays had to be shortened
   to achive a 15uS overall delay
   Setting/clearing a bit in I/O Register needs 1 cyle in OW_ONE_BUS
   but around 14 cyles in configureable bus (us-Delay is 4 cyles per uS) */


uint8_t ow_bit_io_intern( uint8_t b)
{

	//cli();
	SET(DDR,WIRE1_PIN);    // drive bus low
		_delay_us(2);    // T_INT > 1usec accoding to timing-diagramm
		if ( b ) {
			CLR(DDR,WIRE1_PIN); // to write "1" release bus, resistor pulls high
		}
		// "Output data from the DS18B20 is valid for 15usec after the falling
		// edge that initiated the read time slot. Therefore, the master must
		// release the bus and then sample the bus state within 15ussec from
		// the start of the slot."
		_delay_us(15-2-OW_CONF_DELAYOFFSET);

		if( (GET(WIRE1_PIN)) == 0 ) {
			b = 0;  // sample at end of read-timeslot
		}

		_delay_us(60-15-2+OW_CONF_DELAYOFFSET);

		CLR(DDR,WIRE1_PIN);



	//sei();

	_delay_us(OW_RECOVERY_TIME); // may be increased for longer wires

	return b;
}


uint8_t ow_bit_io( uint8_t b )
{
	return ow_bit_io_intern( b & 1);
}

uint8_t ow_byte_wr( uint8_t b )
{
	uint8_t i = 8, j;

	do {
		j = ow_bit_io( b & 1 );
		b >>= 1;
		if( j ) {
			b |= 0x80;
		}
	} while( --i );

	return b;
}


uint8_t ow_byte_rd( void )
{
	// read by sending only "1"s, so bus gets released
	// after the init low-pulse in every slot
	return ow_byte_wr( 0xFF );
}


uint8_t WIRE1_RESET(void)
{
	//cli();
uint8_t presence;
CLR(PORT,WIRE1_PIN);            // stan niski
SET(DDR,WIRE1_PIN);              // wyjœcie
_delay_us(480);       // na 500us - wg noty 480us

//ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
CLR(DDR,WIRE1_PIN);                // wejœcie
_delay_us(64);        // czekamy na odpowiedŸ min. 60us, my czekamy 70us-72us wg noty 64-66

//SET(PORT,WIRE1_PIN);           // wywaliæ zwalnia magistrale ustawia stan pocz¹tkowy czyli wysoki
	presence = GET(WIRE1_PIN);   // no presence detect if err!=0: nobody pulled to low, still high
////	} //ATOMIC

_delay_us(480-64);// czekamy na zakoñczenie trwania PRESENCE IMPULS /440us wg noty,480-64

if ((GET(WIRE1_PIN)) == 0){          // jezeli DS ustawi³ stan niski
presence = 1;
}
// ustaw zmienn¹ na 1 - brak urz¹dzeñ /ERR1
//else                         // je¿eli nie odpowiedzia³
//presence = 0;          // ustaw zmienn¹ na 0 - s¹ urz¹dzenia /ERR0
//sei();
return presence; // zwróæ presence: 1-uk³ad jest;  0-uk³adu nie ma
}

uint8_t WIRE1_READBIT(void)
{
	uint8_t BIT;
	SET(DDR,WIRE1_PIN); // ustaw  jako wyjscie
	CLR(PORT,WIRE1_PIN); // ustaw PIN w stan 0 (start timeslot)
	//-delay(2);						// odczekaj
	CLR(DDR,WIRE1_PIN); // ustaw pin jako wejscie
	_delay_ms(15); 						// delay 15us from start of timeslot
	if(!(GET(WIRE1_PIN)))          // sprawdz sygnal na pinie
			BIT = 1;
	else
			BIT = 0;

	return BIT;					// return value of DQ line
}



void WIRE1_WRITEBIT(uint8_t BIT)
{
	SET(DDR,WIRE1_PIN);			// ustaw jako wyjscie
	CLR(PORT,WIRE1_PIN);			// ustaw pin w stan 0 (start timeslot)
	asm ("nop");
	if(BIT==1)
	{
	       CLR(PORT,WIRE1_PIN);            // stan niski
	       _delay_us(7);           // czekamy 7us
	       SET(PORT,WIRE1_PIN);            // stan wysoki
	       _delay_us(70);         // czekamy do koñca write time slot
	}
	     else                             // je¿eli trzeba wys³aæ 0
    {
	    	 CLR(PORT,WIRE1_PIN);          // stan niski
	        _delay_us(70);         // czekamy 70us
	        SET(PORT,WIRE1_PIN);             // stan wysoki
	        _delay_us(7);           // czekamy 7us do nastêpnego write time slot
	}

}



uint8_t WIRE1_READ(void)
{
  uint8_t i, OCTET = 0;             // zmienne: licznikowa oraz przechowuj¹ca odebrany bajt

  SET(DDR,WIRE1_PIN);   // pin jako wejœcie
  cli();
  for (i=0; i<8; i++)                        // pêtla wykonuje siê 8 razy odbiera bit po bicie
{                                            //           zaczynaj¹c od najstarszego /*
     CLR(PORT,WIRE1_PIN);                             // stan niski
     _delay_us(3);                            // czekamy 3us
     SET(PORT,WIRE1_PIN);                             // stan wysoki - zwalniamy magistrale
     _delay_us(7);                            // czekamy 7us

     if(GET(WIRE1_PIN)) OCTET |= 0x01<<i;     // je¿eli stan wysoki to ustaw bit 1, po czym przesuñ
                                    //                       w lewo, ¿eby ten pierwszy by³ najstarszy */
     _delay_us(70);                          // czekaj na zakoñczenie Read Time Slot
  }
  sei();
  return OCTET;                                 // zwróæ odebrany bajt
}

void WIRE1_WRITE(uint8_t OCTET)
{
   uint8_t i;            // zmienna licznikowa
   SET(DDR,WIRE1_PIN);
   cli();
   for (i=0; i<8; i++)        // pêtla wykonuje siê 8 razy, aby wys³aæ bit po bicie ca³y bajt
   {
     if (OCTET & 0x01)         // je¿eli trzeba wys³aæ 1
     {
       CLR(PORT,WIRE1_PIN);            // stan niski
       _delay_us(7);           // czekamy 7us
       SET(PORT,WIRE1_PIN);            // stan wysoki
       _delay_us(70);         // czekamy do koñca write time slot
     }
     else                             // je¿eli trzeba wys³aæ 0
     {
    	 CLR(PORT,WIRE1_PIN);          // stan niski
        _delay_us(70);         // czekamy 70us
        SET(PORT,WIRE1_PIN);             // stan wysoki
        _delay_us(7);           // czekamy 7us do nastêpnego write time slot
     }

OCTET >>= 1;                // przesuwamy bity w prawo, aby znów wys³aæ kolejny bit; dzia³a tak:
                           // je¿eli mamy bajt: 01110101 to po wykonianiu byte >>=1 bajt staje siê taki:
                          //  00111010 /*
   }
   sei();
}



//Szuka urzadzen 1Wire i uaktywnia N-te urzadzenie/////
uint8_t WIRE1_USEDEVICE(uint8_t USEIT)
{
uint8_t bit,bit_complementary,pozycja,i,buffer[USEIT+1];
for(i=0;i<USEIT+1;i++) {buffer[i]=0;}

for(i=0;i<USEIT;i++) {
	pozycja=0;
	//if(~WIRE1_RESET()) return 0;
	WIRE1_WRITE(DS18B20_SEARCHROM);
	    uint8_t bitNo;
	    for(bitNo=0;bitNo<64;bitNo++) {
		bit=WIRE1_READBIT();
		bit_complementary=WIRE1_READBIT();
			if ((~(bit|bit_complementary))&1) {
				if(buffer[pozycja+1]==0) {
				buffer[pozycja]++;
				}
			WIRE1_WRITEBIT((buffer[pozycja]-1));
			pozycja++;
			} else {
			WIRE1_WRITEBIT(bit);
			}
		}
		while(buffer[pozycja-1]==2) {
		buffer[pozycja]=0;
		pozycja--;
		}
	}
	return 1;
}

void WIRE1_GETID(char ID[],uint8_t STR){
	uint8_t OCTET;
	if(WIRE1_RESET()){
				WIRE1_WRITE(DS18B20_READROM); // Wys³anie rozkazu odczytu romu
				for (uint8_t i = 0; i < 8; i++) {
				OCTET=WIRE1_READ(); //odczyt romu bajt po bajcie
				if (STR){
		    	ID[i*2]=NIBBLE2HEX((OCTET>>4 & 0x0F));
                ID[i*2+1]=NIBBLE2HEX((OCTET & 0x0F));
				}
				else{
				ID[i]=OCTET;
				}
			}
}
}

int16_t DS18B20_TMPBITS(void){
     uint8_t WIRE1_READY;
    int16_t DS_TEMP=0;
    if(WIRE1_RESET())  // je¿eli uk³ad jest na linii i odpowiedzia³
		{
		    WIRE1_WRITE(DS18B20_SKIPROM);        // SKIP ROM
		    WIRE1_WRITE(DS18B20_CONVERTTEMP);       // CONVERT T
		    WIRE1_READY = 1;        // ustawiamy zmienn¹ oznaczaj¹c¹ gotowoœæ uk³adu bêdzie
		                          //                        potrzebna póŸniej */
		}
		else                                        // je¿eli uk³ad nie odpowiedzia³
		WIRE1_READY = 0;                   // ustaw zmienn¹ gotowoœci na 0


		_delay_ms(250);                  // czekamy na zakoñczenie konwersji temperatury; fabrycznie
		_delay_ms(250);                 //   konwersja ustawiona jest na 12bit, która trwa do 750ms;
		_delay_ms(250);                 //  my poczekamy 800ms

		if(WIRE1_READY == 1)            // je¿eli uk³ad by³ na linii
		{
		    if(WIRE1_RESET())         // trzeba ponownie zresetowaæ uk³ad i je¿eli odpowiedzia³
		    {
		        WIRE1_WRITE(DS18B20_SKIPROM); // SKIP ROM
		        WIRE1_WRITE(DS18B20_RSCRATCHPAD); // READ SCRATCHPAD
		        DS_TEMP = WIRE1_READ(); // czytaj 2 bajty temperatury
		       DS_TEMP =DS_TEMP+(WIRE1_READ()<<8);

		       //Konwersja kodów ujemnej temperatury
		        	if(DS_TEMP & 0x8000) DS_TEMP=1-(DS_TEMP ^ 0xFFFF);
		        WIRE1_RESET();  // reset - koniec czytania
		    }


		}
		return DS_TEMP;
}

uint8_t WIRE1_ROM_SEARCH( uint8_t diff, uint8_t id[] )
{
	uint8_t i, j, next_diff;
	uint8_t b;

	if(WIRE1_RESET()) {
		return OW_PRESENCE_ERR;         // error, no device found <--- early exit!
	}

	ow_byte_wr(DS18B20_SEARCHROM );        // ROM search command
	next_diff = OW_LAST_DEVICE;         // unchanged on last device

	i = OW_ROMCODE_SIZE * 8;            // 8 bytes

	do {
		j = 8;                          // 8 bits
		do {
			b = ow_bit_io( 1 );         // read bit
			if( ow_bit_io( 1 ) ) {      // read complement bit
				if( b ) {               // 0b11
					return OW_DATA_ERR; // data error <--- early exit!
				}
			}
			else {
				if( !b ) {              // 0b00 = 2 devices
					if( diff > i || ((*id & 1) && diff != i) ) {
						b = 1;          // now 1
						next_diff = i;  // next pass 0
					}
				}
			}
			ow_bit_io( b );             // write bit
			*id >>= 1;
			if( b ) {
				*id |= 0x80;            // store bit
			}

			i--;

		} while( --j );

		id++;                           // next byte

	} while( i );

	return next_diff;                   // to continue search
}

