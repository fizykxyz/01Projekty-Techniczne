/////////////////////////////////////////////////////////////////////////////////////////////
// langsys.h - plik nag³ówkowy modu³u zarz¹dzaj¹cego danymi jêzyka
//   Tutaj znajduj¹ siê oznaczenia syboliczne wszystkich ³añcuchów
//
// Autor: Rados³aw Koppel   Kompilator: WinAVR 20060125
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef LANGSYS_H_INCLUDED
#define LANGSYS_H_INCLUDED
#include <avr/pgmspace.h>
//__________________________________________________________________________________________
// Funkcje interfejsu
uint8_t langsys_GetNumOfLangs(void);
void langsys_Select(uint8_t index);
uint8_t langsys_GetSelected(void);
const char * const PROGMEM langsys_GetLangName(uint8_t index);
LCD_LOCAL_PGM* langsys_GetSpec(void);
const char * const PROGMEM langsys_GetText(uint8_t index);

//__________________________________________________________________________________________
// Indeksy dla poszczególnych napisów
// Du¿ymi literami specjalne, reszta tak jak odpowiednie zmienne
enum
{
   IDS_LANGNAME,
   IDS_Start,

   // Nie umieszczaæ ¿adnych identyfikatorów za poni¿szym!
   IDS_END
};

#endif //LANGSYS_H_INCLUDED
