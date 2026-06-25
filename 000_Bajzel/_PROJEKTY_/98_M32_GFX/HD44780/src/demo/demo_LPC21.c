/****************************************************************************
* Biblioteka obslugi wyswietlacza alfanumerycznego, opartego na sterowniku
* HD44780 lub odpowiedniku.
* 
* Program demonstracyjny dla platformy ARM7 LPC21XX.
*
* Nazwa pliku: demo_LPC21.c
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

#include <HD44780.h>
#include <disp_conf.h>
#include <LPC21XX.h> 

char text[] = "Demo HD44780       Demo HD44780";

void wait_ms(int ms) {
  while(ms--)
    for (uint16_t i=0; i<(uint16_t)(8700.0*F_CPU/60000000.0); i++)
      asm volatile("nop\n\t"::);
}

int main(void) {
  Disp_init();
  Disp_puts(text);
  Disp_update();
  
  while(1) {
    Disp_move_txt_window(RIGHT);
    wait_ms(150);
    Disp_update();
  }
  return 0;
}
