/*
 * ATMEGA32L_LCD pcd8544cfg.h
 *
 *  Stworzono: 2014-10-02
 *      Autor: D¹browski Tomasz
 *     Edycja: 22:45:24
 */

#ifndef PCD8544CFG_H_
#define PCD8544CFG_H_

#include <avr/io.h>
#include "../ADDS/pins.h"

//struktura do przechowywania wielu ekranów
typedef struct
{
	uint8_t WIDTH;//il.kolumn pikseli/szerokoœæ
	uint8_t HEIGHT;//il.linii pikseli/wysokoœæ
    uint8_t SCEN;//linia sygna³owa Enable/SCE
} TPCD8544;

/* Parametry biblioteki */
#define PCD8544_GFX 1

/* Parametry wyœwietlacza */
//static TPCD8544  PCD8544_SCE={84,48,G2O(PB, 4)};

/*  Konfiguracja pinów PCD8544 - nokia3310/5110 LCD  */
#define PCD8544_DC G2O(PB, 0)
//#define PCD8544_SCE G2O(PB, 4) //do usuniecia
#define PCD8544_SDIN G2O(PB, 5)
#define PCD8544_RES G2O(PB, 6)
#define PCD8544_SCLK G2O(PB, 7)

#endif /* PCD8544CFG_H_ */
