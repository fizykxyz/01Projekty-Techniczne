/****************************************************************************
* Biblioteka obslugi wyswietlacza alfanumerycznego, opartego na sterowniku
* HD44780 lub odpowiedniku.
* 
* Plik konfiguracyjny dla programu demonstracyjnego.
*
* Nazwa pliku: disp_conf.h
* Data: 13.07.2007
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
#define DISP_OSC 270          // czestotliwosc taktowania wyswietlacza [kHz]
#define DISP_EN_NUM 1         // liczba linii Enable (1 lub 2)
#define DISP_C_ROWS_NUM 2     // liczba wierszy (znakow)
#define DISP_C_COLS_NUM 16    // liczba kolumn (znakow)
#define DISP_C_ROW0_ADDR 0x00 // adres wiersza nr 0
#define DISP_C_ROW1_ADDR 0x40 // adres wiersza nr 1
#define DISP_C_ROW2_ADDR 0x14 // adres wiersza nr 2
#define DISP_C_ROW3_ADDR 0x54 // adres wiersza nr 3
#define DISP_SPECIAL 0        // okresla rodzaj wyswietlacza
                              // (0) wyswietlacz standardowy
                              // (16) wyswietlacz 1x16 adresowany jak 2x8
                              // (20) wyswietlacz 1x20 adresowany jak 2x10
#define DISP_CHAR_LENGTH 8    // wysokosc znakow (8 lub 10 pikseli)     
#define DISP_DOUBLE_SPEED 0   // max czestotliwosc pracy magistrali
                              // (0) 1 MHz (Ecyc = 1000 ns)
                              // (1) 2 MHz (Ecyc = 500 ns)


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
