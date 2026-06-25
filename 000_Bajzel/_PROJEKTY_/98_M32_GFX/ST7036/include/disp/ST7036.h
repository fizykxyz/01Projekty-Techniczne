/****************************************************************************
* Biblioteka obslugi wyswietlacza alfanumerycznego, opartego na sterowniku
* ST7036 lub odpowiedniku.
*
* Nazwa pliku: ST7036.h
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

#ifndef __ST7036_H__
#define __ST7036_H__ 1

#include <inttypes.h>
#include <disp_conf.h>

/*
 * Deklaracje typow wyliczeniowych.
 */
enum {NO = 0, YES = 1};
enum {DEFERRED = 0, IMMEDIATELY = 1};
enum {OFF = 0, ON = 1};
enum {AND = 0, OR = 1, XOR = 2};
enum {NO_BLINK = 0, BLINK = 1};
enum {NORMAL = 0, INVERSED = 1};
enum {INC = 0, DEC = 1};
enum {LEFT = 0, RIGHT = 1};

/*
 * Deklaracje znakow specjalnych
 */
#define PL_CHARSET { \
     0x00, 0x00, 0x0E, 0x01, 0x0F, 0x11, 0x0F, 0x03,  /* 0 = a */ \
     0x02, 0x04, 0x0E, 0x10, 0x10, 0x11, 0x0E, 0x00,  /* 1 = c */ \
     0x00, 0x00, 0x0E, 0x11, 0x1F, 0x10, 0x0E, 0x03,  /* 2 = e */ \
     0x0C, 0x04, 0x06, 0x0C, 0x04, 0x04, 0x0E, 0x00,  /* 3 = l */ \
     0x02, 0x04, 0x16, 0x19, 0x11, 0x11, 0x11, 0x00,  /* 4 = n */ \
     0x02, 0x04, 0x0E, 0x11, 0x11, 0x11, 0x0E, 0x00,  /* 5 = o */ \
     0x02, 0x04, 0x0E, 0x10, 0x0E, 0x01, 0x1E, 0x00,  /* 6 = s */ \
     0x04, 0x00, 0x1F, 0x02, 0x04, 0x08, 0x1F, 0x00}  /* 7 = z */
#define PL_CHARSET_LENGTH 64
enum {PL_A=0, PL_C=1, PL_E=2, PL_L=3, PL_N=4, PL_O=5, PL_S=6, PL_Z=7};

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
 *    coarse - kontrast zgrubny (0...7)
 *    fine   - kontrast dokladny (0...63)
 */
void Disp_set_contrast(uint8_t coarse, uint8_t fine);

/*
 * Okresla wielkosc znakow (font).
 * 
 * parametry:
 *    font - wysokosc w pikselach (8 lub 16)
 *    position - nastawa dla linii gornych (0) lub dolnych (1)
 */
void Disp_set_font(uint8_t font, uint8_t position);

/*
 * Okresla atrybuty kursora znakowego.
 * 
 * parametry:
 *    on    - wlaczony (ON) badz wylaczony (OFF)
 *    blink - migajacy (BLINK) badz niemigajacy (NO_BLINK)
 */
void Disp_set_cur(uint8_t on, uint8_t blink);

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
 * Przesuwa okno o jedna pozycje.
 * 
 * parametry:
 *    dir - w prawo (RIGHT) lub w lewo (LEFT)
 */
void Disp_move_txt_window(uint8_t dir);

/*
 * Czysci pole tekstowe wyswietlacza.
 */
void Disp_clr_txt(void);

/*
 * Czysci zawartość wyswietlacza.
 */
void Disp_clr(void);

/*
 * Przywraca kursor i okno do pozycji poczatkowej.
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
 * Pobiera znak z aktualnej pozycji kursora.
 * 
 * zwraca:
 *    kod znaku na aktualnej pozycji kursora
 */
char Disp_getc(void);

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
 * Pobiera lancuch znakow poczawszy od aktualnej pozycji kursora.
 * 
 * parametry:
 *    string - wskaznik poczatku docelowego lancucha znakow
 *    count  - liczba znakow do odczytania
 */
void Disp_gets(char* string, uint8_t count);

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

/*
 * Odczytuje tablice szablonow znakowych z wyswietlacza.
 * 
 * parametry:
 *    templates - wskaznik poczatku docelowej tablicy szablonow
 *    count     - dlugosc tablicy szablonow w bajtach
 *    addr      - numer pierwszego szablonu do odczytania
 */
void Disp_get_ctempl(uint8_t* templates, uint8_t count, uint8_t addr);

#endif // ! __ST7036_H__
