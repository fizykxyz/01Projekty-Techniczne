/****************************************************************************
* Biblioteka obslugi wyswietlacza alfanumerycznego, opartego na sterowniku
* HD44780 lub odpowiedniku.
* 
* Program demonstracyjny dla platformy AVR.
*
* Nazwa pliku: demo_AVR.c
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

#include <avr/io.h>
#include <util/delay.h>
#include <HD44780.h>
#include <disp_conf.h>
#include <avr/pgmspace.h>

char text[] = "Demo HD44780       Demo HD44780";

void wait_ms(int ms) {
  while(ms--)
    _delay_loop_2((uint16_t)(65535/(262.14/(F_CPU/1000000UL))));
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
