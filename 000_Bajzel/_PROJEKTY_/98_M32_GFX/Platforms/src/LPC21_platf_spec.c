/****************************************************************************
* Biblioteka obslugi wyswietlacza.
* 
* Plik funkcji specyficznych dla platformy ARM7 LPC21xx.
*
* Nazwa pliku: LPC21_platf_spec.c
* Data: 15.08.2007
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
#include <LPC21XX.h>
#include <inttypes.h>
#include <disp_conf.h>

/*
 * Deklaracja zmiennych prywatnych.
 */
static uint16_t prev_CPSR;

/*
 * Blokuje wykonywanie przerwan.
 */
__inline__ void disable_interrupts(void) {
  uint16_t temp;
  asm volatile ("mrs %0, cpsr":"=r" (temp):);
  prev_CPSR = temp;
  temp |= INT_MSK;
  asm volatile ("msr cpsr, %0"::"r" (temp));
}

/*
 * Odblokowuje wykonywanie przerwan, jesli przed wykonaniem funkcji
 * disable_interrupts() byly one odblokowane.
 */
__inline__ void revert_interrupts(void) {
  uint16_t temp = prev_CPSR;
  asm volatile ("msr cpsr, %0"::"r" (temp));
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
      for (uint16_t i=0; i<(uint16_t)(8700.0*F_CPU/60000000.0); i++)
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
  #if defined(DISP_DB_SET) && defined(DISP_DB_CLR) && defined(DISP_DB_PIN) && defined(DISP_DB_DDR) \
      && DISP_DB_WIDTH == 8 && DISP_DB_MIX == 0 && defined(DISP_DB_OFFSET)
    
    DISP_DB_DDR |= (0xFF<<DISP_DB_OFFSET);
    DISP_DB_CLR  = (0xFF<<DISP_DB_OFFSET);
    
  #elif  defined(DISP_DB_SET) && defined(DISP_DB_CLR) && defined(DISP_DB_PIN) && defined(DISP_DB_DDR) \
      && DISP_DB_WIDTH == 4 && DISP_DB_MIX == 0 && defined(DISP_DB_OFFSET)
    
    DISP_DB_DDR |= (0x0F<<DISP_DB_OFFSET);
    DISP_DB_CLR  = (0x0F<<DISP_DB_OFFSET);
    
  #elif  defined(DISP_DB4_SET) && defined(DISP_DB4_CLR) && defined(DISP_DB4_PIN) && defined(DISP_DB4_DDR) && defined(DISP_DB4_NO) \
      && defined(DISP_DB5_SET) && defined(DISP_DB5_CLR) && defined(DISP_DB5_PIN) && defined(DISP_DB5_DDR) && defined(DISP_DB5_NO) \
      && defined(DISP_DB6_SET) && defined(DISP_DB6_CLR) && defined(DISP_DB6_PIN) && defined(DISP_DB6_DDR) && defined(DISP_DB6_NO) \
      && defined(DISP_DB7_SET) && defined(DISP_DB7_CLR) && defined(DISP_DB7_PIN) && defined(DISP_DB7_DDR) && defined(DISP_DB7_NO) \
      && DISP_DB_WIDTH == 4 && DISP_DB_MIX == 1
    
    DISP_DB4_DDR |= (1<<DISP_DB4_NO);
    DISP_DB4_CLR  = (1<<DISP_DB4_NO);
    DISP_DB5_DDR |= (1<<DISP_DB5_NO);
    DISP_DB5_CLR  = (1<<DISP_DB5_NO);
    DISP_DB6_DDR |= (1<<DISP_DB6_NO);
    DISP_DB6_CLR  = (1<<DISP_DB6_NO);
    DISP_DB7_DDR |= (1<<DISP_DB7_NO);
    DISP_DB7_CLR  = (1<<DISP_DB7_NO);
    
  #elif  defined(DISP_DB0_SET) && defined(DISP_DB0_CLR) && defined(DISP_DB0_PIN) && defined(DISP_DB0_DDR) && defined(DISP_DB0_NO) \
      && defined(DISP_DB1_SET) && defined(DISP_DB1_CLR) && defined(DISP_DB1_PIN) && defined(DISP_DB1_DDR) && defined(DISP_DB1_NO) \
      && defined(DISP_DB2_SET) && defined(DISP_DB2_CLR) && defined(DISP_DB2_PIN) && defined(DISP_DB2_DDR) && defined(DISP_DB2_NO) \
      && defined(DISP_DB3_SET) && defined(DISP_DB3_CLR) && defined(DISP_DB3_PIN) && defined(DISP_DB3_DDR) && defined(DISP_DB3_NO) \
      && defined(DISP_DB4_SET) && defined(DISP_DB4_CLR) && defined(DISP_DB4_PIN) && defined(DISP_DB4_DDR) && defined(DISP_DB4_NO) \
      && defined(DISP_DB5_SET) && defined(DISP_DB5_CLR) && defined(DISP_DB5_PIN) && defined(DISP_DB5_DDR) && defined(DISP_DB5_NO) \
      && defined(DISP_DB6_SET) && defined(DISP_DB6_CLR) && defined(DISP_DB6_PIN) && defined(DISP_DB6_DDR) && defined(DISP_DB6_NO) \
      && defined(DISP_DB7_SET) && defined(DISP_DB7_CLR) && defined(DISP_DB7_PIN) && defined(DISP_DB7_DDR) && defined(DISP_DB7_NO) \
      && DISP_DB_WIDTH == 8 && DISP_DB_MIX == 1
    
    DISP_DB0_DDR |= (1<<DISP_DB0_NO);
    DISP_DB0_CLR  = (1<<DISP_DB0_NO);
    DISP_DB1_DDR |= (1<<DISP_DB1_NO);
    DISP_DB1_CLR  = (1<<DISP_DB1_NO);
    DISP_DB2_DDR |= (1<<DISP_DB2_NO);
    DISP_DB2_CLR  = (1<<DISP_DB2_NO);
    DISP_DB3_DDR |= (1<<DISP_DB3_NO);
    DISP_DB3_CLR  = (1<<DISP_DB3_NO);
    DISP_DB4_DDR |= (1<<DISP_DB4_NO);
    DISP_DB4_CLR  = (1<<DISP_DB4_NO);
    DISP_DB5_DDR |= (1<<DISP_DB5_NO);
    DISP_DB5_CLR  = (1<<DISP_DB5_NO);
    DISP_DB6_DDR |= (1<<DISP_DB6_NO);
    DISP_DB6_CLR  = (1<<DISP_DB6_NO);
    DISP_DB7_DDR |= (1<<DISP_DB7_NO);
    DISP_DB7_CLR  = (1<<DISP_DB7_NO);
    
  #elif defined(DISP_MOSI_SET) && defined(DISP_MOSI_CLR) && defined(DISP_MOSI_DDR) && defined(DISP_MOSI_NO) \
      && defined(DISP_MISO_SET) && defined(DISP_MISO_CLR) && defined(DISP_MISO_PIN) && defined(DISP_MISO_DDR) && defined(DISP_MISO_NO) \
      && DISP_DB_WIDTH == 2
    
    DISP_MOSI_DDR |= (1<<DISP_MOSI_NO);
    DISP_MOSI_CLR  = (1<<DISP_MOSI_NO);
    
    DISP_MISO_DDR &= ~(1<<DISP_MISO_NO);
    DISP_MISO_CLR  = (1<<DISP_MISO_NO);
    
  #elif defined(DISP_MOSI_SET) && defined(DISP_MOSI_CLR) && defined(DISP_MOSI_DDR) && defined(DISP_MOSI_NO) \
      && DISP_DB_WIDTH == 1
      
    DISP_MOSI_DDR |= (1<<DISP_MOSI_NO);
    DISP_MOSI_CLR  = (1<<DISP_MOSI_NO);
    
  #else
  #  error "Ustawienia magistrali wyswietlacza (stale z prefiksem DISP_DB) sa niepoprawne."
  #endif
  
  // Inicjalizacja portow linii specjalnych
  #if defined(DISP_E1_SET) && defined(DISP_E1_CLR) && defined(DISP_E1_DDR) && defined(DISP_E1_NO)
    DISP_E1_DDR |= (1<<DISP_E1_NO);
    DISP_E1_CLR  = (1<<DISP_E1_NO);
  #endif
  
  #if defined(DISP_E2_SET) && defined(DISP_E2_CLR) && defined(DISP_E2_DDR) && defined(DISP_E2_NO)
    DISP_E2_DDR |= (1<<DISP_E2_NO);
    DISP_E2_CLR  = (1<<DISP_E2_NO);
  #endif
  
  #if defined(DISP_E3_SET) && defined(DISP_E3_CLR) && defined(DISP_E3_DDR) && defined(DISP_E3_NO)
    DISP_E3_DDR |= (1<<DISP_E3_NO);
    DISP_E3_CLR  = (1<<DISP_E3_NO);
  #endif
  
  #if defined(DISP_E4_SET) && defined(DISP_E4_CLR) && defined(DISP_E4_DDR) && defined(DISP_E4_NO)
    DISP_E4_DDR |= (1<<DISP_E4_NO);
    DISP_E4_CLR  = (1<<DISP_E4_NO);
  #endif
  
  #if defined(DISP_E5_SET) && defined(DISP_E5_CLR) && defined(DISP_E5_DDR) && defined(DISP_E5_NO)
    DISP_E5_DDR |= (1<<DISP_E5_NO);
    DISP_E5_CLR  = (1<<DISP_E5_NO);
  #endif
  
  #if defined(DISP_E6_SET) && defined(DISP_E6_CLR) && defined(DISP_E6_DDR) && defined(DISP_E6_NO)
    DISP_E6_DDR |= (1<<DISP_E6_NO);
    DISP_E6_CLR  = (1<<DISP_E6_NO);
  #endif
  
  #if defined(DISP_E7_SET) && defined(DISP_E7_CLR) && defined(DISP_E7_DDR) && defined(DISP_E7_NO)
    DISP_E7_DDR |= (1<<DISP_E7_NO);
    DISP_E7_CLR  = (1<<DISP_E7_NO);
  #endif
  
  #if defined(DISP_E8_SET) && defined(DISP_E8_CLR) && defined(DISP_E8_DDR) && defined(DISP_E8_NO)
    DISP_E8_DDR |= (1<<DISP_E8_NO);
    DISP_E8_CLR  = (1<<DISP_E8_NO);
  #endif
  
  #if defined(DISP_RS_SET) && defined(DISP_RS_CLR) && defined(DISP_RS_DDR) && defined(DISP_RS_NO)
    DISP_RS_DDR |= (1<<DISP_RS_NO);
    DISP_RS_CLR  = (1<<DISP_RS_NO);
  #endif
  
  #if defined(DISP_RW_SET) && defined(DISP_RW_CLR) && defined(DISP_RW_DDR) && defined(DISP_RW_NO)
    DISP_RW_DDR |= (1<<DISP_RW_NO);
    DISP_RW_CLR  = (1<<DISP_RW_NO);
  #endif
  
  #if defined(DISP_CD_SET) && defined(DISP_CD_CLR) && defined(DISP_CD_DDR) && defined(DISP_CD_NO)
    DISP_CD_DDR |= (1<<DISP_CD_NO);
    DISP_CD_CLR  = (1<<DISP_CD_NO);
  #endif
  
  #if defined(DISP_CE_SET) && defined(DISP_CE_CLR) && defined(DISP_CE_DDR) && defined(DISP_CE_NO)
    DISP_CE_DDR |= (1<<DISP_CE_NO);
    DISP_CE_CLR  = (1<<DISP_CE_NO);
  #endif
  
  #if defined(DISP_RD_SET) && defined(DISP_RD_CLR) && defined(DISP_RD_DDR) && defined(DISP_RD_NO)
    DISP_RD_DDR |= (1<<DISP_RD_NO);
    DISP_RD_CLR  = (1<<DISP_RD_NO);
  #endif
  
  #if defined(DISP_WR_SET) && defined(DISP_WR_CLR) && defined(DISP_WR_DDR) && defined(DISP_WR_NO)
    DISP_WR_DDR |= (1<<DISP_WR_NO);
    DISP_WR_CLR  = (1<<DISP_WR_NO);
  #endif
  
  #if defined(DISP_RES_SET) && defined(DISP_RES_CLR) && defined(DISP_RES_DDR) && defined(DISP_RES_NO)
    DISP_RES_DDR |= (1<<DISP_RES_NO);
    DISP_RES_CLR  = (1<<DISP_RES_NO);
  #endif
  
  #if defined(DISP_CS1_SET) && defined(DISP_CS1_CLR) && defined(DISP_CS1_DDR) && defined(DISP_CS1_NO)
    DISP_CS1_DDR |= (1<<DISP_CS1_NO);
    DISP_CS1_CLR  = (1<<DISP_CS1_NO);
  #endif
  
  #if defined(DISP_CS2_SET) && defined(DISP_CS2_CLR) && defined(DISP_CS2_DDR) && defined(DISP_CS2_NO)
    DISP_CS2_DDR |= (1<<DISP_CS2_NO);
    DISP_CS2_CLR  = (1<<DISP_CS2_NO);
  #endif
  
  #if defined(DISP_CS3_SET) && defined(DISP_CS3_CLR) && defined(DISP_CS3_DDR) && defined(DISP_CS3_NO)
    DISP_CS3_DDR |= (1<<DISP_CS3_NO);
    DISP_CS3_CLR  = (1<<DISP_CS3_NO);
  #endif
  
  #if defined(DISP_CS4_SET) && defined(DISP_CS4_CLR) && defined(DISP_CS4_DDR) && defined(DISP_CS4_NO)
    DISP_CS4_DDR |= (1<<DISP_CS4_NO);
    DISP_CS4_CLR  = (1<<DISP_CS4_NO);
  #endif
  
  #if defined(DISP_CS5_SET) && defined(DISP_CS5_CLR) && defined(DISP_CS5_DDR) && defined(DISP_CS5_NO)
    DISP_CS5_DDR |= (1<<DISP_CS5_NO);
    DISP_CS5_CLR  = (1<<DISP_CS5_NO);
  #endif
  
  #if defined(DISP_CS6_SET) && defined(DISP_CS6_CLR) && defined(DISP_CS6_DDR) && defined(DISP_CS6_NO)
    DISP_CS6_DDR |= (1<<DISP_CS6_NO);
    DISP_CS6_CLR  = (1<<DISP_CS6_NO);
  #endif
  
  #if defined(DISP_CS7_SET) && defined(DISP_CS7_CLR) && defined(DISP_CS7_DDR) && defined(DISP_CS7_NO)
    DISP_CS7_DDR |= (1<<DISP_CS7_NO);
    DISP_CS7_CLR  = (1<<DISP_CS7_NO);
  #endif
  
  #if defined(DISP_CS8_SET) && defined(DISP_CS8_CLR) && defined(DISP_CS8_DDR) && defined(DISP_CS8_NO)
    DISP_CS8_DDR |= (1<<DISP_CS8_NO);
    DISP_CS8_CLR  = (1<<DISP_CS8_NO);
  #endif
  
  #if defined(DISP_CLK_SET) && defined(DISP_CLK_CLR) && defined(DISP_CLK_DDR) && defined(DISP_CLK_NO)
    DISP_CLK_DDR |= (1<<DISP_CLK_NO);
    DISP_CLK_CLR  = (1<<DISP_CLK_NO);
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
      DISP_DB_DDR &= ~(0xFF<<DISP_DB_OFFSET);
    else
      DISP_DB_DDR |=  (0xFF<<DISP_DB_OFFSET);
      
    DISP_DB_CLR = (0xFF<<DISP_DB_OFFSET);
    
  #elif DISP_DB_WIDTH == 4 && DISP_DB_MIX == 0
    
    if (dir == IN)
      DISP_DB_DDR &= ~(0x0F<<DISP_DB_OFFSET);
    else
      DISP_DB_DDR |=  (0x0F<<DISP_DB_OFFSET);
    
    DISP_DB_CLR = (0x0F<<DISP_DB_OFFSET);
    
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
      
    DISP_DB4_CLR = (1<<DISP_DB4_NO);
    DISP_DB5_CLR = (1<<DISP_DB5_NO);
    DISP_DB6_CLR = (1<<DISP_DB6_NO);
    DISP_DB7_CLR = (1<<DISP_DB7_NO);
    
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
    
    DISP_DB0_CLR = (1<<DISP_DB0_NO);
    DISP_DB1_CLR = (1<<DISP_DB1_NO);
    DISP_DB2_CLR = (1<<DISP_DB2_NO);
    DISP_DB3_CLR = (1<<DISP_DB3_NO);
    DISP_DB4_CLR = (1<<DISP_DB4_NO);
    DISP_DB5_CLR = (1<<DISP_DB5_NO);
    DISP_DB6_CLR = (1<<DISP_DB6_NO);
    DISP_DB7_CLR = (1<<DISP_DB7_NO);
    
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

    DISP_DB_CLR = (0xFF<<DISP_DB_OFFSET);
    DISP_DB_SET = (data<<DISP_DB_OFFSET);
    
  #elif DISP_DB_WIDTH == 4 && DISP_DB_MIX == 0

    DISP_DB_CLR = (0x0F<<DISP_DB_OFFSET);
    DISP_DB_SET = ((data & 0x0F)<<DISP_DB_OFFSET);
    
  #elif DISP_DB_WIDTH == 4 && DISP_DB_MIX == 1
  
    DISP_DB4_CLR = (1<<DISP_DB4_NO);
    DISP_DB5_CLR = (1<<DISP_DB5_NO);
    DISP_DB6_CLR = (1<<DISP_DB6_NO);
    DISP_DB7_CLR = (1<<DISP_DB7_NO);
    
    if (data & 0x01)
      DISP_DB4_SET = (1<<DISP_DB4_NO);
    if (data & 0x02)
      DISP_DB5_SET = (1<<DISP_DB5_NO);
    if (data & 0x04)
      DISP_DB6_SET = (1<<DISP_DB6_NO);
    if (data & 0x08)
      DISP_DB7_SET = (1<<DISP_DB7_NO);
    
  #elif DISP_DB_WIDTH == 8 && DISP_DB_MIX == 1
    
    DISP_DB0_CLR = (1<<DISP_DB0_NO);
    DISP_DB1_CLR = (1<<DISP_DB1_NO);
    DISP_DB2_CLR = (1<<DISP_DB2_NO);
    DISP_DB3_CLR = (1<<DISP_DB3_NO);
    DISP_DB4_CLR = (1<<DISP_DB4_NO);
    DISP_DB5_CLR = (1<<DISP_DB5_NO);
    DISP_DB6_CLR = (1<<DISP_DB6_NO);
    DISP_DB7_CLR = (1<<DISP_DB7_NO);
    
    if (data & 0x01)
      DISP_DB0_SET = (1<<DISP_DB0_NO);
    if (data & 0x02)
      DISP_DB1_SET = (1<<DISP_DB1_NO);
    if (data & 0x04)
      DISP_DB2_SET = (1<<DISP_DB2_NO);
    if (data & 0x08)
      DISP_DB3_SET = (1<<DISP_DB3_NO);
    if (data & 0x10)
      DISP_DB4_SET = (1<<DISP_DB4_NO);
    if (data & 0x20)
      DISP_DB5_SET = (1<<DISP_DB5_NO);
    if (data & 0x40)
      DISP_DB6_SET = (1<<DISP_DB6_NO);
    if (data & 0x80)
      DISP_DB7_SET = (1<<DISP_DB7_NO);
    
  #elif defined(DISP_MOSI_SET) && defined(DISP_MOSI_CLR) && defined(DISP_MOSI_DDR) && defined(DISP_MOSI_NO) \
    && (DISP_DB_WIDTH == 1 || DISP_DB_WIDTH == 2) 

    DISP_MOSI_CLR = (1<<DISP_MOSI_NO);
    DISP_MOSI_SET = ((data & 0x01)<<DISP_MOSI_NO);
    
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

    return (uint8_t)((DISP_DB_PIN>>DISP_DB_OFFSET) & 0xFF);
    
  #elif DISP_DB_WIDTH == 4 && DISP_DB_MIX == 0

    return (uint8_t)((DISP_DB_PIN>>DISP_DB_OFFSET) & 0x0F);
    
  #elif DISP_DB_WIDTH == 4 && DISP_DB_MIX == 1
  
    uint8_t data = 0;
    
    if (DISP_DB4_PIN & (1<<DISP_DB4_NO))
      data |= 0x01;
    if (DISP_DB5_PIN & (1<<DISP_DB5_NO))
      data |= 0x02;
    if (DISP_DB6_PIN & (1<<DISP_DB6_NO))
      data |= 0x04;
    if (DISP_DB7_PIN & (1<<DISP_DB7_NO))
      data |= 0x08;
      
    return data;
    
  #elif DISP_DB_WIDTH == 8 && DISP_DB_MIX == 1
    
    uint8_t data = 0;
    
    if (DISP_DB0_PIN & (1<<DISP_DB0_NO))
      data |= 0x01;
    if (DISP_DB1_PIN & (1<<DISP_DB1_NO))
      data |= 0x02;
    if (DISP_DB2_PIN & (1<<DISP_DB2_NO))
      data |= 0x04;
    if (DISP_DB3_PIN & (1<<DISP_DB3_NO))
      data |= 0x08;
    if (DISP_DB4_PIN & (1<<DISP_DB4_NO))
      data |= 0x10;
    if (DISP_DB5_PIN & (1<<DISP_DB5_NO))
      data |= 0x20;
    if (DISP_DB6_PIN & (1<<DISP_DB6_NO))
      data |= 0x40;
    if (DISP_DB7_PIN & (1<<DISP_DB7_NO))
      data |= 0x80;
      
    return data;
    
  #elif defined(DISP_MISO_SET) && defined(DISP_MISO_CLR) && defined(DISP_MISO_PIN) && defined(DISP_MISO_DDR) && defined(DISP_MISO_NO) \
    && DISP_DB_WIDTH == 2

    if (DISP_MISO_PIN & (1<<DISP_MISO_NO))
      return 1;
    else
      return 0;
    
  #else
  
    return 0;
    
  #endif
}
