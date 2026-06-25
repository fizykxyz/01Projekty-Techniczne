/*
 * ATMEGA32L_LCD onewire.h
 *
 *  Stworzono: 2014-10-13
 *      Autor: D¹browski Tomasz
 *     Edycja: 06:55:09
 */

#ifndef ONEWIRE_H_
#define ONEWIRE_H_


//pliki za³¹czone
#include <avr/io.h>
//#include <stdint.h>

#include "../ADDS/pins.h"
#include "onewire_cfg.h"
#include "onewire_fam.h"

//TODO: rozbudowa o obs³ugê: wykrywanie b³êdów magistrali i szuk. alarmów, tu przenieœæ definicje rodzin?


#define EXT_POWER 0 // zasilanie zewnetrzne
#define PARASITE_MODE 1 //tryb paso¿ytniczy

// czasy obs³ugi one wire w us (mikrosekundach)
#define OW_TIMEUS_RESET 	480  //ref.480 reset linii
#define OW_TIMEUS_PRESENCE 	64	 //ref.64 oczekiwanie na slave's
#define OW_TIMEUS_SLOT_IO 	60	 //ref.60 sloty czasowe operacji odczytu/zapisu bitów
#define OW_TIMEUS_FALLING 	2	 //ref.1 zbocze opadaj¹ce master'a
#define OW_TIMEUS_QUARTER 	15	 //ref.15 przedzia³ miêdy zapisem 1 a 0
// Recovery time (T_Rec) minimum 1usec - increase for long lines
// 5 usecs is a value give in some Maxim AppNotes
// 30u secs seem to be reliable for longer lines
//#define OW_RECOVERY_TIME        5  /* usec */
//#define OW_RECOVERY_TIME      300 /* usec */
#define OW_TIMEUS_RECOVERY	10 /* usec */


//definicje kodów b³êdów

#define OW_ERR_OK 					0x00
#define OW_ERR_SHORTED				0x01
#define OW_ERR_NO_PRESENCE_PULSE	0x02
#define OW_ERR_SEARCH_NO_RESPONSE 	0x03
#define OW_ERR_SEARCH_FIRST			0xFB        // start new search
#define OW_ERR_LAST_DEVICE  		0xFC        // last device found
#define OW_ERR_CR					0xFD
#define OW_ERR_DATA					0xFE
#define OW_ERR_NO_DEVICES			0xFF

extern uint8_t OW_ERROR; //kod b³êdu na zewn¹trz

//adresy rozkazów One Wire

#define OW_MATCH_ROM    0x55
#define OW_SKIP_ROM     0xCC
#define OW_SEARCH_ROM   0xF0
#define OW_ALARM_SEARCH 0xEC

// rom-code size including CRC
#define OW_ROMCODE_SIZE 8



// reset magistrali + czy jest odpowiedŸ z szyny (zanegowana) ?
uint8_t OW_RESETPULSE();


uint8_t _OW_IO1BIT( uint8_t BIT, uint8_t PARASITE_ON); //zapis BIT'u na magistralê, odczyt poprzez wstawienie BIT=1
// zapis BIT'u na magistralê, odczyt przy OCTET=0xFF + power dla PARASITE mode
uint8_t OW_W8BIT( uint8_t OCTET, uint8_t PARASITE_ON);
// odczyt OKTETU na magistralê
uint8_t OW_R8BIT();

// wybranie urz¹dzenia po adresie (tablica 8 bajtów ID), brak ID pominiêcie innych urz¹dzeñ (przy jednym czujniku)
void OW_COMMAND(const uint8_t *ID, uint8_t COMMAND, uint8_t PARASITE_ON);

void OW_RESETSEARCH(); //reset przesukiwania One Wire, mo¿e do usuniêcia
// przeszukiwanie magistrali
uint8_t OW_SEARCH(uint8_t *ID);
// wyliczenie sumy CRC8 z tablicy TAB przy d³ugoœci LEN
uint8_t OW_CRC8(const uint8_t *TAB, uint8_t LEN);


#endif /* ONEWIRE_H_ */
