/*
 * ATMEGA32L_LCD onewire.h
 *
 *  Stworzono: 2014 13-10-2014
 *      Autor: D¹browski Tomasz
 *     Edycja: 06:55:09
 */

#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#include <stdbool.h>
#include <stdint.h>




//Onewire basic

#define OW_OK	0
#define OW_BusShorted	1
#define OW_NoPresencePulse	2
#define OW_SearchNoResponse 3

extern uint8_t Error;

void OW_INIT();
void OW_WRITEBIT(bool bit);
bool OW_READBIT();
void OW_RESETPULSE();
bool OW_WAITFORPRESENCEPULSE();

//onewire basic end
//ONEWIRE EXT

#define OW_MatchROM	0x55
#define OW_SearchROM 0xF0
#define OW_SkipROM 0xCC
#define OW_Alarm_Search 0xEC

void OW_WRITE(uint8_t);
uint8_t OW_READ();
void OW_SELECTDEVICE(const uint8_t *aID);
uint8_t OW_SEARCH(uint8_t cmd, uint8_t *aID, uint8_t deviationpos);
void OW_SKIP();

uint8_t OW_CRC8(const uint8_t *addr, uint8_t len);



////ONE WRE EXT END
//
//unsigned char ROM_NO[8];
//uint8_t LastDiscrepancy;
//uint8_t LastFamilyDiscrepancy;
//uint8_t LastDeviceFlag;
//
//void ONEWIRE_INIT(void);
//uint8_t ONEWIRE_RESET(void);
//void ONEWIRE_WRITEBIT(uint8_t v);
//uint8_t ONEWIRE_READBIT(void);
//void ONEWIRE_WRITE(uint8_t v, uint8_t power /* = 0 */);
//uint8_t ONEWIRE_READ();
//void ONEWIRE_SELECT(const uint8_t rom[8]);
//void ONEWIRE_SKIP();
//void ONEWIRE_RESETSEARCH();
//uint8_t ONEWIRE_SEARCH(uint8_t *newAddr);
//uint8_t ONEWIRE_CRC8(const uint8_t *addr, uint8_t len);
//

#endif /* ONEWIRE_H_ */
