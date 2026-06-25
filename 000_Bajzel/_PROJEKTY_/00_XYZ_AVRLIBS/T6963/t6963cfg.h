/*
 * 00_XYZ_AVRLIBS t6963cfg.h
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 2016-09-06   
 *     Edycja: 12:42:54
 */

#ifndef T6963_T6963CFG_H_
#define T6963_T6963CFG_H_

#include <avr/io.h>
#include "../ADDS/pins.h"

//struktura do przechowywania wielu ekranów
typedef struct
{
	uint8_t WIDTH;//il.kolumn pikseli/szerokoœæ
	uint8_t HEIGHT;//il.linii pikseli/wysokoœæ
    uint8_t CEN;//linia sygna³owa Enable/CE
} TT6963;

/* Parametry biblioteki */
#define T6963_GFX 1

/* Parametry wyœwietlacza */
static TT6963  T6963_CE={240,128,G2O(PA, 2)};

/*  Konfiguracja pinów T6963   */
#define T6963_WR G2O(PA, 0)
#define T6963_RD G2O(PA, 1)
#define T6963_CD G2O(PA, 3)
#define T6963_RST G2O(PA, 4)
#define T6963_FS G2O(PA, 5)

/* Piny danych 8 bit */
#define T6963_D0 G2O(PD, 0)
#define T6963_D1 G2O(PD, 1)
#define T6963_D2 G2O(PD, 2)
#define T6963_D3 G2O(PD, 3)
#define T6963_D4 G2O(PD, 4)
#define T6963_D5 G2O(PD, 5)
#define T6963_D6 G2O(PD, 6)
#define T6963_D7 G2O(PD, 7)


#endif /* T6963_T6963CFG_H_ */
