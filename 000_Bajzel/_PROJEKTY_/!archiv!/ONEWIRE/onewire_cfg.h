/*
 * 11_M32_MULTILCD onewire_cfg.h
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 2015-11-01
 *     Edycja: 16:31:42
 */

#ifndef ONEWIRE_CFG_H_
#define ONEWIRE_CFG_H_

//do³¹czane pliki
#include <avr/io.h>

#include "../ADDS/pins.h"


//typedef struct
//{
//  uint8_t OW_PIN;//linia sygna³owa magistrali One Wire
//} TOWIRE;

// Use AVR's internal pull-up resistor instead of external 4,7k resistor.
// Based on information from Sascha Schade. Experimental but worked in tests
// with one DS18B20 and one DS18S20 on a rather short bus (60cm), where both
// sensores have been parasite-powered.
#define OW_INT_PULLUP 0  /* Rezystor podci¹gaj¹cy liniê  0=zewn najl. 4k7, 1=wewnêtrzny*/


#ifndef OW_PIN
#define OW_PIN G2O(PA, 1)
#endif

#endif /* ONEWIRE_CFG_H_ */
