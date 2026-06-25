/*
 * ATMEGA32L_LCD onewire.c
 *
 *  Stworzono: 2014-10-13
 *      Autor: D¹browski Tomasz
 *     Edycja: 06:55:32
 *     //TODO: rozbudowa o obs³ugê (odczyt, ustawienie) parasite mode, wykrywanie b³êdów magistrali i szuk. alarmów
 *
 * je¿eli by³by problem rozwa¿yæ u¿ycie podstawowych operacji zabis/odczyt bitu/oktetu z biblioteki:
 */

/************************************************************************/
/*                                                                      */
/*        Access Dallas 1-Wire Device with ATMEL AVRs                   */
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                      danni@specs.de                                  */
/*                                                                      */
/* modified by Martin Thomas <eversmith@heizung-thomas.de> 9/2004       */
/************************************************************************/


#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>

#include "../ADDS/pins.h"
#include "onewire_cfg.h"
#include "onewire.h"

uint8_t OW_ERROR;
uint8_t _ow_LastDevice;
uint8_t _ow_LastDiscrepency;
uint8_t _ow_LastFamilyDiscrepency;
uint8_t *_ow_last_buffer;


//tzw presence reset czyli: MASTER wysy³a RESET PULSE, a jeœli jest/s¹ SLAVE na linii odsy³aj¹ PRESENCE PULSE
//, niby czy coœ jest czy nie na magistrali. Obecnie nawet masê pod³. na sta³e uzna za ok,
//TODO: wykonaæ procedurê tak, aby faktycznie spr. 1Wire, mo¿e pomiar czasów odpowiedzi, albo jakoœ tak
//warto przestudowaæ http://www.jaglarz.info/ja/elektronika/ds18b20.pdf i tajmingi z dok.1Wire
//uint8_t OW_RESETPULSE() // reset magistrali //static uint8_t onewire_reset() i tu bêdzie problem !! rozjebany
//{
//	 //ONEWIRE_DDR |= (1<<ONEWIRE_PIN);
//	 OUTPUT(OW_PIN);
//	 _delay_us(480);
//	 //ONEWIRE_DDR &= ~(1<<ONEWIRE_PIN);
//	 INPUT(OW_PIN);
//	 _delay_us(70); //to i ostatni powinien daæ 480 us, a ten w zakresie 15-60, 70 pewna
//
//	 // chcemy negowaæ bit, wiêc powinno byæ 0->1, 1->0. ale to jest bajt
//	 //czyli 0b00000001->0b11111110 (1->254), a 0b00000000->0b11111111 (0->255)
//	 //uint8_t ret = ~(ONEWIRE_PORT & (1<<ONEWIRE_PIN)); //org daje 251 przy od³¹czonej magistrali,255 z urz.
//	 //tu tak jest bo jest sprawdzana negacja WAGI bitu!!!
//	 //uint8_t ret = ~(READ(OW_PIN)); // moje 254 przy braku ,przy w³¹czonym urz. daje 255, czyli niby ok
//	 uint8_t PRESENCE = !(READ(OW_PIN)); // a tak dzia³a mimo, ¿e tak nie wolno bo to operand zaprzeczenia !
//	 //uint8_t ret = 1-READ(OW_PIN); //tu jest ok (0 jak brak urz. na linii, 1 jak s¹)
//	 _delay_us(410);
//	 return PRESENCE;
//}
uint8_t OW_RESETPULSE() // reset magistrali //static uint8_t onewire_reset() i tu bêdzie problem !! rozjebany
{
		uint8_t PRESENCE;
		LOW(OW_PIN);
		OUTPUT(OW_PIN); // mo¿e mo¿na przestawiæ wy¿ej aby mia³o logike, ale to wymaga testów i czasu
		_delay_us(OW_TIMEUS_RESET);
		uint8_t sreg = SREG;
		cli();
		INPUT(OW_PIN);
		#if OW_INT_PULLUP
			HIGH(OW_PIN);
		#endif
		_delay_us(OW_TIMEUS_PRESENCE);
		PRESENCE = READ(OW_PIN);   // sygna³ obecnoœci, je¿eli 0 to ktoœ jest na linii (1 u mnie, u innych waga bitu-stan wysoki na pinie to brak urz¹dzeñ)
		SREG = sreg;
		_delay_us(OW_TIMEUS_RESET - OW_TIMEUS_PRESENCE);
		if(READ(OW_PIN) == 0 )
		{
			PRESENCE = 1;             // zwarcie na linii ? spodziewane 0 jest high
		}

		return PRESENCE;
}



uint8_t _OW_IO1BIT( uint8_t BIT, uint8_t PARASITE_ON)
{
		uint8_t sreg = SREG;
		cli();
		#if OW_INT_PULLUP
			LOW(OW_PIN);
		#endif
		OUTPUT(OW_PIN);    // drive bus low
		_delay_us(OW_TIMEUS_FALLING);    // T_INT > 1usec accoding to timing-diagramm
		if ( BIT & 1 )
		{
		INPUT(OW_PIN); // to write "1" release bus, resistor pulls high
		#if OW_INT_PULLUP
			HIGH(OW_PIN);
		#endif
		}

		// "Output data from the DS18B20 is valid for 15usec after the falling
		// edge that initiated the read time slot. Therefore, the master must
		// release the bus and then sample the bus state within 15ussec from
		// the start of the slot."
		_delay_us(OW_TIMEUS_QUARTER-OW_TIMEUS_FALLING);

		if( READ(OW_PIN) == 0 )
		{
			BIT = 0;  // sample at end of read-timeslot
		}

		_delay_us(OW_TIMEUS_SLOT_IO-OW_TIMEUS_QUARTER-OW_TIMEUS_FALLING);
		#if OW_INTL_PULLUP
			HIGH(OW_PIN);
		#endif
		INPUT(OW_PIN);

		if ( PARASITE_ON & 1) {
			HIGH(OW_PIN);
			OUTPUT(OW_PIN);
		}

	SREG=sreg; /* ATOMIC_BLOCK */

	_delay_us(OW_TIMEUS_RECOVERY); // may be increased for longer wires

	return BIT;
}



uint8_t OW_W8BIT( uint8_t OCTET, uint8_t PARASITE_ON)
{
	uint8_t i = 8, j;

	do {
		if ( i != 1 ) {
			j = _OW_IO1BIT( OCTET & 1, 0 );
		} else {
			j = _OW_IO1BIT( OCTET & 1, PARASITE_ON );
		}
		OCTET >>= 1;
		if( j ) {
			OCTET |= 0x80;
		}
	} while( --i );

	return OCTET;
}



uint8_t OW_R8BIT()  // odczyt OKTETU na magistralê
{
	// read by sending only "1"s, so bus gets released
	// after the init low-pulse in every slot
	 return OW_W8BIT( 0xFF, 0 );
}



// wybranie urz¹dzenia po adresie (tablica 8 bajtów ID) i wys³anie komendy  //static void onewire_write_command(uint8_t *id)
void OW_COMMAND(const uint8_t *ID, uint8_t COMMAND, uint8_t PARASITE_ON)
{

	uint8_t i;

	OW_RESETPULSE();

	if( ID )
	{
		OW_W8BIT( OW_MATCH_ROM, EXT_POWER );     // to a single device
		i = OW_ROMCODE_SIZE;
		do {
			OW_W8BIT( *ID, EXT_POWER );
			ID++;
		} while( --i );
	}
	else
	{
		OW_W8BIT( OW_SKIP_ROM, 0 );      // to all devices
	}
	OW_W8BIT( COMMAND, PARASITE_ON & 1 );
}



void OW_RESETSEARCH() //reset przesukiwania One Wire, mo¿e do usuniêcia //static void onewire_findinit()
{
	 _ow_LastDevice = 0;
	 _ow_LastDiscrepency = 0;
	 _ow_LastFamilyDiscrepency = 0;
	 _ow_last_buffer = 0;
}



//TODO: OW_SEARCH(uint8_t CMD, uint8_t *ID, uint8_t DEVIATIONPOS) szukanie alarmów i ROM
uint8_t OW_SEARCH(uint8_t *ID) // przeszukiwanie magistrali //static uint8_t _onewire_search_rom(uint8_t *buffer)
{


	 uint8_t id_bit_number = 1;
	 uint8_t last_zero = 0;
	 uint8_t rom_byte_number = 0;
	 uint8_t search_result = OW_ERR_NO_DEVICES;
	 uint8_t id_bit, cmp_id_bit;
	 uint8_t rom_byte_mask = 1;
	 uint8_t search_direction;
	#ifdef ONEWIRE_CRC
	 uint8_t crc8 = 0;
	#endif
	 if(!_ow_LastDevice)
	 {
	  if(OW_RESETPULSE())
	  {
	   OW_RESETSEARCH();
	   return OW_ERR_NO_DEVICES;
	  }
	  OW_W8BIT(OW_SEARCH_ROM, EXT_POWER);
	  do
	  {
	   id_bit = _OW_IO1BIT(1, 0);
	   cmp_id_bit = _OW_IO1BIT(1, 0);
	   if(id_bit && cmp_id_bit)
	    return OW_ERR_DATA;
	   else
	   {
	    if(id_bit | cmp_id_bit)
	     search_direction = id_bit;
	    else
		{
		 if(id_bit_number < _ow_LastDiscrepency)
		 {
		  if(_ow_last_buffer)
		   search_direction = (_ow_last_buffer[rom_byte_number] & rom_byte_mask);
	      else
		   search_direction = (ID[rom_byte_number] & rom_byte_mask);
	     }
	     else
		  search_direction = id_bit_number == _ow_LastDiscrepency;
	     if(search_direction == 0)
		 {
		  last_zero = id_bit_number;
		  if(last_zero < 9)
		   _ow_LastFamilyDiscrepency = last_zero;
	     }
	    }
		if(search_direction)
		 ID[rom_byte_number] |= rom_byte_mask;
	    else
		 ID[rom_byte_number] &= ~rom_byte_mask;
	    _OW_IO1BIT(search_direction, 0);
	    id_bit_number++;
		rom_byte_mask <<= 1;
		if(rom_byte_mask == 0)
		{
		 rom_byte_mask = 1;
	#ifdef ONEWIRE_CRC
	     _onewire_crc8(&crc8, buffer[rom_byte_number]);
	#endif
		 rom_byte_number++;
	    }
	   }
	  }
	  while(rom_byte_number < 8);
	  if(id_bit_number > 64)
	  {
	   _ow_LastDiscrepency = last_zero;
	   if(_ow_LastDiscrepency == 0)
	   {
	    _ow_LastDevice = 1;
	   }
	   search_result = OW_ERR_OK;
	  }
	  if(search_result != OW_ERR_OK || !ID[0])
	  {
	   OW_RESETSEARCH();
	   search_result = OW_ERR_DATA;
	  }
	#ifdef ONEWIRE_CRC
	  if(crc8 != 0)
	  {
	   onewire_findinit();
	   search_result = ONEWIRE_CRC_ERROR;
	  }
	#endif
	 }
	 _ow_last_buffer = ID;
	 return search_result;
}




// Perform a search. If this function returns a '1' then it has
// enumerated the next device and you may retrieve the ROM from the
// OneWire::address variable. If there are no devices, no further
// devices, or something horrible happens in the middle of the
// enumeration then a 0 is returned.  If a new device is found then
// its address is copied to newAddr.  Use OneWire::reset_search() to
// start over.
//
// --- Replaced by the one from the Dallas Semiconductor web site ---
//--------------------------------------------------------------------------
// Perform the 1-Wire Search Algorithm on the 1-Wire bus using the existing
// search state.
// Return TRUE  : device found, ROM number in ROM_NO buffer
//        FALSE : device not found, end of search
//
// Compute a Dallas Semiconductor 8 bit CRC directly.
// this is much slower, but much smaller, than the lookup table.
//
uint8_t OW_CRC8(const uint8_t *TAB, uint8_t LEN) // wyliczenie sumy CRC8 z tablicy ID przy d³ugoœci LEN
{
	uint8_t crc = 0;

	while (LEN--) {
		uint8_t inbyte = *TAB++;
		for (uint8_t i = 8; i; i--) {
			uint8_t mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}


/************************** KONIEC RESZTA PO KONTROLI DO WYWALENIA ***********************/
