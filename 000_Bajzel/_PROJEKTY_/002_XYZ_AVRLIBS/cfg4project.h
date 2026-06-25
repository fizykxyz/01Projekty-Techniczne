/*
 * 002_XYZ_AVRLIBS cfg4project.h
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 2020-09-22   
 *     Edycja: 12:12:38
 */

#ifndef CFG4PROJECT_H_INCLUDED
#define CFG4PROJECT_H_INCLUDED

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <stdio.h>

#include "ADDS/pins.h"
#include "ADDS/tools.h"


//#define HD44780_D4 G2O(PC, 2)
//#define HD44780_D5 G2O(PC, 3)
//#define HD44780_D6 G2O(PC, 4)
//#define HD44780_D7 G2O(PC, 5)

#include "HD44780/hd44780.h"


//#include "PCD8544/pcd8544.h"
//#include "DS18xxx/ds18xxx.h"

#endif /* CFG4PROJECT_H_INCLUDED */
