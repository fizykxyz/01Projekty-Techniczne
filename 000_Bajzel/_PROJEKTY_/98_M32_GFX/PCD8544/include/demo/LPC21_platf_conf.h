/****************************************************************************
* Biblioteka obslugi wyswietlacza graficznego, opartego na sterowniku
* PCD8544 lub odpowiedniku.
* 
* Plik konfiguracyjny platformy ARM7 LPC21xx dla programu demonstracyjnego.
*
* Nazwa pliku: LPC21_platf_conf.h
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
#define DISP_MOSI_SET IOSET1  // linia SerialDataINput
#define DISP_MOSI_CLR IOCLR1
#define DISP_MOSI_DDR IODIR1
#define DISP_MOSI_NO 16

/*
 *  Ustawienia linii specjalnych
 */
#define DISP_CLK_SET IOSET1   // linia SerialCLock
#define DISP_CLK_CLR IOCLR1
#define DISP_CLK_DDR IODIR1
#define DISP_CLK_NO 17
#define DISP_CD_SET IOSET1    // linia Data/nCommand
#define DISP_CD_CLR IOCLR1
#define DISP_CD_DDR IODIR1
#define DISP_CD_NO 18
#define DISP_CE_SET IOSET1    // linia nSerialChipEnable
#define DISP_CE_CLR IOCLR1
#define DISP_CE_DDR IODIR1
#define DISP_CE_NO 19
#define DISP_RES_SET IOSET1   // linia nReset
#define DISP_RES_CLR IOCLR1
#define DISP_RES_DDR IODIR1
#define DISP_RES_NO 20

#endif // ! __LPC21_PLATF_CONF_H__
