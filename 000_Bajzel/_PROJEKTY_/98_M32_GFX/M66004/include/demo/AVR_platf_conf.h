/****************************************************************************
* Biblioteka obslugi wyswietlacza alfanumerycznego, opartego na sterowniku
* M66004 (VFD) lub odpowiedniku.
* 
* Plik konfiguracyjny platformy AVR dla programu demonstracyjnego.
*
* Nazwa pliku: AVR_platf_conf.h
* Data: 21.08.2007
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
#define DISP_MOSI_PORT PORTA  // linia Data
#define DISP_MOSI_PIN PINA
#define DISP_MOSI_DDR DDRA
#define DISP_MOSI_NO 0

/*
 *  Ustawienia linii specjalnych
 */
#define DISP_CS1_PORT PORTB   // linia nChipSelect
#define DISP_CS1_DDR DDRB
#define DISP_CS1_NO 0
#define DISP_CLK_PORT PORTB   // linia Clock
#define DISP_CLK_DDR DDRB
#define DISP_CLK_NO 1

#endif // ! __AVR_PLATF_CONF_H__
