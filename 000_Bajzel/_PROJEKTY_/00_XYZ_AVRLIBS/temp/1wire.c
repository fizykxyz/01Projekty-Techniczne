#ifndef _AVR_ONEWIRE_
#define _AVR_ONEWIRE_

#include <avr/interrupt.h>
#include <util/delay.h>

#define ONEWIRE_PORT PINA
#define ONEWIRE_DDR DDRA
#define ONEWIRE_PIN PA2

#ifdef ONEWIRE_CRC
#include <avr/pgmspace.h>:
#endif

#define ONEWIRE_SEARCH_ROM 0xF0

#define ONEWIRE_NO_DEVICES 0xFF
#define ONEWIRE_DATA_ERROR 0xFE
#define ONEWIRE_CRC_ERROR 0xFD
#define ONEWIRE_OK 0x00

uint8_t _onewire_LastDevice;
uint8_t _onewire_LastDiscrepency;
uint8_t _onewire_LastFamilyDiscrepency;
uint8_t *_onewire_last_buffer;

static uint8_t onewire_reset();
static void _onewire_write_bit(uint8_t value);
static uint8_t _onewire_read_bit();
static void onewire_write_byte(uint8_t value);
static uint8_t onewire_read_byte();
static void onewire_write_command(uint8_t *id);
static void onewire_findinit();
static uint8_t _onewire_search_rom(uint8_t *buffer);
static inline uint8_t onewire_find(uint8_t *buffer);



static uint8_t onewire_reset()
{
 ONEWIRE_DDR |= (1<<ONEWIRE_PIN);
 _delay_us(480);
 ONEWIRE_DDR &= ~(1<<ONEWIRE_PIN);
 _delay_us(70);
 uint8_t ret = ~(ONEWIRE_PORT & (1<<ONEWIRE_PIN));
 _delay_us(410);
 return ret;
}

static void _onewire_write_bit(uint8_t value)
{
 uint8_t sreg = SREG;
 cli();
 ONEWIRE_DDR |= (1<<ONEWIRE_PIN);
 _delay_us(1);
 if(value)
 {
  ONEWIRE_DDR &= ~(1<<ONEWIRE_PIN);
  _delay_us(59);
 }
 else
 {
  _delay_us(59);
  ONEWIRE_DDR &= ~(1<<ONEWIRE_PIN);
 }
 SREG = sreg;
 _delay_us(30);
}

static uint8_t _onewire_read_bit()
{
 uint8_t sreg = SREG;
 cli();
 ONEWIRE_DDR |= (1<<ONEWIRE_PIN);
 _delay_us(1);
 ONEWIRE_DDR &= ~(1<<ONEWIRE_PIN);
 _delay_us(15);
 uint8_t ret = ONEWIRE_PORT & (1<<ONEWIRE_PIN);
 SREG = sreg;
 _delay_us(75);
 return ret;
}

static void onewire_write_byte(uint8_t value)
{
 for(uint8_t i = 0; i < 8; i++)
 {
  _onewire_write_bit(value & 0x01);
  value = value >> 1;
 }
}

static uint8_t onewire_read_byte()
{
 uint8_t ret = 0;
 for(uint8_t i = 0; i < 8; i++)
 {
  ret = ret >> 1;
  if(_onewire_read_bit()) ret |= 0x80;
 }
 return ret;
}

static void onewire_write_command(uint8_t *id)
{
 onewire_reset();
 if(id)
 {
  onewire_write_byte(0x55);
  for(uint8_t i = 0; i < 8; i++)
   onewire_write_byte(*id++);
 }
 else
 {
  onewire_write_byte(0xCC);
 }
}

#ifdef ONEWIRE_CRC

uint8_t PROGMEM _onewire_crc_table[] = {
        0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
      157,195, 33,127,252,162, 64, 30, 95,  1,227,189, 62, 96,130,220,
       35,125,159,193, 66, 28,254,160,225,191, 93,  3,128,222, 60, 98,
      190,224,  2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
       70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89,  7,
      219,133,103, 57,186,228,  6, 88, 25, 71,165,251,120, 38,196,154,
      101, 59,217,135,  4, 90,184,230,167,249, 27, 69,198,152,122, 36,
      248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91,  5,231,185,
      140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
       17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
      175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
       50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
      202,148,118, 40,171,245, 23, 73,  8, 86,180,234,105, 55,213,139,
       87,  9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
      233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
      116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53};

static void _onewire_crc8(uint8_t *crc, uint8_t value)
{
 *crc = pgm_read_byte(&_onewire_crc_table[*crc ^ value]);
}

#endif

static void onewire_findinit()
{
 _onewire_LastDevice = 0;
 _onewire_LastDiscrepency = 0;
 _onewire_LastFamilyDiscrepency = 0;
 _onewire_last_buffer = 0;
}

static uint8_t _onewire_search_rom(uint8_t *buffer)
{
 uint8_t id_bit_number = 1;
 uint8_t last_zero = 0;
 uint8_t rom_byte_number = 0;
 uint8_t search_result = ONEWIRE_NO_DEVICES;
 uint8_t id_bit, cmp_id_bit;
 uint8_t rom_byte_mask = 1;
 uint8_t search_direction;
#ifdef ONEWIRE_CRC
 uint8_t crc8 = 0;
#endif
 if(!_onewire_LastDevice)
 {
  if(!onewire_reset())
  {
   onewire_findinit();
   return ONEWIRE_NO_DEVICES;
  }
  onewire_write_byte(ONEWIRE_SEARCH_ROM);
  do
  {
   id_bit = _onewire_read_bit();
   cmp_id_bit = _onewire_read_bit();
   if(id_bit && cmp_id_bit)
    return ONEWIRE_DATA_ERROR;
   else
   {
    if(id_bit | cmp_id_bit)
     search_direction = id_bit;
    else
	{
	 if(id_bit_number < _onewire_LastDiscrepency)
	 {
	  if(_onewire_last_buffer)
	   search_direction = (_onewire_last_buffer[rom_byte_number] & rom_byte_mask);
      else
	   search_direction = (buffer[rom_byte_number] & rom_byte_mask);
     }
     else
	  search_direction = id_bit_number == _onewire_LastDiscrepency;
     if(search_direction == 0)
	 {
	  last_zero = id_bit_number;
	  if(last_zero < 9)
	   _onewire_LastFamilyDiscrepency = last_zero;
     }
    }
	if(search_direction)
	 buffer[rom_byte_number] |= rom_byte_mask;
    else
	 buffer[rom_byte_number] &= ~rom_byte_mask;
    _onewire_write_bit(search_direction);
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
   _onewire_LastDiscrepency = last_zero;
   if(_onewire_LastDiscrepency == 0)
   {
    _onewire_LastDevice = 1;
   }
   search_result = ONEWIRE_OK;
  }
  if(search_result != ONEWIRE_OK || !buffer[0])
  {
   onewire_findinit();
   search_result = ONEWIRE_DATA_ERROR;
  }
#ifdef ONEWIRE_CRC
  if(crc8 != 0)
  {
   onewire_findinit();
   search_result = ONEWIRE_CRC_ERROR;
  }
#endif
 }
 _onewire_last_buffer = buffer;
 return search_result;
}

static inline uint8_t onewire_find(uint8_t *buffer)
{
 return _onewire_search_rom(buffer);
}

#endif
