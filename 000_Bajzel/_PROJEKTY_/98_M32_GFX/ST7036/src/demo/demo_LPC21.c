/****************************************************************************
* Biblioteka obslugi wyswietlacza alfanumerycznego, opartego na sterowniku
* ST7036 lub odpowiedniku.
* 
* Program demonstracyjny dla platformy ARM7 LPC21XX.
*
* Nazwa pliku: demo_LPC21.c
* Data: 16.10.2007
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

#include <ST7036.h>
#include <disp_conf.h>
#include <LPC21XX.h> 

char text[] = "Demo ST7036";

void wait_ms(int ms) {
  while(ms--)
    for (uint16_t i=0; i<(uint16_t)(8700.0*F_CPU/60000000.0); i++)
      asm volatile("nop\n\t"::);
}

int main(void) {
  Disp_init();
  Disp_puts(text);
  Disp_update();
  
  uint8_t i=0;
  
  while(1) {
    
    if ((i++) & 0x10) 
      Disp_set_font(16, 0);
    else
      Disp_set_font(8, 0);
    
    Disp_move_txt_window(RIGHT);
    wait_ms(250);
    Disp_update();
  }
  return 0;
}
