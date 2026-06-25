/****************************************************************************
* Biblioteka obslugi wyswietlacza.
* 
* Plik funkcji specyficznych dla platformy ARM9 STR91xx.
*
* Nazwa pliku: STR91_platf_spec.c
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

#include <platf_conf.h>
#include <91x_map.h>
#include <91x_scu.h>
#include <inttypes.h>
#include <disp_conf.h>

/*
 * Deklaracja zmiennych prywatnych.
 */
//static uint16_t prev_STAN_PRZERWAN;

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

    while(ms--)
      for (uint8_t i=0; i<(50.0*F_CPU/25000000.0); i++)
        for (uint8_t j=0; j<35; j++)
          asm volatile("nop\n\t"::);

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
  #if defined(DISP_DB_GP) && defined(DISP_DB_DR) && defined(DISP_DB_DDR) \
      && defined(DISP_DB_TYPE) && defined(DISP_DB_IN) && defined(DISP_DB_OUT) \
      && DISP_DB_WIDTH == 8 && DISP_DB_MIX == 0
    
    SCU->PRR1 |= DISP_DB_GP;
    SCU->PCGR1 |= DISP_DB_GP;
    DISP_DB_TYPE = 0x00;
    DISP_DB_IN  = 0x00;
    DISP_DB_OUT = 0x5555;
    DISP_DB_DDR = 0xFF;
    DISP_DB_DATA = 0;
    
  #elif  defined(DISP_DB_GP) && defined(DISP_DB_DR) && defined(DISP_DB_DDR) \
      && defined(DISP_DB_TYPE) && defined(DISP_DB_IN) && defined(DISP_DB_OUT) \
      && DISP_DB_WIDTH == 4 && DISP_DB_MIX == 0 && defined(DISP_DB_OFFSET)
    
    SCU->PRR1 |= DISP_DB_GP;
    SCU->PCGR1 |= DISP_DB_GP;
    DISP_DB_TYPE &= ~(0x0F<<DISP_DB_OFFSET);
    DISP_DB_IN  &= ~(0x0F<<DISP_DB_OFFSET);
    DISP_DB_OUT &= ~(0xFF<<(2*DISP_DB_OFFSET));
    DISP_DB_OUT |= (0x55<<(2*DISP_DB_OFFSET));
    DISP_DB_DDR |= (0x0F<<DISP_DB_OFFSET);
    DISP_DB_DATA = 0;
    
  #elif  defined(DISP_DB4_GP) && defined(DISP_DB4_DR) && defined(DISP_DB4_DDR) && defined(DISP_DB4_TYPE) && defined(DISP_DB4_IN) && defined(DISP_DB4_OUT) && defined(DISP_DB4_NO) \
      && defined(DISP_DB5_GP) && defined(DISP_DB5_DR) && defined(DISP_DB5_DDR) && defined(DISP_DB5_TYPE) && defined(DISP_DB5_IN) && defined(DISP_DB5_OUT) && defined(DISP_DB5_NO) \
      && defined(DISP_DB6_GP) && defined(DISP_DB6_DR) && defined(DISP_DB6_DDR) && defined(DISP_DB6_TYPE) && defined(DISP_DB6_IN) && defined(DISP_DB6_OUT) && defined(DISP_DB6_NO) \
      && defined(DISP_DB7_GP) && defined(DISP_DB7_DR) && defined(DISP_DB7_DDR) && defined(DISP_DB7_TYPE) && defined(DISP_DB7_IN) && defined(DISP_DB7_OUT) && defined(DISP_DB7_NO) \
      && DISP_DB_WIDTH == 4 && DISP_DB_MIX == 1
    
    SCU->PRR1 |= DISP_DB4_GP | DISP_DB5_GP | DISP_DB6_GP | DISP_DB7_GP;
    SCU->PCGR1 |= DISP_DB4_GP | DISP_DB5_GP | DISP_DB6_GP | DISP_DB7_GP;
    
    DISP_DB4_TYPE &= ~(1<<DISP_DB4_NO);
    DISP_DB4_IN  &= ~(1<<DISP_DB4_NO);
    DISP_DB4_OUT &= ~(0x03<<(2*DISP_DB4_NO));
    DISP_DB4_OUT |= (0x01<<(2*DISP_DB4_NO));
    DISP_DB4_DDR |= (1<<DISP_DB4_NO);
    DISP_DB4_DR = 0;
    DISP_DB5_TYPE &= ~(1<<DISP_DB5_NO);
    DISP_DB5_IN  &= ~(1<<DISP_DB5_NO);
    DISP_DB5_OUT &= ~(0x03<<(2*DISP_DB5_NO));
    DISP_DB5_OUT |= (0x01<<(2*DISP_DB5_NO));
    DISP_DB5_DDR |= (1<<DISP_DB5_NO);
    DISP_DB5_DR = 0;
    DISP_DB6_TYPE &= ~(1<<DISP_DB6_NO);
    DISP_DB6_IN  &= ~(1<<DISP_DB6_NO);
    DISP_DB6_OUT &= ~(0x03<<(2*DISP_DB6_NO));
    DISP_DB6_OUT |= (0x01<<(2*DISP_DB6_NO));
    DISP_DB6_DDR |= (1<<DISP_DB6_NO);
    DISP_DB6_DR = 0;
    DISP_DB7_TYPE &= ~(1<<DISP_DB7_NO);
    DISP_DB7_IN  &= ~(1<<DISP_DB7_NO);
    DISP_DB7_OUT &= ~(0x03<<(2*DISP_DB7_NO));
    DISP_DB7_OUT |= (0x01<<(2*DISP_DB7_NO));
    DISP_DB7_DDR |= (1<<DISP_DB7_NO);
    DISP_DB7_DR = 0;
    
    
  #elif  defined(DISP_DB0_GP) && defined(DISP_DB0_DR) && defined(DISP_DB0_DDR) && defined(DISP_DB0_TYPE) && defined(DISP_DB0_IN) && defined(DISP_DB0_OUT) && defined(DISP_DB0_NO) \
      && defined(DISP_DB1_GP) && defined(DISP_DB1_DR) && defined(DISP_DB1_DDR) && defined(DISP_DB1_TYPE) && defined(DISP_DB1_IN) && defined(DISP_DB1_OUT) && defined(DISP_DB1_NO) \
      && defined(DISP_DB2_GP) && defined(DISP_DB2_DR) && defined(DISP_DB2_DDR) && defined(DISP_DB2_TYPE) && defined(DISP_DB2_IN) && defined(DISP_DB2_OUT) && defined(DISP_DB2_NO) \
      && defined(DISP_DB3_GP) && defined(DISP_DB3_DR) && defined(DISP_DB3_DDR) && defined(DISP_DB3_TYPE) && defined(DISP_DB3_IN) && defined(DISP_DB3_OUT) && defined(DISP_DB3_NO) \
      && defined(DISP_DB4_GP) && defined(DISP_DB4_DR) && defined(DISP_DB4_DDR) && defined(DISP_DB4_TYPE) && defined(DISP_DB4_IN) && defined(DISP_DB4_OUT) && defined(DISP_DB4_NO) \
      && defined(DISP_DB5_GP) && defined(DISP_DB5_DR) && defined(DISP_DB5_DDR) && defined(DISP_DB5_TYPE) && defined(DISP_DB5_IN) && defined(DISP_DB5_OUT) && defined(DISP_DB5_NO) \
      && defined(DISP_DB6_GP) && defined(DISP_DB6_DR) && defined(DISP_DB6_DDR) && defined(DISP_DB6_TYPE) && defined(DISP_DB6_IN) && defined(DISP_DB6_OUT) && defined(DISP_DB6_NO) \
      && defined(DISP_DB7_GP) && defined(DISP_DB7_DR) && defined(DISP_DB7_DDR) && defined(DISP_DB7_TYPE) && defined(DISP_DB7_IN) && defined(DISP_DB7_OUT) && defined(DISP_DB7_NO) \
      && DISP_DB_WIDTH == 8 && DISP_DB_MIX == 1
    
    SCU->PRR1 |= DISP_DB0_GP | DISP_DB1_GP | DISP_DB2_GP | DISP_DB3_GP \
                | DISP_DB4_GP | DISP_DB5_GP | DISP_DB6_GP | DISP_DB7_GP;
    SCU->PCGR1 |= DISP_DB0_GP | DISP_DB1_GP | DISP_DB2_GP | DISP_DB3_GP \
                 | DISP_DB4_GP | DISP_DB5_GP | DISP_DB6_GP | DISP_DB7_GP;
    
    DISP_DB0_TYPE &= ~(1<<DISP_DB0_NO);
    DISP_DB0_IN  &= ~(1<<DISP_DB0_NO);
    DISP_DB0_OUT &= ~(0x03<<(2*DISP_DB0_NO));
    DISP_DB0_OUT |= (0x01<<(2*DISP_DB0_NO));
    DISP_DB0_DDR |= (1<<DISP_DB0_NO);
    DISP_DB0_DR = 0;
    DISP_DB1_TYPE &= ~(1<<DISP_DB1_NO);
    DISP_DB1_IN  &= ~(1<<DISP_DB1_NO);
    DISP_DB1_OUT &= ~(0x03<<(2*DISP_DB1_NO));
    DISP_DB1_OUT |= (0x01<<(2*DISP_DB1_NO));
    DISP_DB1_DDR |= (1<<DISP_DB1_NO);
    DISP_DB1_DR = 0;
    DISP_DB2_TYPE &= ~(1<<DISP_DB2_NO);
    DISP_DB2_IN  &= ~(1<<DISP_DB2_NO);
    DISP_DB2_OUT &= ~(0x03<<(2*DISP_DB2_NO));
    DISP_DB2_OUT |= (0x01<<(2*DISP_DB2_NO));
    DISP_DB2_DDR |= (1<<DISP_DB2_NO);
    DISP_DB2_DR = 0;
    DISP_DB3_TYPE &= ~(1<<DISP_DB3_NO);
    DISP_DB3_IN  &= ~(1<<DISP_DB3_NO);
    DISP_DB3_OUT &= ~(0x03<<(2*DISP_DB3_NO));
    DISP_DB3_OUT |= (0x01<<(2*DISP_DB3_NO));
    DISP_DB3_DDR |= (1<<DISP_DB3_NO);
    DISP_DB3_DR = 0;
    DISP_DB4_TYPE &= ~(1<<DISP_DB4_NO);
    DISP_DB4_IN  &= ~(1<<DISP_DB4_NO);
    DISP_DB4_OUT &= ~(0x03<<(2*DISP_DB4_NO));
    DISP_DB4_OUT |= (0x01<<(2*DISP_DB4_NO));
    DISP_DB4_TYPE &= ~(1<<DISP_DB4_NO);
    DISP_DB4_IN  &= ~(1<<DISP_DB4_NO);
    DISP_DB4_OUT &= ~(0x03<<(2*DISP_DB4_NO));
    DISP_DB4_OUT |= (0x01<<(2*DISP_DB4_NO));
    DISP_DB4_DDR |= (1<<DISP_DB4_NO);
    DISP_DB4_DR = 0;
    DISP_DB5_TYPE &= ~(1<<DISP_DB5_NO);
    DISP_DB5_IN  &= ~(1<<DISP_DB5_NO);
    DISP_DB5_OUT &= ~(0x03<<(2*DISP_DB5_NO));
    DISP_DB5_OUT |= (0x01<<(2*DISP_DB5_NO));
    DISP_DB5_DDR |= (1<<DISP_DB5_NO);
    DISP_DB5_DR = 0;
    DISP_DB6_TYPE &= ~(1<<DISP_DB6_NO);
    DISP_DB6_IN  &= ~(1<<DISP_DB6_NO);
    DISP_DB6_OUT &= ~(0x03<<(2*DISP_DB6_NO));
    DISP_DB6_OUT |= (0x01<<(2*DISP_DB6_NO));
    DISP_DB6_DDR |= (1<<DISP_DB6_NO);
    DISP_DB6_DR = 0;
    DISP_DB7_TYPE &= ~(1<<DISP_DB7_NO);
    DISP_DB7_IN  &= ~(1<<DISP_DB7_NO);
    DISP_DB7_OUT &= ~(0x03<<(2*DISP_DB7_NO));
    DISP_DB7_OUT |= (0x01<<(2*DISP_DB7_NO));
    DISP_DB7_DDR |= (1<<DISP_DB7_NO);
    DISP_DB7_DR = 0;
    
  #elif defined(DISP_MOSI_GP) && defined(DISP_MOSI_DR) && defined(DISP_MOSI_DDR) && defined(DISP_MOSI_TYPE) && defined(DISP_MOSI_IN) && defined(DISP_MOSI_OUT) && defined(DISP_MOSI_NO) \
      && defined(DISP_MISO_GP) && defined(DISP_MISO_DR) && defined(DISP_MISO_DDR) && defined(DISP_MISO_TYPE) && defined(DISP_MISO_IN) && defined(DISP_MISO_OUT) &&defined(DISP_MISO_NO) \
      && DISP_DB_WIDTH == 2
      
    SCU->PRR1 |= DISP_MOSI_GP | DISP_MISO_GP;
    SCU->PCGR1 |= DISP_MOSI_GP | DISP_MISO_GP;
    
    DISP_MOSI_TYPE &= ~(1<<DISP_MOSI_NO);
    DISP_MOSI_IN  &= ~(1<<DISP_MOSI_NO);
    DISP_MOSI_OUT &= ~(0x03<<(2*DISP_MOSI_NO));
    DISP_MOSI_OUT |= (0x01<<(2*DISP_MOSI_NO));
    DISP_MOSI_DDR |= (1<<DISP_MOSI_NO);
    DISP_MOSI_DR = 0;
    
    DISP_MISO_TYPE &= ~(1<<DISP_MISO_NO);
    DISP_MISO_IN  &= ~(1<<DISP_MISO_NO);
    DISP_MISO_OUT &= ~(0x03<<(2*DISP_MISO_NO));
    DISP_MISO_DDR &= ~(1<<DISP_MISO_NO);
    DISP_MISO_DR = 0;
    
  #elif defined(DISP_MOSI_GP) && defined(DISP_MOSI_DR) && defined(DISP_MOSI_DDR) && defined(DISP_MOSI_TYPE) && defined(DISP_MOSI_IN) && defined(DISP_MOSI_OUT) && defined(DISP_MOSI_NO) \
      && DISP_DB_WIDTH == 1
    
    SCU->PRR1 |= DISP_MOSI_GP;
    SCU->PCGR1 |= DISP_MOSI_GP;
    
    DISP_MOSI_TYPE &= ~(1<<DISP_MOSI_NO);
    DISP_MOSI_IN  &= ~(1<<DISP_MOSI_NO);
    DISP_MOSI_OUT &= ~(0x03<<(2*DISP_MOSI_NO));
    DISP_MOSI_OUT |= (0x01<<(2*DISP_MOSI_NO));
    DISP_MOSI_DDR |= (1<<DISP_MOSI_NO);
    DISP_MOSI_DR = 0;
    
  #else
  #  error "Ustawienia magistrali wyswietlacza (stale z prefiksem DISP_DB) sa niepoprawne."
  #endif
  
  // Inicjalizacja portow linii specjalnych
  #if defined(DISP_E1_GP) && defined(DISP_E1_DR) && defined(DISP_E1_DDR) && defined(DISP_E1_TYPE) && defined(DISP_E1_IN) && defined(DISP_E1_OUT) && defined(DISP_E1_NO) 
    SCU->PRR1 |= DISP_E1_GP;
    SCU->PCGR1 |= DISP_E1_GP;
    
    DISP_E1_TYPE &= ~(1<<DISP_E1_NO);
    DISP_E1_IN  &= ~(1<<DISP_E1_NO);
    DISP_E1_OUT &= ~(0x03<<(2*DISP_E1_NO));
    DISP_E1_OUT |= (0x01<<(2*DISP_E1_NO));
    DISP_E1_DDR |= (1<<DISP_E1_NO);
    DISP_E1_DR = 0;
  #endif
  
  #if defined(DISP_E2_GP) && defined(DISP_E2_DR) && defined(DISP_E2_DDR) && defined(DISP_E2_TYPE) && defined(DISP_E2_IN) && defined(DISP_E2_OUT) && defined(DISP_E2_NO)
    SCU->PRR1 |= DISP_E2_GP;
    SCU->PCGR1 |= DISP_E2_GP;
    
    DISP_E2_TYPE &= ~(1<<DISP_E2_NO);
    DISP_E2_IN  &= ~(1<<DISP_E2_NO);
    DISP_E2_OUT &= ~(0x03<<(2*DISP_E2_NO));
    DISP_E2_OUT |= (0x01<<(2*DISP_E2_NO));
    DISP_E2_DDR |= (1<<DISP_E2_NO);
    DISP_E2_DR = 0;
  #endif
  
  #if defined(DISP_E3_GP) && defined(DISP_E3_DR) && defined(DISP_E3_DDR) && defined(DISP_E3_TYPE) && defined(DISP_E3_IN) && defined(DISP_E3_OUT) && defined(DISP_E3_NO)
    SCU->PRR1 |= DISP_E3_GP;
    SCU->PCGR1 |= DISP_E3_GP;
    
    DISP_E3_TYPE &= ~(1<<DISP_E3_NO);
    DISP_E3_IN  &= ~(1<<DISP_E3_NO);
    DISP_E3_OUT &= ~(0x03<<(2*DISP_E3_NO));
    DISP_E3_OUT |= (0x01<<(2*DISP_E3_NO));
    DISP_E3_DDR |= (1<<DISP_E3_NO);
    DISP_E3_DR = 0;
  #endif
  
  #if defined(DISP_E4_GP) && defined(DISP_E4_DR) && defined(DISP_E4_DDR) && defined(DISP_E4_TYPE) && defined(DISP_E4_IN) && defined(DISP_E4_OUT) && defined(DISP_E4_NO)
    SCU->PRR1 |= DISP_E4_GP;
    SCU->PCGR1 |= DISP_E4_GP;
    
    DISP_E4_TYPE &= ~(1<<DISP_E4_NO);
    DISP_E4_IN  &= ~(1<<DISP_E4_NO);
    DISP_E4_OUT &= ~(0x03<<(2*DISP_E4_NO));
    DISP_E4_OUT |= (0x01<<(2*DISP_E4_NO));
    DISP_E4_DDR |= (1<<DISP_E4_NO);
    DISP_E4_DR = 0;
  #endif
  
  #if defined(DISP_E5_GP) && defined(DISP_E5_DR) && defined(DISP_E5_DDR) && defined(DISP_E5_TYPE) && defined(DISP_E5_IN) && defined(DISP_E5_OUT) && defined(DISP_E5_NO)
    SCU->PRR1 |= DISP_E5_GP;
    SCU->PCGR1 |= DISP_E5_GP;
    
    DISP_E5_TYPE &= ~(1<<DISP_E5_NO);
    DISP_E5_IN  &= ~(1<<DISP_E5_NO);
    DISP_E5_OUT &= ~(0x03<<(2*DISP_E5_NO));
    DISP_E5_OUT |= (0x01<<(2*DISP_E5_NO));
    DISP_E5_DDR |= (1<<DISP_E5_NO);
    DISP_E5_DR = 0;
  #endif
  
  #if defined(DISP_E6_GP) && defined(DISP_E6_DR) && defined(DISP_E6_DDR) && defined(DISP_E6_TYPE) && defined(DISP_E6_IN) && defined(DISP_E6_OUT) && defined(DISP_E6_NO)
    SCU->PRR1 |= DISP_E6_GP;
    SCU->PCGR1 |= DISP_E6_GP;
    
    DISP_E6_TYPE &= ~(1<<DISP_E6_NO);
    DISP_E6_IN  &= ~(1<<DISP_E6_NO);
    DISP_E6_OUT &= ~(0x03<<(2*DISP_E6_NO));
    DISP_E6_OUT |= (0x01<<(2*DISP_E6_NO));
    DISP_E6_DDR |= (1<<DISP_E6_NO);
    DISP_E6_DR = 0;
  #endif
  
  #if defined(DISP_E7_GP) && defined(DISP_E7_DR) && defined(DISP_E7_DDR) && defined(DISP_E7_TYPE) && defined(DISP_E7_IN) && defined(DISP_E7_OUT) && defined(DISP_E7_NO)
    SCU->PRR1 |= DISP_E7_GP;
    SCU->PCGR1 |= DISP_E7_GP;
    
    DISP_E7_TYPE &= ~(1<<DISP_E7_NO);
    DISP_E7_IN  &= ~(1<<DISP_E7_NO);
    DISP_E7_OUT &= ~(0x03<<(2*DISP_E7_NO));
    DISP_E7_OUT |= (0x01<<(2*DISP_E7_NO));
    DISP_E7_DDR |= (1<<DISP_E7_NO);
    DISP_E7_DR = 0;
  #endif
  
  #if defined(DISP_E8_GP) && defined(DISP_E8_DR) && defined(DISP_E8_DDR) && defined(DISP_E8_TYPE) && defined(DISP_E8_IN) && defined(DISP_E8_OUT) && defined(DISP_E8_NO)
    SCU->PRR1 |= DISP_E8_GP;
    SCU->PCGR1 |= DISP_E8_GP;
    
    DISP_E8_TYPE &= ~(1<<DISP_E8_NO);
    DISP_E8_IN  &= ~(1<<DISP_E8_NO);
    DISP_E8_OUT &= ~(0x03<<(2*DISP_E8_NO));
    DISP_E8_OUT |= (0x01<<(2*DISP_E8_NO));
    DISP_E8_DDR |= (1<<DISP_E8_NO);
    DISP_E8_DR = 0;
  #endif
  
  #if defined(DISP_RS_GP) && defined(DISP_RS_DR) && defined(DISP_RS_DDR) && defined(DISP_RS_TYPE) && defined(DISP_RS_IN) && defined(DISP_RS_OUT) && defined(DISP_RS_NO)
    SCU->PRR1 |= DISP_RS_GP;
    SCU->PCGR1 |= DISP_RS_GP;
    
    DISP_RS_TYPE &= ~(1<<DISP_RS_NO);
    DISP_RS_IN  &= ~(1<<DISP_RS_NO);
    DISP_RS_OUT &= ~(0x03<<(2*DISP_RS_NO));
    DISP_RS_OUT |= (0x01<<(2*DISP_RS_NO));
    DISP_RS_DDR |= (1<<DISP_RS_NO);
    DISP_RS_DR = 0;
  #endif
  
  #if defined(DISP_RW_GP) && defined(DISP_RW_DR) && defined(DISP_RW_DDR) && defined(DISP_RW_TYPE) && defined(DISP_RW_IN) && defined(DISP_RW_OUT) && defined(DISP_RW_NO)
    SCU->PRR1 |= DISP_RW_GP;
    SCU->PCGR1 |= DISP_RW_GP;
    
    DISP_RW_TYPE &= ~(1<<DISP_RW_NO);
    DISP_RW_IN  &= ~(1<<DISP_RW_NO);
    DISP_RW_OUT &= ~(0x03<<(2*DISP_RW_NO));
    DISP_RW_OUT |= (0x01<<(2*DISP_RW_NO));
    DISP_RW_DDR |= (1<<DISP_RW_NO);
    DISP_RW_DR = 0;
  #endif
  
  #if defined(DISP_CD_GP) && defined(DISP_CD_DR) && defined(DISP_CD_DDR) && defined(DISP_CD_TYPE) && defined(DISP_CD_IN) && defined(DISP_CD_OUT) && defined(DISP_CD_NO)
    SCU->PRR1 |= DISP_CD_GP;
    SCU->PCGR1 |= DISP_CD_GP;
    
    DISP_CD_TYPE &= ~(1<<DISP_CD_NO);
    DISP_CD_IN  &= ~(1<<DISP_CD_NO);
    DISP_CD_OUT &= ~(0x03<<(2*DISP_CD_NO));
    DISP_CD_OUT |= (0x01<<(2*DISP_CD_NO));
    DISP_CD_DDR |= (1<<DISP_CD_NO);
    DISP_CD_DR = 0;
  #endif
  
  #if defined(DISP_CE_GP) && defined(DISP_CE_DR) && defined(DISP_CE_DDR) && defined(DISP_CE_TYPE) && defined(DISP_CE_IN) && defined(DISP_CE_OUT) && defined(DISP_CE_NO)
    SCU->PRR1 |= DISP_CE_GP;
    SCU->PCGR1 |= DISP_CE_GP;
    
    DISP_CE_TYPE &= ~(1<<DISP_CE_NO);
    DISP_CE_IN  &= ~(1<<DISP_CE_NO);
    DISP_CE_OUT &= ~(0x03<<(2*DISP_CE_NO));
    DISP_CE_OUT |= (0x01<<(2*DISP_CE_NO));
    DISP_CE_DDR |= (1<<DISP_CE_NO);
    DISP_CE_DR = 0;
  #endif
  
  #if defined(DISP_RD_GP) && defined(DISP_RD_DR) && defined(DISP_RD_DDR) && defined(DISP_RD_TYPE) && defined(DISP_RD_IN) && defined(DISP_RD_OUT) && defined(DISP_RD_NO)
    SCU->PRR1 |= DISP_RD_GP;
    SCU->PCGR1 |= DISP_RD_GP;
    
    DISP_RD_TYPE &= ~(1<<DISP_RD_NO);
    DISP_RD_IN  &= ~(1<<DISP_RD_NO);
    DISP_RD_OUT &= ~(0x03<<(2*DISP_RD_NO));
    DISP_RD_OUT |= (0x01<<(2*DISP_RD_NO));
    DISP_RD_DDR |= (1<<DISP_RD_NO);
    DISP_RD_DR = 0;
  #endif
  
  #if defined(DISP_WR_GP) && defined(DISP_WR_DR) && defined(DISP_WR_DDR) && defined(DISP_WR_TYPE) && defined(DISP_WR_IN) && defined(DISP_WR_OUT) && defined(DISP_WR_NO)
    SCU->PRR1 |= DISP_WR_GP;
    SCU->PCGR1 |= DISP_WR_GP;
    
    DISP_WR_TYPE &= ~(1<<DISP_WR_NO);
    DISP_WR_IN  &= ~(1<<DISP_WR_NO);
    DISP_WR_OUT &= ~(0x03<<(2*DISP_WR_NO));
    DISP_WR_OUT |= (0x01<<(2*DISP_WR_NO));
    DISP_WR_DDR |= (1<<DISP_WR_NO);
    DISP_WR_DR = 0;
  #endif
  
  #if defined(DISP_RES_GP) && defined(DISP_RES_DR) && defined(DISP_RES_DDR) && defined(DISP_RES_TYPE) && defined(DISP_RES_IN) && defined(DISP_RES_OUT) && defined(DISP_RES_NO)
    SCU->PRR1 |= DISP_RES_GP;
    SCU->PCGR1 |= DISP_RES_GP;
    
    DISP_RES_TYPE &= ~(1<<DISP_RES_NO);
    DISP_RES_IN  &= ~(1<<DISP_RES_NO);
    DISP_RES_OUT &= ~(0x03<<(2*DISP_RES_NO));
    DISP_RES_OUT |= (0x01<<(2*DISP_RES_NO));
    DISP_RES_DDR |= (1<<DISP_RES_NO);
    DISP_RES_DR = 0;
  #endif
  
  #if defined(DISP_CS1_GP) && defined(DISP_CS1_DR) && defined(DISP_CS1_DDR) && defined(DISP_CS1_TYPE) && defined(DISP_CS1_IN) && defined(DISP_CS1_OUT) && defined(DISP_CS1_NO)
    SCU->PRR1 |= DISP_CS1_GP;
    SCU->PCGR1 |= DISP_CS1_GP;
    
    DISP_CS1_TYPE &= ~(1<<DISP_CS1_NO);
    DISP_CS1_IN  &= ~(1<<DISP_CS1_NO);
    DISP_CS1_OUT &= ~(0x03<<(2*DISP_CS1_NO));
    DISP_CS1_OUT |= (0x01<<(2*DISP_CS1_NO));
    DISP_CS1_DDR |= (1<<DISP_CS1_NO);
    DISP_CS1_DR = 0;
  #endif
  
  #if defined(DISP_CS2_GP) && defined(DISP_CS2_DR) && defined(DISP_CS2_DDR) && defined(DISP_CS2_TYPE) && defined(DISP_CS2_IN) && defined(DISP_CS2_OUT) && defined(DISP_CS2_NO)
    SCU->PRR1 |= DISP_CS2_GP;
    SCU->PCGR1 |= DISP_CS2_GP;
    
    DISP_CS2_TYPE &= ~(1<<DISP_CS2_NO);
    DISP_CS2_IN  &= ~(1<<DISP_CS2_NO);
    DISP_CS2_OUT &= ~(0x03<<(2*DISP_CS2_NO));
    DISP_CS2_OUT |= (0x01<<(2*DISP_CS2_NO));
    DISP_CS2_DDR |= (1<<DISP_CS2_NO);
    DISP_CS2_DR = 0;
  #endif
  
  #if defined(DISP_CS3_GP) && defined(DISP_CS3_DR) && defined(DISP_CS3_DDR) && defined(DISP_CS3_TYPE) && defined(DISP_CS3_IN) && defined(DISP_CS3_OUT) && defined(DISP_CS3_NO)
    SCU->PRR1 |= DISP_CS3_GP;
    SCU->PCGR1 |= DISP_CS3_GP;
    
    DISP_CS3_TYPE &= ~(1<<DISP_CS3_NO);
    DISP_CS3_IN  &= ~(1<<DISP_CS3_NO);
    DISP_CS3_OUT &= ~(0x03<<(2*DISP_CS3_NO));
    DISP_CS3_OUT |= (0x01<<(2*DISP_CS3_NO));
    DISP_CS3_DDR |= (1<<DISP_CS3_NO);
    DISP_CS3_DR = 0;
  #endif
  
  #if defined(DISP_CS4_GP) && defined(DISP_CS4_DR) && defined(DISP_CS4_DDR) && defined(DISP_CS4_TYPE) && defined(DISP_CS4_IN) && defined(DISP_CS4_OUT) && defined(DISP_CS4_NO)
    SCU->PRR1 |= DISP_CS4_GP;
    SCU->PCGR1 |= DISP_CS4_GP;
    
    DISP_CS4_TYPE &= ~(1<<DISP_CS4_NO);
    DISP_CS4_IN  &= ~(1<<DISP_CS4_NO);
    DISP_CS4_OUT &= ~(0x03<<(2*DISP_CS4_NO));
    DISP_CS4_OUT |= (0x01<<(2*DISP_CS4_NO));
    DISP_CS4_DDR |= (1<<DISP_CS4_NO);
    DISP_CS4_DR = 0;
  #endif
  
  #if defined(DISP_CS5_GP) && defined(DISP_CS5_DR) && defined(DISP_CS5_DDR) && defined(DISP_CS5_TYPE) && defined(DISP_CS5_IN) && defined(DISP_CS5_OUT) && defined(DISP_CS5_NO)
    SCU->PRR1 |= DISP_CS5_GP;
    SCU->PCGR1 |= DISP_CS5_GP;
    
    DISP_CS5_TYPE &= ~(1<<DISP_CS5_NO);
    DISP_CS5_IN  &= ~(1<<DISP_CS5_NO);
    DISP_CS5_OUT &= ~(0x03<<(2*DISP_CS5_NO));
    DISP_CS5_OUT |= (0x01<<(2*DISP_CS5_NO));
    DISP_CS5_DDR |= (1<<DISP_CS5_NO);
    DISP_CS5_DR = 0;
  #endif
  
  #if defined(DISP_CS6_GP) && defined(DISP_CS6_DR) && defined(DISP_CS6_DDR) && defined(DISP_CS6_TYPE) && defined(DISP_CS6_IN) && defined(DISP_CS6_OUT) && defined(DISP_CS6_NO)
    SCU->PRR1 |= DISP_CS6_GP;
    SCU->PCGR1 |= DISP_CS6_GP;
    
    DISP_CS6_TYPE &= ~(1<<DISP_CS6_NO);
    DISP_CS6_IN  &= ~(1<<DISP_CS6_NO);
    DISP_CS6_OUT &= ~(0x03<<(2*DISP_CS6_NO));
    DISP_CS6_OUT |= (0x01<<(2*DISP_CS6_NO));
    DISP_CS6_DDR |= (1<<DISP_CS6_NO);
    DISP_CS6_DR = 0;
  #endif
  
  #if defined(DISP_CS7_GP) && defined(DISP_CS7_DR) && defined(DISP_CS7_DDR) && defined(DISP_CS7_TYPE) && defined(DISP_CS7_IN) && defined(DISP_CS7_OUT) && defined(DISP_CS7_NO)
    SCU->PRR1 |= DISP_CS7_GP;
    SCU->PCGR1 |= DISP_CS7_GP;
    
    DISP_CS7_TYPE &= ~(1<<DISP_CS7_NO);
    DISP_CS7_IN  &= ~(1<<DISP_CS7_NO);
    DISP_CS7_OUT &= ~(0x03<<(2*DISP_CS7_NO));
    DISP_CS7_OUT |= (0x01<<(2*DISP_CS7_NO));
    DISP_CS7_DDR |= (1<<DISP_CS7_NO);
    DISP_CS7_DR = 0;
  #endif
  
  #if defined(DISP_CS8_GP) && defined(DISP_CS8_DR) && defined(DISP_CS8_DDR) && defined(DISP_CS8_TYPE) && defined(DISP_CS8_IN) && defined(DISP_CS8_OUT) && defined(DISP_CS8_NO)
    SCU->PRR1 |= DISP_CS8_GP;
    SCU->PCGR1 |= DISP_CS8_GP;
    
    DISP_CS8_TYPE &= ~(1<<DISP_CS8_NO);
    DISP_CS8_IN  &= ~(1<<DISP_CS8_NO);
    DISP_CS8_OUT &= ~(0x03<<(2*DISP_CS8_NO));
    DISP_CS8_OUT |= (0x01<<(2*DISP_CS8_NO));
    DISP_CS8_DDR |= (1<<DISP_CS8_NO);
    DISP_CS8_DR = 0;
  #endif
  
  #if defined(DISP_CLK_GP) && defined(DISP_CLK_DR) && defined(DISP_CLK_DDR) && defined(DISP_CLK_TYPE) && defined(DISP_CLK_IN) && defined(DISP_CLK_OUT) && defined(DISP_CLK_NO)
    SCU->PRR1 |= DISP_CLK_GP;
    SCU->PCGR1 |= DISP_CLK_GP;
    
    DISP_CLK_TYPE &= ~(1<<DISP_CLK_NO);
    DISP_CLK_IN  &= ~(1<<DISP_CLK_NO);
    DISP_CLK_OUT &= ~(0x03<<(2*DISP_CLK_NO));
    DISP_CLK_OUT |= (0x01<<(2*DISP_CLK_NO));
    DISP_CLK_DDR |= (1<<DISP_CLK_NO);
    DISP_CLK_DR = 0;
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
  
    if (dir == IN)
      DISP_DB_DDR &= ~(0xFF);
    else
      DISP_DB_DDR |=  (0xFF);
      
    DISP_DB_DATA = 0;
    
  #elif DISP_DB_WIDTH == 4 && DISP_DB_MIX == 0
    
    if (dir == IN)
      DISP_DB_DDR &= ~(0x0F<<DISP_DB_OFFSET);
    else
      DISP_DB_DDR |=  (0x0F<<DISP_DB_OFFSET);
    
    DISP_DB_DATA = 0;
    
  #elif DISP_DB_WIDTH == 4 && DISP_DB_MIX == 1
  
    if (dir == IN) {
      DISP_DB4_DDR  &=  ~(1<<DISP_DB4_NO);
      DISP_DB5_DDR  &=  ~(1<<DISP_DB5_NO);
      DISP_DB6_DDR  &=  ~(1<<DISP_DB6_NO);
      DISP_DB7_DDR  &=  ~(1<<DISP_DB7_NO);
    } else {
      DISP_DB4_DDR  |=  (1<<DISP_DB4_NO);
      DISP_DB5_DDR  |=  (1<<DISP_DB5_NO);
      DISP_DB6_DDR  |=  (1<<DISP_DB6_NO);
      DISP_DB7_DDR  |=  (1<<DISP_DB7_NO);
    }
      
    DISP_DB4_DR = 0;
    DISP_DB5_DR = 0;
    DISP_DB6_DR = 0;
    DISP_DB7_DR = 0;
    
  #elif DISP_DB_WIDTH == 8 && DISP_DB_MIX == 1
    
    if (dir == IN) {
      DISP_DB0_DDR  &=  ~(1<<DISP_DB0_NO);
      DISP_DB1_DDR  &=  ~(1<<DISP_DB1_NO);
      DISP_DB2_DDR  &=  ~(1<<DISP_DB2_NO);
      DISP_DB3_DDR  &=  ~(1<<DISP_DB3_NO);
      DISP_DB4_DDR  &=  ~(1<<DISP_DB4_NO);
      DISP_DB5_DDR  &=  ~(1<<DISP_DB5_NO);
      DISP_DB6_DDR  &=  ~(1<<DISP_DB6_NO);
      DISP_DB7_DDR  &=  ~(1<<DISP_DB7_NO);
    } else {
      DISP_DB0_DDR  |=  (1<<DISP_DB0_NO);
      DISP_DB1_DDR  |=  (1<<DISP_DB1_NO);
      DISP_DB2_DDR  |=  (1<<DISP_DB2_NO);
      DISP_DB3_DDR  |=  (1<<DISP_DB3_NO);
      DISP_DB4_DDR  |=  (1<<DISP_DB4_NO);
      DISP_DB5_DDR  |=  (1<<DISP_DB5_NO);
      DISP_DB6_DDR  |=  (1<<DISP_DB6_NO);
      DISP_DB7_DDR  |=  (1<<DISP_DB7_NO);
    }
    
    DISP_DB0_DR = 0;
    DISP_DB1_DR = 0;
    DISP_DB2_DR = 0;
    DISP_DB3_DR = 0;
    DISP_DB4_DR = 0;
    DISP_DB5_DR = 0;
    DISP_DB6_DR = 0;
    DISP_DB7_DR = 0;
    
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

    DISP_DB_DATA = data;
    
  #elif DISP_DB_WIDTH == 4 && DISP_DB_MIX == 0

    DISP_DB_DATA = (data<<DISP_DB_OFFSET);
    
  #elif DISP_DB_WIDTH == 4 && DISP_DB_MIX == 1
  
    DISP_DB4_DR = 0;
    DISP_DB5_DR = 0;
    DISP_DB6_DR = 0;
    DISP_DB7_DR = 0;
    
    if (data & 0x01)
      DISP_DB4_DR = (1<<DISP_DB4_NO);
    if (data & 0x02)
      DISP_DB5_DR = (1<<DISP_DB5_NO);
    if (data & 0x04)
      DISP_DB6_DR = (1<<DISP_DB6_NO);
    if (data & 0x08)
      DISP_DB7_DR = (1<<DISP_DB7_NO);
    
  #elif DISP_DB_WIDTH == 8 && DISP_DB_MIX == 1
    
    DISP_DB0_DR = 0;
    DISP_DB1_DR = 0;
    DISP_DB2_DR = 0;
    DISP_DB3_DR = 0;
    DISP_DB4_DR = 0;
    DISP_DB5_DR = 0;
    DISP_DB6_DR = 0;
    DISP_DB7_DR = 0;
    
    if (data & 0x01)
      DISP_DB0_DR = (1<<DISP_DB0_NO);
    if (data & 0x02)
      DISP_DB1_DR = (1<<DISP_DB1_NO);
    if (data & 0x04)
      DISP_DB2_DR = (1<<DISP_DB2_NO);
    if (data & 0x08)
      DISP_DB3_DR = (1<<DISP_DB3_NO);
    if (data & 0x10)
      DISP_DB4_DR = (1<<DISP_DB4_NO);
    if (data & 0x20)
      DISP_DB5_DR = (1<<DISP_DB5_NO);
    if (data & 0x40)
      DISP_DB6_DR = (1<<DISP_DB6_NO);
    if (data & 0x80)
      DISP_DB7_DR = (1<<DISP_DB7_NO);
    
  #elif defined(DISP_MOSI_GP) && defined(DISP_MOSI_DR) && defined(DISP_MOSI_DDR) && defined(DISP_MOSI_TYPE) && defined(DISP_MOSI_IN) && defined(DISP_MOSI_OUT) && defined(DISP_MOSI_NO) \
    && (DISP_DB_WIDTH == 1 || DISP_DB_WIDTH == 2) 

    DISP_MOSI_DR = (data<<DISP_MOSI_NO);
    
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

    return (uint8_t)(DISP_DB_DATA);
    
  #elif DISP_DB_WIDTH == 4 && DISP_DB_MIX == 0

    return (uint8_t)((DISP_DB_DATA>>DISP_DB_OFFSET) & 0x0F);
    
  #elif DISP_DB_WIDTH == 4 && DISP_DB_MIX == 1
  
    uint8_t data = 0;
    
    if (DISP_DB4_DR)
      data |= 0x01;
    if (DISP_DB5_DR)
      data |= 0x02;
    if (DISP_DB6_DR)
      data |= 0x04;
    if (DISP_DB7_DR)
      data |= 0x08;
      
    return data;
    
  #elif DISP_DB_WIDTH == 8 && DISP_DB_MIX == 1
    
    uint8_t data = 0;
    
    if (DISP_DB0_DR)
      data |= 0x01;
    if (DISP_DB1_DR)
      data |= 0x02;
    if (DISP_DB2_DR)
      data |= 0x04;
    if (DISP_DB3_DR)
      data |= 0x08;
    if (DISP_DB4_DR)
      data |= 0x10;
    if (DISP_DB5_DR)
      data |= 0x20;
    if (DISP_DB6_DR)
      data |= 0x40;
    if (DISP_DB7_DR)
      data |= 0x80;
      
    return data;
    
  #elif defined(DISP_MISO_GP) && defined(DISP_MISO_DR) && defined(DISP_MISO_DDR) && defined(DISP_MISO_TYPE) && defined(DISP_MISO_IN) && defined(DISP_MISO_OUT) && defined(DISP_MISO_NO) \
    && DISP_DB_WIDTH == 2

    if (DISP_MISO_DR)
      return 1;
    else
      return 0;
    
  #else
  
    return 0;
    
  #endif
}
