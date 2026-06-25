/*
 * 00_XYZ_AVRLIBS U74xx595.h
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 2016-09-19   
 *     Edycja: 11:57:51
 */

#ifndef U74XX595_U74XX595_H_
#define U74XX595_U74XX595_H_

#include "../ADDS/pins.h"
#include "../ADDS/tools.h"
#include "U74xx595cfg.h"

void U74xx595_NR(uint8_t NR, TU74xx595 *STRUCT);
void U74xx595_INIT();
void U74xx595_START(TU74xx595 *STRUCT,uint8_t SIZE);
void U74xx595_W8BIT (uint8_t OCTET);



#endif /* U74XX595_U74XX595_H_ */
