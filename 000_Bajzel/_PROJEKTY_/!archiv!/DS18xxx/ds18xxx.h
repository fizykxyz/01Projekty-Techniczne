/*
 * ATMEGA32L_LCD DS18XXX.h
 *
 *  Stworzono: 2014-09-18
 *      Autor: D¹browski Tomasz
 *     Edycja: 21:54:32
 */

#ifndef DS18xxx_H_
#define DS18xxx_H_

//pliki za³¹czone
#include "../ADDS/tools.h"
#include "../ONEWIRE/onewire.h"
//8421 15-8=7

#define DS_TMPMINUS(TMPBITS) (((TMPBITS) & 0x8000)>>15)
#define DS_TMPINT(TMPBITS) (((DS_TMPMINUS(TMPBITS)*(~(TMPBITS)+DS_TMPMINUS(TMPBITS))) +  (!DS_TMPMINUS(TMPBITS)*(TMPBITS))) >> 4)
#define DS_TMPFRAC(TMPBITS) (((TMPBITS) & 0b1111)*625)

// DS18XXX OneWire commands
#define DS18XXX_CONVERT_T    	0x44  // Tells device to take a temperature reading and put it on the scratchpad
#define DS18XXX_EE_SAVE     	0x48  // Copy EEPROM
#define DS18XXX_READ     		0xBE  // Read EEPROM
#define DS18XXX_WRITE    		0x4E  // Write to EEPROM
#define DS18XXX_EE_LOAD   		0xB8  // Reload from last known
#define DS18XXX_POWER_SUPPLY 	0xB4  // Determine if device needs parasite power
#define DS18XXX_ALARM_SEARCH    0xEC  // Query bus for devices with an alarm condition

#define DS18XXX_COPYSP_DELAY      10 /* ms */
// constant to convert the fraction bits to cel*(10^-4)
#define DS18XXX_FRACCONV          625

#define DS18XXX_DECIMAL_CHAR      '.'

// scratchpad size in bytes
#define DS18XXX_SP_SIZE           9

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


// Error Codes
#define DEVICE_DISCONNECTED -127

//#define DS18XXX_OK                0x00
//#define DS18XXX_ERROR             0x01
//#define DS18XXX_START_FAIL        0x02
//#define DS18XXX_ERROR_CRC         0x03

// Device resolution
#define TEMP_9_BIT  0x1F //  9 bit
#define TEMP_10_BIT 0x3F // 10 bit
#define TEMP_11_BIT 0x5F // 11 bit
#define TEMP_12_BIT 0x7F // 12 bit

#define DS18XXX_9_BIT             0
#define DS18XXX_10_BIT            (1<<5)
#define DS18XXX_11_BIT            (1<<6)
#define DS18XXX_12_BIT            ((1<<6)|(1<<5))
#define DS18XXX_RES_MASK          ((1<<6)|(1<<5))

// undefined bits in LSB if 18B20 != 12bit
#define DS18XXX_9_BIT_UNDF        ((1<<0)|(1<<1)|(1<<2))
#define DS18XXX_10_BIT_UNDF       ((1<<0)|(1<<1))
#define DS18XXX_11_BIT_UNDF       ((1<<0))
#define DS18XXX_12_BIT_UNDF       0

// conversion times in milliseconds
#define DS18XXX_TIMEMS_12BIT       750
#define DS18XXX_TCONV_11BIT       DS18XXX_TCONV_12_BIT/2
#define DS18XXX_TCONV_10BIT       DS18XXX_TCONV_12_BIT/4
#define DS18XXX_TCONV_9BIT        DS18XXX_TCONV_12_BIT/8
#define DS18XXX_TCONV             DS18XXX_TCONV_12_BIT
#define DS18xxx_TIMEMS_CONV		  750




uint8_t DS18xxx_PARASITE(uint8_t *ID);
uint8_t DS18xxx_STARTMEAS(uint8_t *ID, uint8_t PARASITE);
uint8_t DS18xxx_READSCRATCH(uint8_t *ID, uint8_t *SCRATCH, uint8_t LEN);
uint16_t DS18xxx_READ_RAWTEMP(uint8_t *ID, uint8_t PARASITE);
uint8_t DS18xxxSIGN(uint16_t RAW);
uint8_t DS18xxxINT(uint16_t RAW);
uint16_t DS18xxxFRAC(uint16_t RAW);

#endif /* DS18xxx_H_ */
