/****************************************************************************
* Biblioteka obslugi wyswietlacza graficznego, opartego na sterowniku
* PCD8544 lub odpowiedniku.
* 
* Plik konfiguracyjny platformy AVR dla programu demonstracyjnego.
*
* Nazwa pliku: AVR_platf_conf.h
* Data: 20.10.2007
* Wersja: 1.0
*
* Copyright (C) 2007 by Rafal Baranowski.
*
* Niniejsze oprogramowanie udostepniane jest nieodplatnie i moze byc
* powielane, modyfikowane i wykorzystywane w celach komercyjnych pod
* warunkiem zachowania noty copyright.
* Na poprawne dzialanie i bezpieczenstwo tego oprogramowania nie jest
* udzalana zadna gwarancja.
****************************************************************************/

#ifndef __AVR_PLATF_CONF_H__
#define __AVR_PLATF_CONF_H__ 1

#include <inttypes.h>

/* 
 * Parametry systemu
 */
#define DISP_OS_PRESENT 0     // system operacyjny obecny (1) lub nie (0)

/*
 *  Ustawienia magistrali
 */
#define DISP_DB_WIDTH 1       // szerokosc magistrali (1 bit)

/*
 *  Ustawienia dla magistrali 1 bit
 */
#define DISP_MOSI_PORT PORTB  // linia SerialDataINput
#define DISP_MOSI_DDR DDRB
#define DISP_MOSI_NO 0

/*
 *  Ustawienia linii specjalnych
 */
#define DISP_CLK_PORT PORTB   // linia SerialCLock
#define DISP_CLK_DDR DDRB
#define DISP_CLK_NO 1
#define DISP_CD_PORT PORTB    // linia Data/nCommand
#define DISP_CD_DDR DDRB
#define DISP_CD_NO 2
#define DISP_CE_PORT PORTB    // linia nSerialChipEnable
#define DISP_CE_DDR DDRB
#define DISP_CE_NO 3
#define DISP_RES_PORT PORTB   // linia nReset
#define DISP_RES_DDR DDRB
#define DISP_RES_NO 4

#endif // ! __AVR_PLATF_CONF_H__
