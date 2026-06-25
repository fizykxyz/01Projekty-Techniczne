/*
 * ATMEGA32L_LCD onewire.c
 *
 *  Stworzono: 2014 13-10-2014
 *      Autor: D¹browski Tomasz
 *     Edycja: 06:55:32
 */



#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include <util/atomic.h>

#include "onewire.h"


void ONEWIRE_INIT(void)
{

ATOMIC_BLOCK(ATOMIC_FORCEON)
		{

		}
	CLR(DDR,ONEWIRE_PIN);
	ONEWIRE_RESETSEARCH();
}


// Perform the onewire reset function.  We will wait up to 250uS for
// the bus to come high, if it doesn't then it is broken or shorted
// and we return a 0;
//
// Returns 1 if a device asserted a presence pulse, 0 otherwise.
//

uint8_t ONEWIRE_RESET(void)
{
	uint8_t r;
	uint8_t retries = 125;
	cli();
	CLR(DDR,ONEWIRE_PIN);
	sei();
	 //wait until the wire is high... just in case
	do {
		if (--retries == 0) return 0;
		_delay_us(2);
	} while (!GETONEWIRE);

	cli();
	CLR(PORT,ONEWIRE_PIN);
	SET(DDR,ONEWIRE_PIN);	// drive output low
	sei();
	_delay_us(480);
	cli();
	CLR(DDR,ONEWIRE_PIN);	// allow it to float
	_delay_us(70);
	r=(!GETONEWIRE);
	sei();
	_delay_us(410);
	cli();
	r=(!GETONEWIRE);
	sei();
	return r;
}

// Write a bit. Port and bit is used to cut lookup time and provide
// more certain timing.
//
void ONEWIRE_WRITEBIT(uint8_t v)
{
	if (v & 1) {
		cli();
		CLR(PORT,ONEWIRE_PIN);
		SET(DDR,ONEWIRE_PIN);	// drive output low
		_delay_us(10);
		SET(PORT,ONEWIRE_PIN);	// drive output high
		sei();
		_delay_us(55);
	} else {
		cli();
		CLR(PORT,ONEWIRE_PIN);
		SET(DDR,ONEWIRE_PIN);	// drive output low
		_delay_us(65);
		SET(PORT,ONEWIRE_PIN);	// drive output high
		sei();
		_delay_us(5);
	}
}

uint8_t ONEWIRE_READBIT(void)
{
	uint8_t r;
	cli();
	SET(DDR,ONEWIRE_PIN);
	CLR(PORT,ONEWIRE_PIN);
	_delay_us(3);
	CLR(DDR,ONEWIRE_PIN);	// let pin float, pull up will raise
	_delay_us(10);
	r = GETONEWIRE;
	sei();
	_delay_us(53);
	return r;
}

// Write a byte. The writing code uses the active drivers to raise the
// pin high, if you need power after the write (e.g. DS18S20 in
// parasite power mode) then set 'power' to 1, otherwise the pin will
// go tri-state at the end of the write to avoid heating in a short or
// other mishap.
//
void ONEWIRE_WRITE(uint8_t v, uint8_t power /* = 0 */)
{
    uint8_t bitMask;
    for (bitMask = 0x01; bitMask; bitMask <<= 1) {
	ONEWIRE_WRITEBIT( (bitMask & v)?1:0);
    }
    if ( !power) {
	cli();
	CLR(DDR,ONEWIRE_PIN);
	CLR(PORT,ONEWIRE_PIN);
	sei();
    }
}

// Read a byte
//
uint8_t ONEWIRE_READ()
{
    uint8_t bitMask;
    uint8_t r = 0;
    for (bitMask = 0x01; bitMask; bitMask <<= 1) {
	if ( ONEWIRE_READBIT()) r |= bitMask;
    }
    return r;
}

// Do a ROM select
//
void ONEWIRE_SELECT(const uint8_t rom[8])
{
    uint8_t i;

    ONEWIRE_WRITE(0x55,0);           // Choose ROM

    for (i = 0; i < 8; i++) ONEWIRE_WRITE(rom[i],0);
}

// Do a ROM skip
//
void ONEWIRE_SKIP()
{
    ONEWIRE_WRITE(0xCC,0);           // Skip ROM
}


void ONEWIRE_RESETSEARCH()
{
  // reset the search state
  LastDiscrepancy = 0;
  LastDeviceFlag = 0;
  LastFamilyDiscrepancy = 0;
  for(int i = 7; ; i--) {
    ROM_NO[i] = 0;
    if ( i == 0) break;
  }
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
uint8_t ONEWIRE_SEARCH(uint8_t *newAddr)
{
   uint8_t id_bit_number;
   uint8_t last_zero, rom_byte_number, search_result;
   uint8_t id_bit, cmp_id_bit;

   unsigned char rom_byte_mask, search_direction;

   // initialize for search
   id_bit_number = 1;
   last_zero = 0;
   rom_byte_number = 0;
   rom_byte_mask = 1;
   search_result = 0;

   // if the last call was not the last one
   if (!LastDeviceFlag)
   {
      // 1-Wire reset
      if (!ONEWIRE_RESET())
      {
         // reset the search
         LastDiscrepancy = 0;
         LastDeviceFlag = 0;
         LastFamilyDiscrepancy = 0;
         return 0;
      }

      // issue the search command
      ONEWIRE_WRITE(0xF0,0);

      // loop to do the search
      do
      {
         // read a bit and its complement
         id_bit = ONEWIRE_READBIT();
         cmp_id_bit = ONEWIRE_READBIT();

         // check for no devices on 1-wire
         if ((id_bit == 1) && (cmp_id_bit == 1))
            break;
         else
         {
            // all devices coupled have 0 or 1
            if (id_bit != cmp_id_bit)
               search_direction = id_bit;  // bit write value for search
            else
            {
               // if this discrepancy if before the Last Discrepancy
               // on a previous next then pick the same as last time
               if (id_bit_number < LastDiscrepancy)
                  search_direction = ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
               else
                  // if equal to last pick 1, if not then pick 0
                  search_direction = (id_bit_number == LastDiscrepancy);

               // if 0 was picked then record its position in LastZero
               if (search_direction == 0)
               {
                  last_zero = id_bit_number;

                  // check for Last discrepancy in family
                  if (last_zero < 9)
                     LastFamilyDiscrepancy = last_zero;
               }
            }

            // set or clear the bit in the ROM byte rom_byte_number
            // with mask rom_byte_mask
            if (search_direction == 1)
              ROM_NO[rom_byte_number] |= rom_byte_mask;
            else
              ROM_NO[rom_byte_number] &= ~rom_byte_mask;

            // serial number search direction write bit
            ONEWIRE_WRITEBIT(search_direction);

            // increment the byte counter id_bit_number
            // and shift the mask rom_byte_mask
            id_bit_number++;
            rom_byte_mask <<= 1;

            // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
            if (rom_byte_mask == 0)
            {
                rom_byte_number++;
                rom_byte_mask = 1;
            }
         }
      }
      while(rom_byte_number < 8);  // loop until through all ROM bytes 0-7

      // if the search was successful then
      if (!(id_bit_number < 65))
      {
         // search successful so set LastDiscrepancy,LastDeviceFlag,search_result
         LastDiscrepancy = last_zero;

         // check for last device
         if (LastDiscrepancy == 0)
            LastDeviceFlag = 1;

         search_result = 1;
      }
   }

   // if no device found then reset counters so next 'search' will be like a first
   if (!search_result || !ROM_NO[0])
   {
      LastDiscrepancy = 0;
      LastDeviceFlag = 0;
      LastFamilyDiscrepancy = 0;
      search_result = 0;
   }
   for (int i = 0; i < 8; i++) newAddr[i] = ROM_NO[i];
   return search_result;
  }

// Compute a Dallas Semiconductor 8 bit CRC directly.
// this is much slower, but much smaller, than the lookup table.
//
uint8_t ONEWIRE_CRC8(const uint8_t *addr, uint8_t len)
{
	uint8_t crc = 0;

	while (len--) {
		uint8_t inbyte = *addr++;
		for (uint8_t i = 8; i; i--) {
			uint8_t mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}

