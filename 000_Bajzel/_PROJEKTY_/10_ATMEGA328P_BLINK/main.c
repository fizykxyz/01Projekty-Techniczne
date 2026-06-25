                                                        /* B³yskaj¹ce diody II */

#include <avr/io.h>
#include <util/delay.h>

#define LED      PC0
#define LED_DDR  DDRC
#define LED_PORT PORTC

#define DELAYTIME 50

#define setBit(sfr, bit)     (_SFR_BYTE(sfr) |= (1 << bit))
#define clearBit(sfr, bit)   (_SFR_BYTE(sfr) &= ~(1 << bit))
#define toggleBit(sfr, bit)  (_SFR_BYTE(sfr) ^= (1 << bit))

int main(void) {

  // Inicjalizacja
  setBit(LED_DDR, LED);                  /* w³¹czenie wyjœcia na pinie z diod¹ */

  // Pêtla g³ówna
  while (1) {

    setBit(LED_PORT, LED);
    _delay_ms(DELAYTIME);

    clearBit(LED_PORT, LED);
    _delay_ms(DELAYTIME);

  }
  return (0);                                          /* koniec pêtli g³ównej */
}
