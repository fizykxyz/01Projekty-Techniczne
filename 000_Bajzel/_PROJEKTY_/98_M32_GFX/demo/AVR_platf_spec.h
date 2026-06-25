/****************************************************************************
* Biblioteka obslugi wyswietlacza.
* 
* Plik naglowkowy funkcji specyficznych dla platformy AVR.
*
* Nazwa pliku: AVR_platf_spec.h
* Data: 11.07.2007
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

#ifndef __AVR_PLATF_SPEC_H__
#define __AVR_PLATF_SPEC_H__

#include <inttypes.h>
#include "disp_conf.h"
#include <avr/io.h>

#ifndef F_CPU
#  warning "Nie okreslono czestotliwosci pracy CPU (F_CPU). Przyjeto 1 MHz."
#  define F_CPU 1000000UL
#endif

/*
 * Deklaracja wartosci okreslajacych kierunek portow.
 */
#define IN 0
#define OUT 1

/*
 * Ustawiaja badz zeruja poszczegolne linie specjalne.
 */
#define set_rs() DISP_RS_PORT |=  (1<<DISP_RS_NO)
#define rst_rs() DISP_RS_PORT &= ~(1<<DISP_RS_NO)
#define get_rs() (!!(DISP_RS_PORT & (1<<DISP_RS_NO)))
#define set_rw() DISP_RW_PORT |=  (1<<DISP_RW_NO)
#define rst_rw() DISP_RW_PORT &= ~(1<<DISP_RW_NO)
#define get_rw() (!!(DISP_RW_PORT & (1<<DISP_RW_NO)))
#define set_en1() DISP_E1_PORT |=  (1<<DISP_E1_NO)
#define rst_en1() DISP_E1_PORT &= ~(1<<DISP_E1_NO)
#define set_en2() DISP_E2_PORT |=  (1<<DISP_E2_NO)
#define rst_en2() DISP_E2_PORT &= ~(1<<DISP_E2_NO)
#define set_en3() DISP_E3_PORT |=  (1<<DISP_E3_NO)
#define rst_en3() DISP_E3_PORT &= ~(1<<DISP_E3_NO)
#define set_en4() DISP_E4_PORT |=  (1<<DISP_E4_NO)
#define rst_en4() DISP_E4_PORT &= ~(1<<DISP_E4_NO)
#define set_en5() DISP_E5_PORT |=  (1<<DISP_E5_NO)
#define rst_en5() DISP_E5_PORT &= ~(1<<DISP_E5_NO)
#define set_en6() DISP_E6_PORT |=  (1<<DISP_E6_NO)
#define rst_en6() DISP_E6_PORT &= ~(1<<DISP_E6_NO
#define set_en7() DISP_E7_PORT |=  (1<<DISP_E7_NO)
#define rst_en7() DISP_E7_PORT &= ~(1<<DISP_E7_NO)
#define set_en8() DISP_E8_PORT |=  (1<<DISP_E8_NO)
#define rst_en8() DISP_E8_PORT &= ~(1<<DISP_E8_NO)
#define set_cd() DISP_CD_PORT |=  (1<<DISP_CD_NO)
#define rst_cd() DISP_CD_PORT &= ~(1<<DISP_CD_NO)
#define get_cd() (!!(DISP_CD_PORT & (1<<DISP_CD_NO)))
#define set_ce() DISP_CE_PORT |=  (1<<DISP_CE_NO)
#define rst_ce() DISP_CE_PORT &= ~(1<<DISP_CE_NO)
#define set_rd() DISP_RD_PORT |=  (1<<DISP_RD_NO)
#define rst_rd() DISP_RD_PORT &= ~(1<<DISP_RD_NO)
#define set_wr() DISP_WR_PORT |=  (1<<DISP_WR_NO)
#define rst_wr() DISP_WR_PORT &= ~(1<<DISP_WR_NO)
#define set_res() DISP_RES_PORT |=  (1<<DISP_RES_NO)
#define rst_res() DISP_RES_PORT &= ~(1<<DISP_RES_NO)
#define set_cs1() DISP_CS1_PORT |=  (1<<DISP_CS1_NO)
#define rst_cs1() DISP_CS1_PORT &= ~(1<<DISP_CS1_NO)
#define set_cs2() DISP_CS2_PORT |=  (1<<DISP_CS2_NO)
#define rst_cs2() DISP_CS2_PORT &= ~(1<<DISP_CS2_NO)
#define set_cs3() DISP_CS3_PORT |=  (1<<DISP_CS3_NO)
#define rst_cs3() DISP_CS3_PORT &= ~(1<<DISP_CS3_NO)
#define set_cs4() DISP_CS4_PORT |=  (1<<DISP_CS4_NO)
#define rst_cs4() DISP_CS4_PORT &= ~(1<<DISP_CS4_NO)
#define set_cs5() DISP_CS5_PORT |=  (1<<DISP_CS5_NO)
#define rst_cs5() DISP_CS5_PORT &= ~(1<<DISP_CS5_NO)
#define set_cs6() DISP_CS6_PORT |=  (1<<DISP_CS6_NO)
#define rst_cs6() DISP_CS6_PORT &= ~(1<<DISP_CS6_NO)
#define set_cs7() DISP_CS7_PORT |=  (1<<DISP_CS7_NO)
#define rst_cs7() DISP_CS7_PORT &= ~(1<<DISP_CS7_NO)
#define set_cs8() DISP_CS8_PORT |=  (1<<DISP_CS8_NO)
#define rst_cs8() DISP_CS8_PORT &= ~(1<<DISP_CS8_NO)
#define set_clk() DISP_CLK_PORT |=  (1<<DISP_CLK_NO)
#define rst_clk() DISP_CLK_PORT &= ~(1<<DISP_CLK_NO)
#define set_mosi() DISP_MOSI_PORT |=  (1<<DISP_MOSI_NO)
#define rst_mosi() DISP_MOSI_PORT &= ~(1<<DISP_MOSI_NO)
#define get_miso() (!!(DISP_MISO_PIN & (1<<DISP_MISO_NO)))

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
__inline__ void delay_dus(uint8_t dus);

/*
 * Zawiesza wykonywanie programu na okreslony czas.
 * 
 * parametry:
 *   us  - minimalny czas oczekiwania w mikrosekundach
 *         (max 100 us)
 */
__inline__ void delay_us(uint8_t us);

/*
 * Zawiesza wykonywanie programu na min. 500ns.
 */
__inline__ void delay_500ns(void);

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

#endif // ! __AVR_PLATF_SPEC_H__
