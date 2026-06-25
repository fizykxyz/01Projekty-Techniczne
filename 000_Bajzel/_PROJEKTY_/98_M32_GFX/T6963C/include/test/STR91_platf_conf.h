/****************************************************************************
* Biblioteka obslugi wyswietlacza graficznego, opartego na sterowniku
* T6963C lub odpowiedniku.
* 
* Plik konfiguracyjny platformy ARM9 STR91xx dla programu demonstracyjnego.
*
* Nazwa pliku: STR91_platf_conf.h
* Data: 13.08.2007
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
#define DISP_DB_WIDTH 8       // szerokosc magistrali (8 bitow)
#define DISP_DB_MIX 0         // magistrala typu mixed (1) lub not-mixed (0)

/*
 *  Ustawienia dla magistrali typu not-mixed
 */ 
#define DISP_DB_GP      __GPIO8
#define DISP_DB_DR(x)   GPIO8->DR[(x)]
#define DISP_DB_DDR     GPIO8->DDR
#define DISP_DB_TYPE    SCU->GPIOTYPE[8]
#define DISP_DB_OUT     SCU->GPIOOUT[8]
#define DISP_DB_IN      SCU->GPIOIN[8]

/*
 *  Ustawienia dla magistrali typu mixed
 */
#define DISP_DB0_GP   __GPIO8           // linia DB 0
#define DISP_DB0_DR   GPIO8->DR[1<<(DISP_DB0_NO+2)]
#define DISP_DB0_DDR  GPIO8->DDR
#define DISP_DB0_TYPE SCU->GPIOTYPE[8]
#define DISP_DB0_OUT  SCU->GPIOOUT[8]
#define DISP_DB0_IN   SCU->GPIOIN[8]
#define DISP_DB0_NO 0
#define DISP_DB1_GP   __GPIO8           // linia DB 1
#define DISP_DB1_DR   GPIO8->DR[1<<(DISP_DB1_NO+2)]
#define DISP_DB1_DDR  GPIO8->DDR
#define DISP_DB1_TYPE SCU->GPIOTYPE[8]
#define DISP_DB1_OUT  SCU->GPIOOUT[8]
#define DISP_DB1_IN   SCU->GPIOIN[8]
#define DISP_DB1_NO 1
#define DISP_DB2_GP   __GPIO8           // linia DB 2
#define DISP_DB2_DR   GPIO8->DR[1<<(DISP_DB2_NO+2)]
#define DISP_DB2_DDR  GPIO8->DDR
#define DISP_DB2_TYPE SCU->GPIOTYPE[8]
#define DISP_DB2_OUT  SCU->GPIOOUT[8]
#define DISP_DB2_IN   SCU->GPIOIN[8]
#define DISP_DB2_NO 2
#define DISP_DB3_GP   __GPIO8           // linia DB 3
#define DISP_DB3_DR   GPIO8->DR[1<<(DISP_DB3_NO+2)]
#define DISP_DB3_DDR  GPIO8->DDR
#define DISP_DB3_TYPE SCU->GPIOTYPE[8]
#define DISP_DB3_OUT  SCU->GPIOOUT[8]
#define DISP_DB3_IN   SCU->GPIOIN[8]
#define DISP_DB3_NO 3
#define DISP_DB4_GP   __GPIO8           // linia DB 4
#define DISP_DB4_DR   GPIO8->DR[1<<(DISP_DB4_NO+2)]
#define DISP_DB4_DDR  GPIO8->DDR
#define DISP_DB4_TYPE SCU->GPIOTYPE[8]
#define DISP_DB4_OUT  SCU->GPIOOUT[8]
#define DISP_DB4_IN   SCU->GPIOIN[8]
#define DISP_DB4_NO 4
#define DISP_DB5_GP   __GPIO8           // linia DB 5
#define DISP_DB5_DR   GPIO8->DR[1<<(DISP_DB5_NO+2)]
#define DISP_DB5_DDR  GPIO8->DDR
#define DISP_DB5_TYPE SCU->GPIOTYPE[8]
#define DISP_DB5_OUT  SCU->GPIOOUT[8]
#define DISP_DB5_IN   SCU->GPIOIN[8]
#define DISP_DB5_NO 5
#define DISP_DB6_GP   __GPIO8           // linia DB 6
#define DISP_DB6_DR   GPIO8->DR[1<<(DISP_DB6_NO+2)]
#define DISP_DB6_DDR  GPIO8->DDR
#define DISP_DB6_TYPE SCU->GPIOTYPE[8]
#define DISP_DB6_OUT  SCU->GPIOOUT[8]
#define DISP_DB6_IN   SCU->GPIOIN[8]
#define DISP_DB6_NO 6
#define DISP_DB7_GP   __GPIO8           // linia DB 7
#define DISP_DB7_DR   GPIO8->DR[1<<(DISP_DB7_NO+2)]
#define DISP_DB7_DDR  GPIO8->DDR
#define DISP_DB7_TYPE SCU->GPIOTYPE[8]
#define DISP_DB7_OUT  SCU->GPIOOUT[8]
#define DISP_DB7_IN   SCU->GPIOIN[8]
#define DISP_DB7_NO 7

/*
 *  Ustawienia linii specjalnych
 */
#define DISP_CD_GP   __GPIO9           // linia Data/nCommand
#define DISP_CD_DR   GPIO9->DR[1<<(DISP_CD_NO+2)]
#define DISP_CD_DDR  GPIO9->DDR
#define DISP_CD_TYPE SCU->GPIOTYPE[9]
#define DISP_CD_OUT  SCU->GPIOOUT[9]
#define DISP_CD_IN   SCU->GPIOIN[9]
#define DISP_CD_NO 0
#define DISP_CE_GP   __GPIO9           // linia nChipEnable
#define DISP_CE_DR   GPIO9->DR[1<<(DISP_CE_NO+2)]
#define DISP_CE_DDR  GPIO9->DDR
#define DISP_CE_TYPE SCU->GPIOTYPE[9]
#define DISP_CE_OUT  SCU->GPIOOUT[9]
#define DISP_CE_IN   SCU->GPIOIN[9]
#define DISP_CE_NO 1
#define DISP_RD_GP   __GPIO9           // linia nRead
#define DISP_RD_DR   GPIO9->DR[1<<(DISP_RD_NO+2)]
#define DISP_RD_DDR  GPIO9->DDR
#define DISP_RD_TYPE SCU->GPIOTYPE[9]
#define DISP_RD_OUT  SCU->GPIOOUT[9]
#define DISP_RD_IN   SCU->GPIOIN[9]
#define DISP_RD_NO 2
#define DISP_WR_GP   __GPIO9           // linia nWrite
#define DISP_WR_DR   GPIO9->DR[1<<(DISP_WR_NO+2)]
#define DISP_WR_DDR  GPIO9->DDR
#define DISP_WR_TYPE SCU->GPIOTYPE[9]
#define DISP_WR_OUT  SCU->GPIOOUT[9]
#define DISP_WR_IN   SCU->GPIOIN[9]
#define DISP_WR_NO 3
#define DISP_RES_GP   __GPIO9          // linia nReset
#define DISP_RES_DR   GPIO9->DR[1<<(DISP_RES_NO+2)]
#define DISP_RES_DDR  GPIO9->DDR
#define DISP_RES_TYPE SCU->GPIOTYPE[9]
#define DISP_RES_OUT  SCU->GPIOOUT[9]
#define DISP_RES_IN   SCU->GPIOIN[9]
#define DISP_RES_NO 4


#endif // ! __STR91_PLATF_CONF_H__
