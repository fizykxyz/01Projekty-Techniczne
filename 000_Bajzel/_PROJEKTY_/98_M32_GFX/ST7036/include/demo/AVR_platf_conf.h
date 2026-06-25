/****************************************************************************
* Biblioteka obslugi wyswietlacza alfanumerycznego, opartego na sterowniku
* ST7036 lub odpowiedniku.
* 
* Plik konfiguracyjny platformy AVR dla programu demonstracyjnego.
*
* Nazwa pliku: AVR_platf_conf.h
* Data: 16.10.2007
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
#define DISP_DB_WIDTH 8       // szerokosc magistrali (1, 4 lub 8 bitow)
#define DISP_DB_MIX 0         // magistrala typu mixed (1) lub not-mixed (0)
#define DISP_USE_RW_LINE 1    // linia RW jest podlaczona (1) badz nie (0)

/*
 *  Ustawienia dla interfejsu rownoleglego (4- lub 8-bitowego)
 */
#define DISP_RW_PORT PORTB    // linia Read/nWrite
#define DISP_RW_DDR DDRB
#define DISP_RW_NO 1
#define DISP_E1_PORT PORTB    // linia Enable
#define DISP_E1_DDR DDRB
#define DISP_E1_NO 2

/*
 *  Ustawienia dla magistrali typu not-mixed (8-bitowej lub 4-bitowej)
 */ 
#define DISP_DB_PORT PORTA
#define DISP_DB_PIN PINA
#define DISP_DB_DDR DDRA
#define DISP_DB_OFFSET 4      // przesuniecie magistrali w trybie 4-bit
                              // (numer portu DB4)

/*
 *  Ustawienia dla magistrali typu mixed (8-bitowej lub 4-bitowej)
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
 *  Ustawienia dla interfejsu szeregowego SPI
 */
#define DISP_MOSI_PORT PORTA   // linia SI
#define DISP_MOSI_PIN PINA
#define DISP_MOSI_DDR DDRA
#define DISP_MOSI_NO 7 
#define DISP_CLK_PORT PORTA    // linia SCL
#define DISP_CLK_PIN PINA
#define DISP_CLK_DDR DDRA
#define DISP_CLK_NO 6
#define DISP_CS1_PORT PORTB    // linia CSB
#define DISP_CS1_DDR DDRB
#define DISP_CS1_NO 3

/*
 *  Ustawienia linii specjalnych
 */
#define DISP_RS_PORT PORTB    // linia Register Select
#define DISP_RS_DDR DDRB
#define DISP_RS_NO 0

#endif // ! __AVR_PLATF_CONF_H__
