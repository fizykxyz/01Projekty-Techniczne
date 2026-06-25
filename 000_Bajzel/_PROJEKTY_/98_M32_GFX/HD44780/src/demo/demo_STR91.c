/****************************************************************************
* Biblioteka obslugi wyswietlacza alfanumerycznego, opartego na sterowniku
* HD44780 lub odpowiedniku.
* 
* Program demonstracyjny dla platformy ARM9 STR91XX.
*
* Nazwa pliku: demo_STR91.c
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

#include <HD44780.h>
#include <disp_conf.h>
#include <91x_map.h>
#include <91x_scu.h>

char text[] = "Demo HD44780       Demo HD44780";

void wait_ms(int ms) {
  while(ms--)
    for (uint8_t i=0; i<(50.0*F_CPU/25000000.0); i++)
      for (uint8_t j=0; j<35; j++)
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
