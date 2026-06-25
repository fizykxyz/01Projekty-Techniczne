/****************************************************************************
* Biblioteka obslugi wyswietlacza graficznego, opartego na sterowniku
* T6963C lub odpowiedniku.
*
* Nazwa pliku: T6963C.h
* Data: 11.08.2007
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

#ifndef __T6963C_H__
#define __T6963C_H__

#include <inttypes.h>

/*
 * Deklaracje typow wyliczeniowych.
 */
enum {NO = 0, YES = 1};
enum {DEFERRED = 0, IMMEDIATELY = 1};
enum {OFF = 0, ON = 1};
enum {AND = 0, OR = 1, XOR = 2, TEXT_ONLY = 3};
enum {NO_BLINK = 0, BLINK = 1};
enum {NORMAL = 0, INVERSED = 1};
enum {INC = 0, DEC = 1};
enum {LEFT = 0, RIGHT = 1};
enum {UP = 0, DOWN = 1};
enum {ROMRAM = 0, RAM = 1};

/*
 * Deklaracje znakow specjalnych
 */
#define BLANK_CHR 0
#define PL_CHARSET { \
     0x04, 0x0A, 0x11, 0x11, 0x1F, 0x11, 0x13, 0x04,  /* 0  = A */ \
     0x02, 0x0E, 0x15, 0x10, 0x10, 0x11, 0x0E, 0x00,  /* 1  = C */ \
     0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F, 0x03,  /* 2  = E */ \
     0x10, 0x12, 0x14, 0x18, 0x10, 0x10, 0x1F, 0x00,  /* 3  = L */ \
     0x02, 0x15, 0x11, 0x19, 0x15, 0x13, 0x11, 0x00,  /* 4  = N */ \
     0x02, 0x0E, 0x15, 0x11, 0x11, 0x11, 0x0E, 0x00,  /* 5  = O */ \
     0x06, 0x0E, 0x11, 0x0E, 0x01, 0x11, 0x0E, 0x00,  /* 6  = S */ \
     0x04, 0x1F, 0x02, 0x04, 0x08, 0x10, 0x1F, 0x00,  /* 7  = Z */ \
     0x00, 0x00, 0x0E, 0x01, 0x0F, 0x11, 0x0F, 0x03,  /* 8  = a */ \
     0x02, 0x04, 0x0E, 0x10, 0x10, 0x11, 0x0E, 0x00,  /* 9  = c */ \
     0x00, 0x00, 0x0E, 0x11, 0x1F, 0x10, 0x0E, 0x03,  /* 10 = e */ \
     0x0C, 0x04, 0x06, 0x0C, 0x04, 0x04, 0x0E, 0x00,  /* 11 = l */ \
     0x02, 0x04, 0x16, 0x19, 0x11, 0x11, 0x11, 0x00,  /* 12 = n */ \
     0x02, 0x04, 0x0E, 0x11, 0x11, 0x11, 0x0E, 0x00,  /* 13 = o */ \
     0x02, 0x04, 0x0E, 0x10, 0x0E, 0x01, 0x1E, 0x00,  /* 14 = s */ \
     0x04, 0x00, 0x1F, 0x02, 0x04, 0x08, 0x1F, 0x00}  /* 15 = z */
#define PL_CHARSET_LENGTH 128
enum {PL_A=160, PL_C=161, PL_E=162, PL_L=163, PL_N=164, PL_O=165, PL_S=166, PL_Z=167};
enum {PL_a=168, PL_c=169, PL_e=170, PL_l=171, PL_n=172, PL_o=173, PL_s=174, PL_z=175};


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
 * Okresla atrybuty kursora znakowego.
 * 
 * parametry:
 *    on    - wlaczony (ON) badz wylaczony (OFF)
 *            lub wysokosc kursora (1..8)
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
 * Okresla tryb pracy generatora znakow.
 * 
 * parametry:
 *    templ_mem - pamiec generatora znakow:
 *                ROMRAM - ROM+RAM
 *                RAM - tylko RAM
 *    disp_mode - tryb wyswietlania znakow:
 *                OR - funkcja "or" tekstu i grafiki
 *                AND - funkcja "and" tekstu i grafiki
 *                XOR - funkcja "xor" tekstu i grafiki
 *                TEXT_ONLY - wyswietlanie grafiki wylaczone,
 *                  przestrzen pamieci grafiki przyporzadkowana
 *                  przestrzeni atrybutow tekstu
 */
void Disp_set_cg_mode(uint8_t templ_mem, uint8_t disp_mode);

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
 * Przesuwa okno w gore lub w dol.
 * 
 * parametry:
 *    dir - w gore (UP) lub w dol (DOWN)
 */ 
void Disp_scroll_txt_window(uint8_t dir);

/*
 * Czysci pole tekstowe wyswietlacza.
 */
void Disp_clr_txt(void);

/*
 * Czysci pole graficzne wyswietlacza.
 */
void Disp_clr_graph(void);

/*
 * Czysci zawartość wyswietlacza.
 */
__inline__ void Disp_clr(void);

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
void Disp_set_ctempl(uint8_t* templates, uint16_t count, uint8_t addr);

/*
 * Wybiera stala tablice szablonow znakowych.
 * 
 * parametry:
 *    templates - wskaznik poczatku tablicy szablonow
 *    count     - dlugosc tablicy szablonow w bajtach
 *    addr      - numer pierwszego szablonu do zapisania
 */
void Disp_set_ctempl_const(const uint8_t* templates, uint16_t count, uint8_t addr);

/*
 * Odczytuje tablice szablonow znakowych z wyswietlacza.
 * 
 * parametry:
 *    templates - wskaznik poczatku docelowej tablicy szablonow
 *    count     - dlugosc tablicy szablonow w bajtach
 *    addr      - adres poczatku pamieci szablonow wyswietlacza
 */
void Disp_get_ctempl(uint8_t* templates, uint16_t count, uint8_t addr);

/*
 * Zmienia pozycje kursora punktowego.
 * 
 * parametry:
 *    px - numer kolumny pikseli
 *    py - numer linii pikseli
 */
void Disp_set_pxy(uint8_t px, uint8_t py);

/*
 * Odczytuje aktualna pozycje kursora punktowego.
 * 
 * parametry:
 *    px - wskaznik docelowej zmiennej numeru kolumny pikseli
 *    py - wskaznik docelowej zmiennej numeru linii pikseli
 */
void Disp_get_pxy(uint8_t* px, uint8_t* py);

/*
 * Odczytuje aktualna pozycje kursora punktowego w osi x.
 * 
 * zwraca:
 *    numer kolumny pikseli
 */
uint8_t Disp_get_px(void);

/*
 * Odczytuje aktualna pozycje kursora punktowego w osi y.
 * 
 * zwraca:
 *    numer linii pikseli
 */
uint8_t Disp_get_py(void);

/*
 * Aktywuje badz deaktywuje piksel na aktualnej pozycji kursora.
 * 
 * parametry:
 *    pixel - piksel aktywny (ON) badz nieaktywny (OFF)
 */
void Disp_putp(uint8_t pixel);

/*
 * Pobiera piksel z aktualnej pozycji kursora.
 * 
 * zwraca:
 *    ON  - piksel aktywny
 *    OFF - piksel nieaktywny
 */
uint8_t Disp_getp(void);

/*
 * Rysuje linie pozioma do wyznaczonego punktu.
 * 
 * parametry:
 *    px        - wspolrzedna "x" drugiego punktu
 *    color     - linia aktywna (ON) badz nieaktywna (OFF)
 */
void Disp_hlineto(uint8_t px, uint8_t color);

/*
 * Rysuje linie pionowa do wyznaczonego punktu.
 * 
 * parametry:
 *    py        - wspolrzedna "y" drugiego punktu
 *    color     - linia aktywna (ON) badz nieaktywna (OFF)
 */
void Disp_vlineto(uint8_t py, uint8_t color);
 
/*
 * Rysuje linie z aktualnej pozycji kursora do wyznaczonego punktu.
 * 
 * parametry:
 *    px, py    - wspolrzedne punktu koncowego
 *    color     - linia aktywna (ON) badz nieaktywna (OFF)
 */
void Disp_lineto(uint8_t px, uint8_t py, \
                      uint8_t color);

/*
 * Rysuje linie pomiedzy dwoma wyznaczonymi punktami.
 * 
 * parametry:
 *    px1, py1  - wspolrzedne pierwszego punktu
 *    px2, py2  - wspolrzedne drugiego punktu
 *    color     - linia aktywna (ON) badz nieaktywna (OFF)
 */
__inline__ void Disp_line(uint8_t px1, uint8_t py1, \
                    uint8_t px2, uint8_t py2, \
                    uint8_t color);

/*
 * Rysuje prostokat pomiedzy dwoma wyznaczonymi punktami.
 * 
 * parametry:
 *    px1, py1  - wspolrzedne pierwszego rogu
 *    px2, py2  - wspolrzedne drugiego rogu
 *    color     - prostokat aktywny (ON) badz nieaktywny (OFF)
 *    filled    - prostokat zaczerniony (YES) lub pusty (NO)
 */
void Disp_box(uint8_t px1, uint8_t py1, \
                   uint8_t px2, uint8_t py2, \
                   uint8_t color, uint8_t filled);
                   
/*
 * Rysuje okrag o srodku w aktualnej pozycji kursora.
 * 
 * parametry:
 *    rad   - promien okregu
 *    color - okrag aktywny (ON) badz nieaktywny (OFF)
 */
void Disp_circle(uint8_t rad, uint8_t color);
                   
/*
 * Wyswietla obraz poczawszy od aktualnej pozycji kursora.
 * 
 * parametry:
 *    picture  - wskaznik obrazu
 *    width    - szerokosc obrazu w pikselach (os x) 
 *    length   - dlugosc obrazu w pikselach (os y)
 */
void Disp_put_picture(uint8_t* picture, \
                       uint8_t width, uint8_t length);
                       
/*
 * Wyswietla staly obraz poczawszy od aktualnej pozycji kursora.
 * 
 * parametry:
 *    picture  - wskaznik obrazu
 *    width    - szerokosc obrazu w pikselach (os x) 
 *    length   - dlugosc obrazu w pikselach (os y)
 */
void Disp_put_picture_const(const uint8_t* picture, \
                       uint8_t width, uint8_t length);
                       
/*
 * Pobiera obraz poczawszy od aktualnej pozycji kursora.
 * 
 * parametry:
 *    picture  - wskaznik docelowej zmiennej obrazu
 *    width    - szerokosc obrazu w pikselach (os x) 
 *    length   - dlugosc obrazu w pikselach (os y)
 * 
 * zwraca:
 *    liczbe pobranych bajtow
 */
uint16_t Disp_get_picture(uint8_t* picture, \
                      uint8_t width, uint8_t length);
                       
#endif // ! __T6963C_H__
