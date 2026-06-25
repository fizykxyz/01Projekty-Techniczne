/****************************************************************************
* Biblioteka obslugi wyswietlacza alfanumerycznego, opartego na sterowniku
* M66004 (VFD) lub odpowiedniku.
* 
* Plik konfiguracyjny dla programu demonstracyjnego.
*
* Nazwa pliku: disp_conf.h
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

#ifndef __DISP_CONF_H__
#define __DISP_CONF_H__ 1

#include <inttypes.h>

/*
 *  Parametry wyswietlacza
 */
#define DISP_OSC 500          // czestotliwosc taktowania wyswietlacza [kHz]
#define DISP_C_COLS_NUM 16    // liczba kolumn (znakow)
#define DISP_SCAN_TIME 1      // nastawa czasu przemiatania znaku
                              // (0) wolno, (1) szybko
#define DISP_DEF_CONTRAST 3   // domyslna wartosc jasnosci (0..7)
#define DISP_CUR_PRESENT 1    // kursor dostepny (1) badz nie (0)

/*
 * Parametry biblioteki
 */
#define DISP_BUFFER_LEN 10    // dlugosc bufora w bajtach
#define DISP_ON_BUFFER_FULL 1 // jesli bufor jest pelny:
                              // (0) pomin zapisywane wartosci (przepelnienie)
                              // (1) przeslij zawartosc bufora do wyswietlacza
                              //     przed zapisem nowych wartosci
                              // (2) oczekuj na zwolnienie bufora

#endif // ! __DISP_CONF_H__
