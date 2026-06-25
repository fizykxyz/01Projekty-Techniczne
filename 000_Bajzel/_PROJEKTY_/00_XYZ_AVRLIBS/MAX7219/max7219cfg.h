/*
 * 00_XYZ_AVRLIBS max7219cfg.h
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 2016-09-26   
 *     Edycja: 11:51:30
 */

#ifndef MAX7219_MAX7219CFG_H_
#define MAX7219_MAX7219CFG_H_

#include <avr/io.h>
#include "../ADDS/pins.h"

//static uint8_t BITORDER[8]={7,6,5,4,3,2,1,0};
//static uint8_t BITORDER[8]={0,1,2,3,4,5,6,7};
//struktura do przechowywania wielu rejestrów
typedef struct
{
//	uint8_t WIDTH;//il.kolumn pikseli/szerokoœæ
//	uint8_t HEIGHT;//il.linii pikseli/wysokoœæ
    uint8_t CSET;//linia sygna³owa ISET
} TMAX7219;

/* Parametry biblioteki */

/* Parametry wyœwietlacza */
static TMAX7219 TMAX7219_ISET={G2O(PB, 7)};

/*  Konfiguracja pinów T6963   */
#define MAX7219_DIN G2O(PA, 0)
#define MAX7219_CLK G2O(PA, 2)
#define MAX7219_LOAD G2O(PA, 1)

#endif /* MAX7219_MAX7219CFG_H_ */
