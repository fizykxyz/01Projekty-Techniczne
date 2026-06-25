/****************************************************************************
* Biblioteka obslugi wyswietlacza graficznego, opartego na sterowniku
* T6963C lub odpowiedniku.
* 
* Plik konfiguracyjny dla programu demonstracyjnego na platformie AVR.
*
* Nazwa pliku: disp_conf.h
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

#ifndef __DISP_CONF_H__
#define __DISP_CONF_H__

#include <inttypes.h>

/*
 *  Parametry wyswietlacza
 */
#define DISP_OSC 6000           // czestotliwosc taktowania wyswietlacza [kHz]
#define DISP_SCR_Y_NUM   1      // liczba ekranow (1 lub 2)
#define DISP_P_ROWS_NUM  128    // liczba wierszy (pikseli)
#define DISP_P_COLS_NUM  240    // liczba kolumn (pikseli)
#define DISP_CHAR_WIDTH  6      // szerokosc znakow (6 lub 8 pikseli)
#define DISP_GRAPH_HOME  0x0000 // adres poczatku obszaru graficznego
#define DISP_ATTR_HOME   0x1400 // adres poczatku obszaru atrybutow tekstu
#define DISP_TEXT_HOME   0x1700 // adres poczatku obszaru tekstowego
#define DISP_CGRAM_HOME  0x1C00 // adres poczatku obszaru generatora znakow
#define DISP_2SCR_OFFSET 0x8000 // offset adresu dla drugiego ekranu
     
#define DISP_C_ROWS_NUM (DISP_P_ROWS_NUM/8)
#define DISP_C_COLS_NUM (DISP_P_COLS_NUM/DISP_CHAR_WIDTH)
#define DISP_P_BYTES_NUM (DISP_P_COLS_NUM/DISP_CHAR_WIDTH)

/*
 * Parametry biblioteki
 */
#define DISP_BUFFER_LEN 50    // dlugosc bufora w 16-bit slowach
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
                              
/*
 * Parametry kompilacji
 */
#define DISP_NOC_PUT_PIC   0  // nie kompiluj (1) procedury Disp_put_picture()
#define DISP_NOC_PUT_CPIC  0  // nie kompiluj (1) procedury Disp_put_picture_const()
#define DISP_NOC_GET_PIC   0  // nie kompuluj (1) procedury Disp_get_picture()
#define DISP_NOC_LINE      0  // nie kompiluj (1) procedury Disp_line()
                              //   oraz Disp_draw_lineto()
#define DISP_NOC_CIRC      0  // nie kompuluj (1) procedury Disp_circle()

#endif // ! __DISP_CONF_H__
