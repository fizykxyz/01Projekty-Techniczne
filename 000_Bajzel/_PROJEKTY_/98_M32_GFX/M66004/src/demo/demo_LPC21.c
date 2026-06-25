/****************************************************************************
* Biblioteka obslugi wyswietlacza alfanumerycznego, opartego na sterowniku
* M66004 (VFD) lub odpowiedniku.
* 
* Program demonstracyjny dla platformy ARM7 LPC21XX.
*
* Nazwa pliku: demo_LPC21.c
* Data: 21.08.2007
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

#include <M66004.h>
#include <disp_conf.h>
#include <LPC21XX.h> 

char text[] = "Demo M66004";

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
    for (uint8_t i=0; i<8; i++) {
      Disp_set_contrast(i);
      Disp_update();
      wait_ms(100);
    }
    for (uint8_t i=7; i!=0xFF; i--) {
      Disp_set_contrast(i);
      Disp_update();
      wait_ms(100);
    }
  }
  
  return 0;
}
