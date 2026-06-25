/****************************************************************************
* Biblioteka obslugi wyswietlacza graficznego, opartego na sterowniku
* PCD8544 lub odpowiedniku.
* 
* Plik konfiguracyjny platformy ARM9 STR91xx dla programu demonstracyjnego.
*
* Nazwa pliku: STR91_platf_conf.h
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

#ifndef __STR91_PLATF_CONF_H__
#define __STR91_PLATF_CONF_H__ 1

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
 #define DISP_MOSI_GP   __GPIO8           // linia SerialDataINput
#define DISP_MOSI_DR   GPIO8->DR[1<<(DISP_MOSI_NO+2)]
#define DISP_MOSI_DDR  GPIO8->DDR
#define DISP_MOSI_TYPE SCU->GPIOTYPE[8]
#define DISP_MOSI_OUT  SCU->GPIOOUT[8]
#define DISP_MOSI_IN   SCU->GPIOIN[8]
#define DISP_MOSI_NO 0

/*
 *  Ustawienia linii specjalnych
 */
#define DISP_CLK_GP   __GPIO8           // linia SerialCLock
#define DISP_CLK_DR   GPIO8->DR[1<<(DISP_CLK_NO+2)]
#define DISP_CLK_DDR  GPIO8->DDR
#define DISP_CLK_TYPE SCU->GPIOTYPE[8]
#define DISP_CLK_OUT  SCU->GPIOOUT[8]
#define DISP_CLK_IN   SCU->GPIOIN[8]
#define DISP_CLK_NO 1
#define DISP_CD_GP   __GPIO8           // linia Data/nCommand
#define DISP_CD_DR   GPIO8->DR[1<<(DISP_CD_NO+2)]
#define DISP_CD_DDR  GPIO8->DDR
#define DISP_CD_TYPE SCU->GPIOTYPE[8]
#define DISP_CD_OUT  SCU->GPIOOUT[8]
#define DISP_CD_IN   SCU->GPIOIN[8]
#define DISP_CD_NO 2
#define DISP_CE_GP   __GPIO8           // linia nSerialChipEnable
#define DISP_CE_DR   GPIO8->DR[1<<(DISP_CE_NO+2)]
#define DISP_CE_DDR  GPIO8->DDR
#define DISP_CE_TYPE SCU->GPIOTYPE[8]
#define DISP_CE_OUT  SCU->GPIOOUT[8]
#define DISP_CE_IN   SCU->GPIOIN[8]
#define DISP_CE_NO 3
#define DISP_RES_GP   __GPIO8           // linia nReset
#define DISP_RES_DR   GPIO8->DR[1<<(DISP_RES_NO+2)]
#define DISP_RES_DDR  GPIO8->DDR
#define DISP_RES_TYPE SCU->GPIOTYPE[8]
#define DISP_RES_OUT  SCU->GPIOOUT[8]
#define DISP_RES_IN   SCU->GPIOIN[8]
#define DISP_RES_NO 4

#endif // ! __STR91_PLATF_CONF_H__
