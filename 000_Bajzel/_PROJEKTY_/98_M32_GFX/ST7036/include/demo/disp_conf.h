/****************************************************************************
* Biblioteka obslugi wyswietlacza alfanumerycznego, opartego na sterowniku
* ST7036 lub odpowiedniku.
* 
* Plik konfiguracyjny dla programu demonstracyjnego.
*
* Nazwa pliku: disp_conf.h
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

#ifndef __DISP_CONF_H__
#define __DISP_CONF_H__ 1

#include <inttypes.h>

/*
 *  Parametry wyswietlacza
 */
#define DISP_OSC 380          // czestotliwosc taktowania wyswietlacza [kHz]
#define DISP_C_ROWS_NUM 3     // liczba wierszy (znakow)
#define DISP_C_COLS_NUM 16    // liczba kolumn (znakow)
#define DISP_C_ROW0_ADDR 0x00 // adres wiersza nr 0
#define DISP_C_ROW1_ADDR 0x10 // adres wiersza nr 1
#define DISP_C_ROW2_ADDR 0x20 // adres wiersza nr 2
#define DISP_POW_SUPPLY 0     // napiecie zasilania
                              // (0) 2,7...4,5 V
                              // (1) 4,5...5,5 V
#define DISP_EXT_MODE 1       // tryb rozszerzony (1) lub normalny (0)
#define DISP_CONTR_COAR 6     // zgrubny kontrast (0...7), np:
                              // (4) dla 5 V
                              // (6) dla 3,3 V
#define DISP_CONTR_FINE 18    // dokladny kontrast (0...63), np:
                              // (12) dla 5 V
                              // (18) dla 3,3 V

/*
 * Parametry biblioteki
 */
#define DISP_BUFFER_LEN 10    // dlugosc bufora w 16-bit slowach
                              // (bufor danych i komend)
#define DISP_MAX_TIMEOUT 10   // maksymalne opoznienie po jakim wyswietlacz
                              // powinien byc gotowy [ms] (max 64)
#define DISP_ON_BUFFER_FULL 1 // jesli bufor jest pelny:
                              // (0) pomin zapisywane wartosci (przepelnienie)
                              // (1) przeslij zawartosc bufora do wyswietlacza
                              //     przed zapisem nowych wartosci
                              // (2) oczekuj na zwolnienie bufora
#define DISP_UPDATE_ON_READ 1 // aktualizuje zawartosc wyswietlacza
                              // przed odczytem (1) lub nie (0)

#endif // ! __DISP_CONF_H__
