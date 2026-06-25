/****************************************************************************
* Biblioteka obslugi wyswietlacza alfanumerycznego, opartego na sterowniku
* M66004 (VFD) lub odpowiedniku.
*
* Nazwa pliku: M66004.h
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

#ifndef __M66004_H__
#define __M66004_H__ 1

#include <inttypes.h>

/*
 * Deklaracje typow wyliczeniowych.
 */
enum {NO = 0, YES = 1};
enum {DEFERRED = 0, IMMEDIATELY = 1};
enum {OFF = 0, ON = 1};
enum {INC = 0, DEC = 1};
enum {LEFT = 0, RIGHT = 1};

/*
 * Deklaracje znakow specjalnych
 */
#define PL_CHARSET { \
     0x3E, 0x48, 0x88, 0x4A, 0x3E,  /*   0 */ \
     0x3C, 0x42, 0x62, 0xC2, 0x24,  /*   1 */ \
     0xFC, 0xA4, 0xA4, 0xA6, 0x86,  /*   2 */ \
     0xFE, 0x12, 0x22, 0x42, 0x02,  /*   3 */ \
     0x7E, 0x10, 0x48, 0x84, 0x7E,  /*   4 */ \
     0x3C, 0x42, 0x62, 0xC2, 0x3C,  /*   5 */ \
     0x24, 0x52, 0xD2, 0xD2, 0x2C,  /*   6 */ \
     0x46, 0x4A, 0xD2, 0x62, 0x42,  /*   7 */ \
     0x08, 0x54, 0x54, 0x56, 0x3E,  /*   8 */ \
     0x1C, 0x22, 0x62, 0xA2, 0x04,  /*   9 */ \
     0x38, 0x54, 0x54, 0x56, 0x32,  /*  10 */ \
     0x00, 0x92, 0xFE, 0x22, 0x00,  /*  11 */ \
     0x3E, 0x10, 0x60, 0xA0, 0x1E,  /*  12 */ \
     0x1C, 0x22, 0x62, 0xA2, 0x1C,  /*  13 */ \
     0x12, 0x2A, 0x6A, 0xAA, 0x04,  /*  14 */ \
     0x22, 0x26, 0xAA, 0x32, 0x22}  /*  15 */
#define PL_CHARSET_LENGTH 80
enum {PL_A=0x90, PL_C=0x91, PL_E=0x92, PL_L=0x93, PL_N=0x94, PL_O=0x95, PL_S=0x96, PL_Z=0x97};
enum {PL_a=0x98, PL_c=0x99, PL_e=0x9A, PL_l=0x9B, PL_n=0x9C, PL_o=0x9D, PL_s=0x9E, PL_z=0x9F};

/*
 * Inicjalizuje wyswietlacz.
 * 
 * zwraca:
 *    0 - jesli inicjalizacja przebiegla poprawnie
 *    1 - jesli wystapil blad
 */
uint8_t Disp_init(void);

/*
 * Sprawdza poprawnosc funkcjonowania wyswietlacza.
 * 
 * zwraca:
 *    0 - jesli wyswietlacz pracuje poprawnie
 *    1 - jesli wystapil blad (np. timeout)
 */
uint8_t Disp_get_error(void);

/*
 * Okresla atrybuty wyswietlacza.
 * 
 * parametry:
 *    on    - wlaczony (ON) badz wylaczony (OFF)
 */
void Disp_set(uint8_t on);

/*
 * Okresla kontrast wyswietlacza.
 * 
 * parametry:
 *    contrast - wartosc z przedzialu 0 do 7
 */
void Disp_set_contrast(uint8_t contrast);

/*
 * Okresla atrybuty kursora znakowego.
 * 
 * parametry:
 *    on    - wlaczony (ON) badz wylaczony (OFF)
 */
void Disp_set_cur(uint8_t on);

/*
 * Okresla tryb wprowadzania.
 * 
 * parametry:
 *    input_mode - autoinkrementacja adresu przy zapisie i odczycie (INC)
 *                 badz autodekrementacja (DEC)
 */
void Disp_set_input_mode(uint8_t input_mode);

/*
 * Przesyla do wyswietlacza dane z bufora komunikacyjnego.
 * 
 * W przypadku implementacji w systemie operacyjnym niniejsza
 * funkcja powinna byc wykonywana cyklicznie jako zadanie.
 * Moze byc ona takze wywolywana przez dowolne, cykliczne przerwanie.
 * 
 * zwraca:
 *    0 - jesli wyswietlacz pracuje poprawnie
 *    1 - jesli wystapil blad (np. timeout)
 */
uint8_t Disp_update(void);

/*
 * Zmienia tryb odswiezania wyswietlacza.
 * 
 * parametry:
 *    new_update - okresla, czy komendy i dane maja byc
 *        przesylane natychmiast (IMMEDIATELY) czy wylacznie w trakcie
 *        wykonywania procedury Disp_update (DEFERRED)
 */
void Disp_set_update(uint8_t new_update);

/*
 * Sprawdza tryb odswiezania wyswietlacza.
 * 
 * zwraca:
 *    IMMEDIATELY - jesli komendy i dane maja byc przesylane natychmiast
 *    DEFERRED    - jesli komendy i dane przesylane wylacznie w trakcie
 *                  wykonywania procedury Disp_update()
 */
uint8_t Disp_get_update(void);

/*
 * Zeruje bufor komunikacyjny.
 */
void Disp_flush_buffer(void);

/*
 * Sprawdza czy zaszlo przepelnienie bufora.
 * 
 * zwraca:
 *    0 - jesli bufor funkcjonuje poprawnie
 *    1 - jesli zaszlo przepelnienie bufora 
 */
uint8_t Disp_get_ovf(void);

/*
 * Zmienia pozycje kursora znakowego.
 * 
 * parametry:
 *    cx - numer kolumny znakow
 *    cy - numer linii znakow
 */
void Disp_set_cxy(uint8_t cx, uint8_t cy);

/*
 * Odczytuje aktualna pozycje kursora znakowego.
 * 
 * parametry:
 *    cx - wskaznik docelowej zmiennej numeru kolumny znakow
 *    cy - wskaznik docelowej zmiennej numeru linii znakow
 */
void Disp_get_cxy(uint8_t* cx, uint8_t* cy);

/*
 * Odczytuje aktualna pozycje kursora znakowego w osi x.
 * 
 * zwraca:
 *    numer kolumny znakow
 */
uint8_t Disp_get_cx(void);

/*
 * Odczytuje aktualna pozycje kursora znakowego w osi y.
 * 
 * zwraca:
 *    numer linii znakow
 */
uint8_t Disp_get_cy(void);

/*
 * Przesuwa kursor znakowy o jedna pozycje.
 * 
 * parametry:
 *    dir - w prawo (RIGHT) lub w lewo (LEFT)
 */
void Disp_move_txt_cur(uint8_t dir);

/*
 * Czysci pole tekstowe wyswietlacza.
 */
void Disp_clr_txt(void);

/*
 * Czysci zawartość wyswietlacza.
 */
void Disp_clr(void);

/*
 * Przywraca kursor do pozycji poczatkowej.
 */
void Disp_ret_cur(void);

/*
 * Wyswietla znak na aktualnej pozycji kursora.
 * 
 * parametry:
 *    character - kod znaku do wyswietlenia
 */
void Disp_putc(char character);

/*
 * Wyswietla lancuch znakow poczawszy od aktualnej pozycji kursora.
 * 
 * parametry:
 *    string - wskaznik poczatku lancucha znakow
 */
void Disp_puts(char* string);

/*
 * Wyswietla staly lancuch znakow poczawszy od aktualnej pozycji kursora.
 * 
 * parametry:
 *    string - wskaznik poczatku lancucha znakow
 */
void Disp_puts_const(const char* string);

/*
 * Wybiera tablice szablonow znakowych.
 * 
 * parametry:
 *    templates - wskaznik poczatku tablicy szablonow
 *    count     - dlugosc tablicy szablonow w bajtach
 *    addr      - numer pierwszego szablonu do zapisania
 */
void Disp_set_ctempl(uint8_t* templates, uint8_t count, uint8_t addr);

/*
 * Wybiera stala tablice szablonow znakowych.
 * 
 * parametry:
 *    templates - wskaznik poczatku tablicy szablonow
 *    count     - dlugosc tablicy szablonow w bajtach
 *    addr      - numer pierwszego szablonu do zapisania
 */
void Disp_set_ctempl_const(const uint8_t* templates, uint8_t count, uint8_t addr);

#endif // ! __M66004_H__
