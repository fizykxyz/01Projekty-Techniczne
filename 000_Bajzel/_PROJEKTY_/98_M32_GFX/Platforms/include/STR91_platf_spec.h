/****************************************************************************
* Biblioteka obslugi wyswietlacza.
* 
* Plik naglowkowy funkcji specyficznych dla platformy ARM9 STR91xx.
*
* Nazwa pliku: STR91_platf_spec.h
* Data: 19.01.2008
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

#ifndef __STR91_PLATF_SPEC_H__
#define __STR91_PLATF_SPEC_H__ 1

#include <91x_map.h>
#include <91x_scu.h>
#include <inttypes.h>
#include <disp_conf.h>

#ifndef F_CPU
#  warning "Nie okreslono czestotliwosci pracy CPU (F_CPU). Przyjeto 25 MHz."
#  define F_CPU 25000000UL
#endif

/*
 * Deklaracja wartosci okreslajacych kierunek portow.
 */
#define IN 0
#define OUT 1

/*
 * Deklaracje wartosci specyficznych dla ARM9.
 */
//#define INT_MSK 0x00C0

#if DISP_DB_WIDTH == 8
 #define DISP_DB_DATA   DISP_DB_DR(0xFF<<2)
#elif DISP_DB_WIDTH == 4
 #define DISP_DB_DATA   DISP_DB_DR(0x0F<<(2+DISP_DB_OFFSET))
#endif

/*
 * Ustawiaja badz zeruja poszczegolne linie specjalne.
 */
#define set_rs() DISP_RS_DR = (1<<DISP_RS_NO)
#define rst_rs() DISP_RS_DR = 0
#define get_rs() (!!(DISP_RS_DR))
#define set_rw() DISP_RW_DR = (1<<DISP_RW_NO)
#define rst_rw() DISP_RW_DR = 0
#define get_rw() (!!(DISP_RW_DR))
#define set_en1() DISP_E1_DR = (1<<DISP_E1_NO)
#define rst_en1() DISP_E1_DR = 0
#define set_en2() DISP_E2_DR = (1<<DISP_E2_NO)
#define rst_en2() DISP_E2_DR = 0
#define set_en3() DISP_E3_DR = (1<<DISP_E3_NO)
#define rst_en3() DISP_E3_DR = 0
#define set_en4() DISP_E4_DR = (1<<DISP_E4_NO)
#define rst_en4() DISP_E4_DR = 0
#define set_en5() DISP_E5_DR = (1<<DISP_E5_NO)
#define rst_en5() DISP_E5_DR = 0
#define set_en6() DISP_E6_DR = (1<<DISP_E6_NO)
#define rst_en6() DISP_E6_DR = 0
#define set_en7() DISP_E7_DR = (1<<DISP_E7_NO)
#define rst_en7() DISP_E7_DR = 0
#define set_en8() DISP_E8_DR = (1<<DISP_E8_NO)
#define rst_en8() DISP_E8_DR = 0
#define set_cd() DISP_CD_DR = (1<<DISP_CD_NO)
#define rst_cd() DISP_CD_DR = 0
#define get_cd() (!!(DISP_CD_DR))
#define set_ce() DISP_CE_DR = (1<<DISP_CE_NO)
#define rst_ce() DISP_CE_DR = 0
#define set_rd() DISP_RD_DR = (1<<DISP_RD_NO)
#define rst_rd() DISP_RD_DR = 0
#define set_wr() DISP_WR_DR = (1<<DISP_WR_NO)
#define rst_wr() DISP_WR_DR = 0
#define set_res() DISP_RES_DR = (1<<DISP_RES_NO)
#define rst_res() DISP_RES_DR = 0
#define set_cs1() DISP_CS1_DR = (1<<DISP_CS1_NO)
#define rst_cs1() DISP_CS1_DR = 0
#define set_cs2() DISP_CS2_DR = (1<<DISP_CS2_NO)
#define rst_cs2() DISP_CS2_DR = 0
#define set_cs3() DISP_CS3_DR = (1<<DISP_CS3_NO)
#define rst_cs3() DISP_CS3_DR = 0
#define set_cs4() DISP_CS4_DR = (1<<DISP_CS4_NO)
#define rst_cs4() DISP_CS4_DR = 0
#define set_cs5() DISP_CS5_DR = (1<<DISP_CS5_NO)
#define rst_cs5() DISP_CS5_DR = 0
#define set_cs6() DISP_CS6_DR = (1<<DISP_CS6_NO)
#define rst_cs6() DISP_CS6_DR = 0
#define set_cs7() DISP_CS7_DR = (1<<DISP_CS7_NO)
#define rst_cs7() DISP_CS7_DR = 0
#define set_cs8() DISP_CS8_DR = (1<<DISP_CS8_NO)
#define rst_cs8() DISP_CS8_DR = 0
#define set_clk() DISP_CLK_DR = (1<<DISP_CLK_NO)
#define rst_clk() DISP_CLK_DR = 0
#define set_mosi() DISP_MOSI_DR = (1<<DISP_MOSI_NO)
#define rst_mosi() DISP_MOSI_DR = 0
#define get_miso() (!!(DISP_MISO_DR))

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

#endif // ! __STR91_PLATF_SPEC_H__
