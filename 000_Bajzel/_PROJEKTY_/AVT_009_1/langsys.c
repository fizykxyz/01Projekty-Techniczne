/////////////////////////////////////////////////////////////////////////////////////////////
// langsys.c - modu³ zarz¹dzaj¹cy jêzykiem interfejsu
//
// Autor: Rados³aw Koppel   Kompilator: WinAVR 20060125
/////////////////////////////////////////////////////////////////////////////////////////////
#include <inttypes.h>
#include <avr/pgmspace.h>

#include "makra.h"
#include "lcd.h" // Potrzebujemy st¹d zmiennej opisuj¹cej wygl¹d znaku specjalnego
#include "langsys.h"
// Tutaj dodaæ wszystkie nag³ówki jêzyków
#include "english.h"
#include "polski.h"

//______________________________________________________________________________________________
// Zmienne
static uint8_t langsys_sel = 0; // wybrany jêzyk

// Definicja informacji o znakach specjalnych
const char * const PROGMEM LCD_LOCAL_PGM langsys_lcdspec[]; /*w EdW brak PROGMEM - to b³¹d!*/
{
   NULL,
   PL_lcdspec
};

// Tablica wszystkich posiadanych napisów
//   [jêzyk][tekst] - na odwrót nie trzebaby pilnowaæ rozmiaru 
//   (pierwszy wymiar mo¿e byæ obliczany przez kompilator), jednka inicjacja by³a by mniej wygodna
//      +---tablica wskaŸników na tablice w pamiêci programu
//      |                                        +--umieœæ t¹ tablicê w pamiêci programu
const char PROGMEM langsys_strTable[2][IDS_END]  =
{
   {
      EN_strDisplay,
      EN_strStart
   },
   {
      PL_strDisplay,
      PL_strStart
   }
};

//______________________________________________________________________________________________
// Funkcje dostêpu

// Iloœæ "zainstalowanych" jêzyków
uint8_t langsys_GetNumOfLangs(void)
{
   return ELEMS(langsys_strTable);
}

// Wybór jêzyka
void langsys_Select(uint8_t index)
{
   langsys_sel = index;
}

// Pobranie informacji o wybranym jêzyku
uint8_t langsys_GetSelected(void)
{
   return langsys_sel;
}

// Pobranie nazwy jêzyka o podanym indeksie
const char * const PROGMEM langsys_GetLangName(uint8_t index)
{
   if(index > ELEMS(langsys_strTable))
      return NULL;
   return (const char * const PROGMEM)pgm_read_word_near(&langsys_strTable[index][IDS_LANGNAME]);
}

// Pobieranie wskaŸnika na informacjê o znakach specjalnych
LCD_LOCAL_PGM* langsys_GetSpec(void)
{
   return (LCD_LOCAL_PGM*)pgm_read_word_near(&langsys_lcdspec[langsys_sel]);
}

// Pobieranie wybranego napisu
const char * const PROGMEM langsys_GetText(uint8_t index)
{
   return (const char * const PROGMEM)pgm_read_word_near(&langsys_strTable[langsys_sel][index]);
}
