/****************************************************************************
* Biblioteka obslugi wyswietlacza alfanumerycznego, opartego na sterowniku
* HD44780 lub odpowiedniku.
* 
* Plik konfiguracyjny platformy ARM9 STR91xx dla programu demonstracyjnego.
*
* Nazwa pliku: STR91_platf_conf.h
* Data: 19.01.2008
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
#define DISP_DB_WIDTH 8       // szerokosc magistrali (4 lub 8 bitow)
#define DISP_DB_MIX 0         // magistrala typu mixed (1) lub not-mixed (0)
#define DISP_USE_RW_LINE 0    // linia RW jest podlaczona (1) badz nie (0)

/*
 *  Ustawienia dla magistrali typu not-mixed (8-bitowej lub 4-bitowej)
 */ 
#define DISP_DB_GP      __GPIO8
#define DISP_DB_DR(x)   GPIO8->DR[(x)]
#define DISP_DB_DDR     GPIO8->DDR
#define DISP_DB_TYPE    SCU->GPIOTYPE[8]
#define DISP_DB_OUT     SCU->GPIOOUT[8]
#define DISP_DB_IN      SCU->GPIOIN[8]
#define DISP_DB_OFFSET  4      // przesuniecie magistrali
                               // (numer DB4 w trybie 4 bit) 

/*
 *  Ustawienia dla magistrali typu mixed (8-bitowej lub 4-bitowej)
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
#define DISP_RS_GP   __GPIO9           // linia Register Select
#define DISP_RS_DR   GPIO9->DR[1<<(DISP_RS_NO+2)]
#define DISP_RS_DDR  GPIO9->DDR
#define DISP_RS_TYPE SCU->GPIOTYPE[9]
#define DISP_RS_OUT  SCU->GPIOOUT[9]
#define DISP_RS_IN   SCU->GPIOIN[9]
#define DISP_RS_NO 2
#define DISP_RW_GP   __GPIO9           // linia Read/nWrite
#define DISP_RW_DR   GPIO9->DR[1<<(DISP_RW_NO+2)]
#define DISP_RW_DDR  GPIO9->DDR
#define DISP_RW_TYPE SCU->GPIOTYPE[9]
#define DISP_RW_OUT  SCU->GPIOOUT[9]
#define DISP_RW_IN   SCU->GPIOIN[9]
#define DISP_RW_NO 1
#define DISP_E1_GP   __GPIO9           // linia Enable1
#define DISP_E1_DR   GPIO9->DR[1<<(DISP_E1_NO+2)]
#define DISP_E1_DDR  GPIO9->DDR
#define DISP_E1_TYPE SCU->GPIOTYPE[9]
#define DISP_E1_OUT  SCU->GPIOOUT[9]
#define DISP_E1_IN   SCU->GPIOIN[9]
#define DISP_E1_NO 0
#define DISP_E2_GP   __GPIO9           // linia Enable2
#define DISP_E2_DR   GPIO9->DR[1<<(DISP_E2_NO+2)]
#define DISP_E2_DDR  GPIO9->DDR
#define DISP_E2_TYPE SCU->GPIOTYPE[9]
#define DISP_E2_OUT  SCU->GPIOOUT[9]
#define DISP_E2_IN   SCU->GPIOIN[9]
#define DISP_E2_NO 3

#endif // ! __STR91_PLATF_CONF_H__
