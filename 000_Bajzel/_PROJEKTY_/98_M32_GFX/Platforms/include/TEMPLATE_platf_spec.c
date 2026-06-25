/****************************************************************************
* Biblioteka obslugi wyswietlacza.
* 
* Wzor pliku funkcji specyficznych dla platformy.
*
* Nazwa pliku: TEMPLATE_platf_spec.c
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

//+++ Dodaj pliki naglowkowe specyficzne dla mikrokontrolera
#include <platf_conf.h>
#include <inttypes.h>
#include <disp_conf.h>

/*
 * Deklaracja zmiennych prywatnych.
 */
static uint16_t prev_STAN_PRZERWAN;

/*
 * Blokuje wykonywanie przerwan.
 */
__inline__ void disable_interrupts(void) {
//+++ Dodaj funkcje zapamietujaca stan przerwan w zmiennej prev_STAN_PRZERWAN
//+++ i tymczasowo blokujaca ich obsluge
}

/*
 * Odblokowuje wykonywanie przerwan, jesli przed wykonaniem funkcji
 * disable_interrupts() byly one odblokowane.
 */
__inline__ void revert_interrupts(void) {
//+++ Dodaj funkcje przywracajaca stan przerwan wg zmiennej prev_STAN_PRZERWAN
}

/*
 * Zawiesza wykonywanie programu na okreslony czas.
 * 
 * parametry:
 *   ms  - minimalny czas oczekiwania w milisekundach (max 255 ms)
 */
void delay_ms(uint8_t ms) {
  #if DISP_OS_PRESENT == 0

//+++ Dodaj funkcje programowego opoznienia

  #else
  
    release(ms);
    
  #endif
}

/*
 * Zawiesza wykonywanie programu na okreslony czas.
 * 
 * parametry:
 *   dus  - minimalny czas oczekiwania w dziesiątkach
 *          mikrosekund (max 10 * 255 us)
 */
void delay_dus(uint8_t dus) {
  while (dus--)
    delay_us(10);
}

/*
 * Zawiesza wykonywanie programu na okreslony czas.
 * 
 * parametry:
 *   us  - minimalny czas oczekiwania w mikrosekundach
 *         (max 100 us)
 */
void delay_us(uint8_t us) {
  while (us--) {
    nop();
    nop();
    nop();
    nop();
    nop();
    nop();
    nop();
    nop();
    nop();
    nop();
  }
}

/*
 * Zawiesza wykonywanie programu na min. 500ns.
 */
void delay_500ns(void) {
    nop();
    nop();
    nop();
    nop();
    nop();
}

/*
 * Zwalnia procesor na okreslony, minimalny czas.
 * (tylko dla implementacji z systemem operacyjnym)
 * 
 * parametry:
 *   ms - minimalny czas zwolnienia w milisekundach (max 255 ms)
 */
void release(uint8_t ms) {
  #if DISP_OS_PRESENT == 1
  
    /* Funkcja powinna zwrocic kontrole nad procesorem
     * systemowi operacyjnemu i powiadomic go o
     * minimalnym czasie zawieszenia aktualnego zadania.
     * (Tylko dla implementacji z systemem operacyjnym,
     * w przeciwnym razie pozostawic puste.)
     */
   
  #endif 
}

/*
 * Zwraca wartosc stalej spod wskazanego adresu.
 * 
 * parametry:
 *  pointer - wskaznik stalej
 */
uint8_t get_const(const uint8_t* pointer) {
  return *pointer;
}

/*
 * Inicjalizuje porty mikrokontrolera.
 */
void init_ports(void) {
  
  // Inicjalizacja portow magistrali
  #if DISP_DB_WIDTH == 8 && DISP_DB_MIX == 0
    
//+++ Dodaj inicjalizacje portow magistrali danych dla interfejsu 8 bit typu not-mixed
//+++ (porty w trybie wyjsciowym, w stanie niskim)
    
  #elif DISP_DB_WIDTH == 4 && DISP_DB_MIX == 0 && defined(DISP_DB_OFFSET)
    
//+++ Dodaj inicjalizacje portow dla interfejsu 4 bit typu not-mixed
//+++ (porty w trybie wyjsciowym, w stanie niskim)
    
  #elif  DISP_DB_WIDTH == 4 && DISP_DB_MIX == 1
    
//+++ Dodaj inicjalizacje portow dla interfejsu 4 bit typu mixed
//+++ (porty w trybie wyjsciowym, w stanie niskim)
    
  #elif  DISP_DB_WIDTH == 8 && DISP_DB_MIX == 1
    
//+++ Dodaj inicjalizacje portow dla interfejsu 8 bit typu mixed
//+++ (porty w trybie wyjsciowym, w stanie niskim)
    
  #elif DISP_DB_WIDTH == 2
    
//+++ Dodaj inicjalizacje portow dla interfejsu SPI (MISO, MOSI)
    
  #elif DISP_DB_WIDTH == 1
      
//+++ Dodaj inicjalizacje portow dla prostego interfejsu szeregowego
//+++ (port MOSI w trybie wyjsciowym, w stanie niskim)
    
  #else
  #  error "Ustawienia magistrali wyswietlacza sa niepoprawne."
  #endif
  
  // Inicjalizacja portow linii specjalnych
  #if defined(DISP_E1_NO)
//+++ Dodaj inicjalizacje linii specjalnej E1
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_E2_NO)
//+++ Dodaj inicjalizacje linii specjalnej E2
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_E3_NO)
//+++ Dodaj inicjalizacje linii specjalnej E3
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_E4_NO)
//+++ Dodaj inicjalizacje linii specjalnej E4
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_E5_NO)
//+++ Dodaj inicjalizacje linii specjalnej E5
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_E6_NO)
//+++ Dodaj inicjalizacje linii specjalnej E6
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_E7_NO)
//+++ Dodaj inicjalizacje linii specjalnej E7
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_E8_NO)
//+++ Dodaj inicjalizacje linii specjalnej E8
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_RS_NO)
//+++ Dodaj inicjalizacje linii specjalnej RS
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_RW_NO)
//+++ Dodaj inicjalizacje linii specjalnej RW
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_CD_NO)
//+++ Dodaj inicjalizacje linii specjalnej CD
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_CE_NO)
//+++ Dodaj inicjalizacje linii specjalnej CE
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_RD_NO)
//+++ Dodaj inicjalizacje linii specjalnej RD
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_WR_NO)
//+++ Dodaj inicjalizacje linii specjalnej WR
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_RES_NO)
//+++ Dodaj inicjalizacje linii specjalnej RES
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_CS1_NO)
//+++ Dodaj inicjalizacje linii specjalnej CS1
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_CS2_NO)
//+++ Dodaj inicjalizacje linii specjalnej CS2
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_CS3_NO)
//+++ Dodaj inicjalizacje linii specjalnej CS3
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_CS4_NO)
//+++ Dodaj inicjalizacje linii specjalnej CS4
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_CS5_NO)
//+++ Dodaj inicjalizacje linii specjalnej CS5
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_CS6_NO)
//+++ Dodaj inicjalizacje linii specjalnej CS6
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_CS7_NO)
//+++ Dodaj inicjalizacje linii specjalnej CS7
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_CS8_NO)
//+++ Dodaj inicjalizacje linii specjalnej CS8
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
  
  #if defined(DISP_CLK_NO)
//+++ Dodaj inicjalizacje linii specjalnej CLK
//+++ (port w trybie wyjsciowym, w stanie niskim)
  #endif
}

/*
 * Zmienia kierunek dzialania magistrali.
 * 
 * parametry:
 *   dir  - kierunek dzialania portu (IN lub OUT)
 */
#if DISP_DB_WIDTH == 8 ||  DISP_DB_WIDTH == 4
__inline__ void chg_dir_db(uint8_t dir) {
  
  #if DISP_DB_WIDTH == 8 && DISP_DB_MIX == 0
  
//+++ Dodaj zmianę kierunku 8-bitowej magistrali danych typu not-mixed
//+++ w zaleznosci od zmiennej dir (= IN - wejscie, = OUT - wyjscie)
//+++ Wyzeruj stan magistrali
    
  #elif DISP_DB_WIDTH == 4 && DISP_DB_MIX == 0
    
//+++ Dodaj zmianę kierunku 4-bitowej magistrali danych typu not-mixed
//+++ w zaleznosci od zmiennej dir (= IN - wejscie, = OUT - wyjscie)
//+++ Wyzeruj stan magistrali
    
  #elif DISP_DB_WIDTH == 4 && DISP_DB_MIX == 1
  
//+++ Dodaj zmianę kierunku 4-bitowej magistrali danych typu mixed
//+++ w zaleznosci od zmiennej dir (= IN - wejscie, = OUT - wyjscie)
//+++ Wyzeruj stan magistrali
    
  #elif DISP_DB_WIDTH == 8 && DISP_DB_MIX == 1
    
//+++ Dodaj zmianę kierunku 8-bitowej magistrali danych typu mixed
//+++ w zaleznosci od zmiennej dir (= IN - wejscie, = OUT - wyjscie)
//+++ Wyzeruj stan magistrali
    
  #endif
}
#endif

/*
 * Wystawia dane na magistrale.
 * 
 * parametry:
 *   data  - dane do wystawienia
 */
__inline__ void write_db(uint8_t data) {
  #if DISP_DB_WIDTH == 8 && DISP_DB_MIX == 0

//+++ Dodaj wystawianie zmiennej data na magistrale 8-bitowa typu not-mixed
    
  #elif DISP_DB_WIDTH == 4 && DISP_DB_MIX == 0

//+++ Dodaj wystawianie zmiennej data (4 LSB) na magistrale 4-bitowa typu not-mixed
    
  #elif DISP_DB_WIDTH == 4 && DISP_DB_MIX == 1
  
//+++ Dodaj wystawianie zmiennej data (4 LSB) na magistrale 4-bitowa typu mixed
    
  #elif DISP_DB_WIDTH == 8 && DISP_DB_MIX == 1
    
//+++ Dodaj wystawianie zmiennej data na magistrale 8-bitowa typu mixed
    
  #elif (DISP_DB_WIDTH == 1 || DISP_DB_WIDTH == 2) 

//+++ Dodaj wystawianie zmiennej data (jej bitu LSB) na linie MOSI interfejsu SPI
    
  #endif
}

/*
 * Odczytuje dane z magistrali.
 * 
 * zwraca:
 *   wartosc odczytana z magistrali
 */
__inline__ uint8_t read_db(void) {
  #if DISP_DB_WIDTH == 8 && DISP_DB_MIX == 0

//+++ Dodaj zwracanie danych z magistrali 8-bit typu not-mixed
    
  #elif DISP_DB_WIDTH == 4 && DISP_DB_MIX == 0

//+++ Dodaj zwracanie danych z magistrali 4-bit typu not-mixed
    
  #elif DISP_DB_WIDTH == 4 && DISP_DB_MIX == 1
  
//+++ Dodaj zwracanie danych z magistrali 4-bit typu mixed
    
  #elif DISP_DB_WIDTH == 8 && DISP_DB_MIX == 1
    
//+++ Dodaj zwracanie danych z magistrali 8-bit typu mixed
    
  #elif DISP_DB_WIDTH == 2

//+++ Dodaj zwracanie danych z linii MISO interfejsu SPI
    
  #else
  
    return 0;
    
  #endif
}

