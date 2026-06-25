/*
 * ATMEGA32L_LCD DS18B20.h
 *
 *  Stworzono: 2014 18-09-2014
 *      Autor: D¹browski Tomasz
 *     Edycja: 21:54:32
 */

#ifndef DS18B20_H_
#define DS18B20_H_

//pliki za³¹czone
#include "../ADDS/tools.h"

#define WIRE1_PIN A, 1

#define DS_TMPINT(TMPBITS) (TMPBITS>>4)
#define DS_TMPFRAC(TMPBITS) ((TMPBITS & 0B1111)*625)
#define NIBBLE2HEX_(NIBBLE) (NIBBLE>9) ? (NIBBLE+55) : (NIBBLE+48);
#define NIBBLE2HEX(NIBBLE) NIBBLE2HEX_(NIBBLE)


#define OW_MATCH_ROM    0x55
#define OW_SKIP_ROM     0xCC
#define OW_SEARCH_ROM   0xF0

#define OW_SEARCH_FIRST 0xFF        // start new search
#define OW_PRESENCE_ERR 0xFF
#define OW_DATA_ERR     0xFE
#define OW_LAST_DEVICE  0x00        // last device found

// rom-code size including CRC
#define OW_ROMCODE_SIZE 8

#define OW_RECOVERY_TIME         10 /* usec */
#define OW_CONF_DELAYOFFSET 0




#define DS18B20_CONVERTTEMP 0x44
#define DS18B20_RSCRATCHPAD 0xBE
#define DS18B20_WSCRATCHPAD 0x4E
#define DS18B20_CPYSCRATCHPAD 0x48
#define DS18B20_RECEEPROM 0xB8
#define DS18B20_RPWRSUPPLY 0xB4
#define DS18B20_SEARCHROM 0xF0
#define DS18B20_READROM 0x33
#define DS18B20_MATCHROM 0x55
#define DS18B20_SKIPROM 0xCC
#define DS18B20_ALARMSEARCH 0xEC

uint8_t ow_byte_wr( uint8_t b );
uint8_t ow_byte_rd( void );


uint8_t WIRE1_RESET(void);
uint8_t WIRE1_READBIT(void);
void WIRE1_WRITEBIT(uint8_t BIT);
uint8_t WIRE1_READ(void);
void WIRE1_WRITE(uint8_t OCTET);
uint8_t WIRE1_USEDEVICE(uint8_t USEIT);
void WIRE1_GETID(char ID[],uint8_t STR);
int16_t DS18B20_TMPBITS(void);
uint8_t WIRE1_ROM_SEARCH( uint8_t diff, uint8_t id[]);
#endif /* DS18B20_H_ */
