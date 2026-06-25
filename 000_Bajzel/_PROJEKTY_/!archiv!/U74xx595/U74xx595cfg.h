/*
 * 00_XYZ_AVRLIBS U74xx595cfg.h
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 2016-09-19   
 *     Edycja: 10:50:36
 */

#ifndef U74XX595_U74XX595CFG_H_
#define U74XX595_U74XX595CFG_H_
#include <avr/io.h>
#include "../ADDS/pins.h"

//static uint8_t BITORDER[8]={7,6,5,4,3,2,1,0};
static uint8_t BITORDER[8]={0,1,2,3,4,5,6,7};
//struktura do przechowywania wielu rejestrów
typedef struct
{
//	uint8_t WIDTH;//il.kolumn pikseli/szerokoœæ
//	uint8_t HEIGHT;//il.linii pikseli/wysokoœæ
    uint8_t COE;//linia sygna³owa Enable/OE
} TU74xx595;

/* Parametry biblioteki */

/* Parametry wyœwietlacza */
static TU74xx595  U74xx595_OE={G2O(PC, 0)};

/*  Konfiguracja pinów T6963   */
#define U74xx595_DS G2O(PC, 1)
#define U74xx595_SH G2O(PB, 1)
#define U74xx595_ST G2O(PB, 2)

#endif /* U74XX595_U74XX595CFG_H_ */
