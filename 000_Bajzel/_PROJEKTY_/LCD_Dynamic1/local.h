/////////////////////////////////////////////////////////////////////////////////////////////
// local.h - plik nag³ówkowy do wszystkich danych które musz¹ zostaæ zlokalizowane
//		teksty oraz znaki narodowe (specjalne)
// Do³¹cz ten plik wszêdzie tam gdzie chcesz korzystaæ ze zmiennych zawartych w local.c
//
// Autor: Rados³aw Koppel	Kompilator: WinAVR 20060125
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef LOCAL_H_INCLUDED
#define LOCAL_H_INCLUDED
#include <avr/pgmspace.h>
extern const LCD_LOCAL_PGM local_lcdspec[];
//extern char PROGMEM str_Hello[];
extern const char PROGMEM str_all[];

#endif // LOCAL_H_INCLUDED
