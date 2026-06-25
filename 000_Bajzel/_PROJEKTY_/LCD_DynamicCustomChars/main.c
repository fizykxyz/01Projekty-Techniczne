
/////////////////////////////////////////////////////////////////////////////////////////////
// main.c - testowanie mo¿liwoœci zmiany jêzyka
//
// Autor: Rados³aw Koppel   Kompilator: WinAVR 20060125
/////////////////////////////////////////////////////////////////////////////////////////////
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "makra.h"
#include "harddef.h"
//#include "makra.h"
#include "lcd.h"
#include "langsys.h"

void uint8_t sw_wait(void)
{
   for(;;)
   {
      // Oczekiwanie na naciœniêcie przycisku
      if(!(PIN(SW_PORT) & 1<<SW1))
      {
         _delay_ms(30);
         if(!(PIN(SW_PORT) & 1<<SW1))
            return 1;
      }
      if(!(PIN(SW_PORT) & 1<<SW2))
      {
         _delay_ms(30);
         if(!(PIN(SW_PORT) & 1<<SW2))
            return 2;
      }
   }
}

int main(void)
{
   // Inicjacja wyprowadzeñ
   DDR(LCD_CTRLPORT) = (1<<LCD_E | 1<<LCD_RW | 1<<LCD_RS | 1<<LCD_LED);
   PORT(LCD_CTRLPORT) = ~(1<<LCD_E | 1<<LCD_LED);
   PORT(SW_PORT) = 1<<SW1 | 1<<SW2;
   // Inicjacja wyœwietlacza
   lcd_Init();
   lcd_SetStatus(LCD_STATUS_DISP);

   // Wyœwietlenie zapytania o jêzyk
   fputs_P(PSTR("S1 - "), lcd_GetFile());
   fputs_P(langsys_GetLangName(0), lcd_GetFile());
   lcd_GoTo(0, 1);
   fputs_P(PSTR("S2 - "), lcd_GetFile());
   fputs_P(langsys_GetLangName(1), lcd_GetFile());
   lcd_Update();

   // Oczekiwanie na przycisk i wybranie odpowiedniego jêzyka
   langsys_Select(sw_wait()-1);

   // Wyœwietlenie przywitania w wybranym jêzyku
   lcd_Cls();
   fputs_P(langsys_GetText(IDS_Start), lcd_GetFile());
   lcd_Update();

   return 0;
}

