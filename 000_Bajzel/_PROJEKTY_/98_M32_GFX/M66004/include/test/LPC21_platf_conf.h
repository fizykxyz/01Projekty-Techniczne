/****************************************************************************
* Biblioteka obslugi wyswietlacza alfanumerycznego, opartego na sterowniku
* M66004 (VFD) lub odpowiedniku.
* 
* Plik konfiguracyjny platformy ARM7 LPC21xx dla programu demonstracyjnego.
*
* Nazwa pliku: LPC21_platf_conf.h
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

#ifndef __LPC21_PLATF_CONF_H__
#define __LPC21_PLATF_CONF_H__ 1

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
#define DISP_MOSI_SET IOSET1  // linia Data
#define DISP_MOSI_CLR IOCLR1
#define DISP_MOSI_PIN IOPIN1
#define DISP_MOSI_DDR IODIR1
#define DISP_MOSI_NO 16

/*
 *  Ustawienia linii specjalnych
 */
#define DISP_CS1_SET IOSET0    // linia nChipSelect
#define DISP_CS1_CLR IOCLR0
#define DISP_CS1_DDR IODIR0
#define DISP_CS1_NO 25
#define DISP_CLK_SET IOSET0    // linia Clock
#define DISP_CLK_CLR IOCLR0
#define DISP_CLK_DDR IODIR0
#define DISP_CLK_NO 23

#endif // ! __LPC21_PLATF_CONF_H__
