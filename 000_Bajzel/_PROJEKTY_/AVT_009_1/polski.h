/////////////////////////////////////////////////////////////////////////////////////////////
// polski.h - dane dla jÍzyka polskiego.
//
// Uwaga: Ten plik definiuje zmienne. Wolno go do≥πczyÊ tylko raz i robimy do w langsys.c
//
// Autor: Rados≥aw Koppel   Kompilator: WinAVR 20060125
/////////////////////////////////////////////////////////////////////////////////////////////

// Definicja znakÛw narodowych
// (Polski ma ich 18)
const LCD_LOCAL_PGM PL_lcdspec[18] PROGMEM =
{
   // Znaczki - poprawi≥em pod wzglÍdem wyglπdu znaczki z EdW 1/98, doda≥em brakujπce
   {{0, 0, 14, 1, 15, 17, 15, 2}, 'a'},    //π - kod 0x80
   {{2, 4, 14, 16, 16, 17, 14, 0}, 'c'},   //Ê - kod 0x81
   {{0, 0, 14, 17, 31, 16, 14, 2}, 'e'},   //Í - kod 0x82
   {{12, 4, 6, 4, 12, 4, 14, 0}, 'l'},     //≥ - kod 0x83
   {{2, 4, 22, 25, 17, 17, 17, 0}, 'n'},   //Ò - kod 0x84
   {{2, 4, 14, 17, 17, 17, 14, 0}, 'o'},   //Û - kod 0x85
   {{2, 4, 14, 16, 14, 1, 30, 0}, 's'},    //ú - kod 0x86
   {{2, 4, 31, 2, 4, 8, 31, 0}, 'z'},      //ü - kod 0x87
   {{4, 0, 31, 2, 4, 8, 31, 0}, 'z'},      //ø - kod 0x88
   {{14, 17, 17, 31, 17, 17, 17, 2}, 'A'}, //• - kod 0x89
   {{2, 14, 21, 16, 16, 16, 17, 14}, 'C'}, //∆ - kod 0x8a
   {{31, 16, 16, 30, 16, 16, 31, 2}, 'E'}, //  - kod 0x8b
   {{16, 16, 20, 24, 16, 16, 31, 0}, 'L'}, //£ - kod 0x8c
   {{2, 21, 17, 25, 21, 19, 17, 17}, 'N'}, //— - kod 0x8d
   {{2, 14, 21, 17, 17, 17, 17, 14}, 'O'}, //” - kod 0x8e
   {{2, 15, 20, 16, 14, 1, 1, 30}, 'S'},   //å - kod 0x8f
   {{2, 31, 5, 2, 4, 8, 16, 31}, 'Z'},     //è - kod 0x90
   {{4, 31, 1, 2, 4, 8, 16, 31}, 'Z'},     //Ø - kod 0x91
};

// Teksty
const char PROGMEM PL_strDisplay[] = "Polski";
const char PROGMEM PL_strStart[] = " Witaj w wersji     Polskiej\x80";
