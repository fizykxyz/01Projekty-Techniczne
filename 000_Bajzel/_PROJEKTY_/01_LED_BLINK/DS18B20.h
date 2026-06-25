#ifndef _DS18B20_H_
#define _DS18B20_H_

#include <stdint.h>

#define OW_CONVERT 0x44
#define OW_READ_SCRATCHPAD 0xBE
#define OW_WRITE_SCRATCHPAD 0x4E
#define OW_COPY_SCRATCHPAD 0x48
#define OW_RECALL_E2 0xB8
#define OW_READ_POWER_SUPPLY 0xB4


#define OW_FAMILY_DS1820 0x10
#define OW_FAMILY_DS18B20 0x28
#define OW_FAMILY_DS1822 0x22
#define OW_FAMILY_DS2502E48 0x89
#define OW_FAMILY_DS2502 0x09

//#define OW_DS2401 0x01 //iButton=1990A 'silicon serial number
//#define OW_DS2411 0x01 //iButton= 'silicon serial number Vcc input
//#define OW_DS1425 0x02 //iButton=1991 'multikey 1153bit secure
//#define OW_DS2404 0x04 //iButton=1994 'econoram time chip
//#define OW_DS2405 0x05 //iButton= 'Addresable Switch
//#define OW_DS1993 0x06 //iButton=1993 '4k memory ibutton
//#define OW_DS1992 0x08 //iButton=1992 '1k memory ibutton
//#define OW_DS2502 0x09 //iButton=1982 '1k add-only memory
//#define OW_DS1995 0x0A //iButton=1995 '16k memory ibutton
//#define OW_DS2505 0x0B //iButton=1985 '16k add-only memory
//#define OW_DS1996 0x0C //iButton=1996 '64k memory ibutton
//#define OW_DS2506 0x0F //iButton=1986 '64k add-only  memory
//#define OW_DS18S20 0x10 //iButton=1920 'high precision digital thermometer
//#define OW_DS2406 0x12 //iButton= 'dual addressable switch plus 1k memory
//#define OW_DS2407 0x12 //iButton= 'dual addressable switch plus 1k memory
//#define OW_DS2430A 0x14 //iButton=1971 '256 eeprom
//#define OW_DS1963L 0x1A //iButton=1963L '4k Monetary
//#define OW_DS2436 0x1B //iButton= 'battery id/monitor chip
//#define OW_DS28E04 0x1C //iButton= '4k EEPROM with PIO
//#define OW_DS2423 0x1D //iButton= '4k ram with counter
//#define OW_DS2409 0x1F //iButton= 'microlan coupler
//#define OW_DS2450 0x20 //iButton= 'quad a/d converter
//#define OW_DS1921 0x21 //iButton=1921 'Thermachron
//#define OW_DS1822 0x22 //iButton= 'Econo Digital Thermometer
//#define OW_DS2433 0x23 //iButton= '4k eeprom
//#define OW_DS2415 0x24 //iButton= 'time chip
//#define OW_DS2438 0x26 //iButton= 'smart battery monitor
//#define OW_DS2417 0x27 //iButton= 'time chip with interrupt
//#define OW_DS18B20 0x28 //iButton= 'programmable resolution digital thermometer
//#define OW_DS2408 0x29 //iButton= '8-channel addressable switch
//#define OW_DS2890 0x2C //iButton= 'digital potentiometer
//#define OW_DS2431 0x2D //iButton= '1k eeprom
//#define OW_DS2770 0x2E //iButton= 'battery monitor and charge controller
//#define OW_DS2760 0x30 //iButton= 'high-precision li+ battery monitor
//#define OW_DS2761 0x30 //iButton= 'high precision li+ battery monitor
//#define OW_DS2762 0x30 //iButton= 'high precision li+ battery monitor with alerts
//#define OW_DS2720 0x31 //iButton= 'efficient addressable single-cell rechargable lithium protection ic
//#define OW_DS2432 0x33 //iButton=1961S '1k protected eeprom with SHA-1
//#define OW_DS2740 0x36 //iButton= 'high precision coulomb counter
//#define OW_DS 0x37 //iButton=1977 'Password protected 32k eeprom
//#define OW_DS2413 0x3A //iButton= 'dual channel addressable switch
//#define OW_DS2422 0x41 //iButton=1922L 1922T 1923 'Temperature Logger 8k mem
//#define OW_DS2751 0x51 //iButton= 'multichemistry battery fuel gauge
//#define OW_DS1420  0x81 //iButton=1420 'Serial ID Button
//#define OW_DS2404S 0x84 //iButton= 'dual port plus time
//#define OW_DS2502 0x89 //iButton=1982U '48 bit node address chip
//#define OW_DS2505-UNW 0x8B //iButton=1985U '16k add-only uniqueware
//#define OW_DS2506-UNW 0x8F //iButton=1986U '64k add-only uniqueware
//#define OW_DSLCD 0xFF //iButton= 'LCD (Swart)
//#define OW_DS28E01-100 0xXX //iButton= '1k EEPROM with SHA-1
//#define OW_DS1821 0x00 //iButton= 'programmable digital thermostat and thermometer
//#define OW_DS2223 0x00 //iButton= 'econoram
//#define OW_DS2224 0x00 //iButton= 'econoram
//#define OW_DS 0x00 //iButton=1962S '1k protected eeprom SHA-1
//



void OW_Start_Conversion(uint8_t block);
int16_t OW_GetTemperature(uint8_t *aID);

#endif
