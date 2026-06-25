/****************************************************************************
* Biblioteka obslugi wyswietlacza.
* 
* Wzor pliku naglowkowego funkcji specyficznych dla platformy.
*
* Nazwa pliku: TEMPLATE_platf_spec.h
* Data: 26.01.2008
* Wersja: 1.0
*
* Copyright (C) 2008 by Rafal Baranowski.
*
* Niniejsze oprogramowanie udostepniane jest nieodplatnie i moze byc
* powielane, modyfikowane i wykorzystywane w celach komercyjnych pod
* warunkiem zachowania noty copyright.
* Na poprawne dzialanie i bezpieczenstwo tego oprogramowania nie jest
* udzalana zadna gwarancja.
****************************************************************************/

#ifndef __TEMPLATE_PLATF_SPEC_H__
#define __TEMPLATE_PLATF_SPEC_H__ 1

//+++ Dodaj pliki naglowkowe specyficzne dla mikrokontrolera
#include <inttypes.h>
#include <disp_conf.h>

//+++ Zmien domyslna czestotliwosc pracy mikrokontrolera
#ifndef F_CPU
#  warning "Nie okreslono czestotliwosci pracy CPU (F_CPU). Przyjeto 12 MHz."
#  define F_CPU 12000000UL
#endif

/*
 * Deklaracja wartosci okreslajacych kierunek portow.
 */
#define IN 0
#define OUT 1

/*
 * Ustawiaja badz zeruja poszczegolne linie specjalne.
 */
//+++ Stworz definicje wszystkich ponizszych makr dla linii specjalnych
//+++ set_xx() - ustawia linie xx
//+++ rst_xx() - zeruje linie xx
//+++ get_xx() - zwraca stan lini xx (0 lub 1)
#define set_rs() 
#define rst_rs() 
#define get_rs() 
#define set_rw() 
#define rst_rw() 
#define get_rw() 
#define set_en1() 
#define rst_en1() 
#define set_en2() 
#define rst_en2() 
#define set_en3() 
#define rst_en3() 
#define set_en4() 
#define rst_en4() 
#define set_en5() 
#define rst_en5() 
#define set_en6() 
#define rst_en6() 
#define set_en7() 
#define rst_en7() 
#define set_en8() 
#define rst_en8() 
#define set_cd() 
#define rst_cd() 
#define get_cd() 
#define set_ce() 
#define rst_ce() 
#define set_rd() 
#define rst_rd() 
#define set_wr() 
#define rst_wr() 
#define set_res() 
#define rst_res() 
#define set_cs1() 
#define rst_cs1() 
#define set_cs2() 
#define rst_cs2() 
#define set_cs3() 
#define rst_cs3() 
#define set_cs4() 
#define rst_cs4() 
#define set_cs5() 
#define rst_cs5() 
#define set_cs6() 
#define rst_cs6() 
#define set_cs7() 
#define rst_cs7() 
#define set_cs8() 
#define rst_cs8() 
#define set_clk() 
#define rst_clk() 
#define set_mosi() 
#define rst_mosi() 
#define get_miso() 

/*
 * Wykonuje instrukcje no-operation (minimum 100 ns).
 */
#if F_CPU < 10000000UL
#  define nop() asm volatile("nop\n\t"::);
#elif F_CPU < 20000000UL
#  define nop() asm volatile("nop\n\t" \
                             "nop\n\t"::);
#elif F_CPU < 30000000UL
#  define nop() asm volatile("nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t"::);  
#elif F_CPU < 40000000UL
#  define nop() asm volatile("nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t"::); 
#elif F_CPU < 50000000UL
#  define nop() asm volatile("nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t"::);   
#elif F_CPU < 60000000UL
#  define nop() asm volatile("nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t"::); 
#elif F_CPU < 70000000UL
#  define nop() asm volatile("nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t"::);                                
#elif F_CPU < 80000000UL
#  define nop() asm volatile("nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t"::); 
#elif F_CPU < 90000000UL
#  define nop() asm volatile("nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t"::);
#else
#  define nop() asm volatile("nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t" \
                             "nop\n\t"::);
#  warning "Makro nop() nie funkcjonuje poprawnie przy tak wysokiej F_CPU."
#endif

/*
 * Blokuje wykonywanie przerwan.
 */
__inline__ void disable_interrupts(void);

/*
 * Odblokowuje wykonywanie przerwan, jesli przed wykonaniem funkcji
 * disable_interrupts() byly one odblokowane.
 */
__inline__ void revert_interrupts(void);

/*
 * Zawiesza wykonywanie programu na okreslony czas.
 * 
 * parametry:
 *   ms  - minimalny czas oczekiwania w milisekundach (max 255 ms)
 */
void delay_ms(uint8_t ms);

/*
 * Zawiesza wykonywanie programu na okreslony czas.
 * 
 * parametry:
 *   dus  - minimalny czas oczekiwania w dziesiątkach
 *          mikrosekund (max 10 * 255 us)
 */
void delay_dus(uint8_t dus);

/*
 * Zawiesza wykonywanie programu na okreslony czas.
 * 
 * parametry:
 *   us  - minimalny czas oczekiwania w mikrosekundach
 *         (max 100 us)
 */
void delay_us(uint8_t us);

/*
 * Zawiesza wykonywanie programu na min. 500ns.
 */
void delay_500ns(void);

/*
 * Zwalnia procesor na okreslony, minimalny czas.
 * (tylko dla implementacji z systemem operacyjnym)
 * 
 * parametry:
 *   ms - minimalny czas zwolnienia w milisekundach (max 255 ms)
 */
void release(uint8_t ms);

/*
 * Zwraca wartosc stalej spod wskazanego adresu.
 * 
 * parametry:
 *  pointer - wskaznik stalej
 */
uint8_t get_const(const uint8_t* pointer);

/*
 * Inicjalizuje porty mikrokontrolera.
 */
void init_ports(void);

/*
 * Zmienia kierunek dzialania magistrali.
 * 
 * parametry:
 *   dir  - kierunek dzialania portu (IN lub OUT)
 */
__inline__ void chg_dir_db(uint8_t dir);

/*
 * Wystawia dane na magistrale.
 * 
 * parametry:
 *   data  - dane do wystawienia
 */
__inline__ void write_db(uint8_t data);

/*
 * Odczytuje dane z magistrali.
 * 
 * zwraca:
 *   wartosc odczytana z magistrali
 */
__inline__ uint8_t read_db(void);

#endif // ! __TEMPLATE_PLATF_SPEC_H__

