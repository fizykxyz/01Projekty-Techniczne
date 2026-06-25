/****************************************************************************
* Biblioteka obslugi wyswietlacza graficznego, opartego na sterowniku
* T6963C lub odpowiedniku.
* 
* Plik konfiguracyjny platformy AVR dla programu demonstracyjnego.
*
* Nazwa pliku: AVR_platf_conf.h
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

#ifndef __AVR_PLATF_CONF_H__
#define __AVR_PLATF_CONF_H__

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
#define DISP_USE_CE 1         // uzywaj linii Chip Enable (1) lub nie (0)

/*
 *  Ustawienia dla magistrali typu not-mixed
 */ 
#define DISP_DB_PORT PORTD
#define DISP_DB_PIN PIND
#define DISP_DB_DDR DDRD

/*
 *  Ustawienia dla magistrali typu mixed
 */
#define DISP_DB0_PORT PORTA   // linia DB 0
#define DISP_DB0_PIN PINA
#define DISP_DB0_DDR DDRA
#define DISP_DB0_NO 0
#define DISP_DB1_PORT PORTA   // linia DB 1
#define DISP_DB1_PIN PINA
#define DISP_DB1_DDR DDRA
#define DISP_DB1_NO 1  
#define DISP_DB2_PORT PORTA   // linia DB 2
#define DISP_DB2_PIN PINA
#define DISP_DB2_DDR DDRA
#define DISP_DB2_NO 2
#define DISP_DB3_PORT PORTA   // linia DB 3
#define DISP_DB3_PIN PINA
#define DISP_DB3_DDR DDRA
#define DISP_DB3_NO 3 
#define DISP_DB4_PORT PORTA   // linia DB 4
#define DISP_DB4_PIN PINA
#define DISP_DB4_DDR DDRA
#define DISP_DB4_NO 4 
#define DISP_DB5_PORT PORTA   // linia DB 5
#define DISP_DB5_PIN PINA
#define DISP_DB5_DDR DDRA
#define DISP_DB5_NO 5 
#define DISP_DB6_PORT PORTA   // linia DB 6
#define DISP_DB6_PIN PINA
#define DISP_DB6_DDR DDRA
#define DISP_DB6_NO 6 
#define DISP_DB7_PORT PORTA   // linia DB 7
#define DISP_DB7_PIN PINA
#define DISP_DB7_DDR DDRA
#define DISP_DB7_NO 7 

/*
 *  Ustawienia linii specjalnych
 */
#define DISP_CD_PORT PORTA    // linia Command/nData
#define DISP_CD_DDR DDRA
#define DISP_CD_NO 3
#define DISP_CE_PORT PORTA    // linia nChipEnable
#define DISP_CE_DDR DDRA
#define DISP_CE_NO 2
#define DISP_RD_PORT PORTA    // linia nRead
#define DISP_RD_DDR DDRA
#define DISP_RD_NO 1
#define DISP_WR_PORT PORTA    // linia nWrite
#define DISP_WR_DDR DDRA
#define DISP_WR_NO 0
#define DISP_RES_PORT PORTA   // linia nReset
#define DISP_RES_DDR DDRA
#define DISP_RES_NO 4

#endif // ! __AVR_PLATF_CONF_H__
