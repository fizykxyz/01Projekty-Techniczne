/*
 * main.c    ATmeg\AVRa8    F_CPU = 8000000 Hz
 *
 * Created on: 2018-03-21
 *     Author: admin
*/
// do³¹czanie systemowych plików nag³ówkowych
#include <avr/io.h>
#include <util/delay.h>

// miejsce na do³¹czanie w³asnych plików
#include "e:-Programowanie\C\_PROJEKTY_\00_XYZ_AVRLIBS\ADDS\tools.h"

// g³ówna funkcja programu - main()
int main( void ) {
   int a=NIBBLE2HEX(4);
// sekcja inicjalizacji peryferiów

// g³ówna pêtla programu
    while ( 1 ) {
        a++;
    }

}