/*
 * 09_ATMEGA163L LEDS_EXT.h
 *
 *  Stworzono: 2015 27 cze 2015
 *      Autor: D¹browski Tomasz
 *     Edycja: 20:33:49
 */

#ifndef ADDS_LEDS_EXT_H_
#define ADDS_LEDS_EXT_H_

#define LEDM01A C,4
#define LEDM01K B,1
#define LEDM01(STAN) if (STAN==0) HIGH(LEDM01A); else {LOW(LEDM01A);LOW(LEDM01K);}

#define LEDM02A A,2
#define LEDM02K B,1
#define LEDM02(STAN) if (STAN==0) HIGH(LEDM02A); else {LOW(LEDM02A);LOW(LEDM02K);}

#define LEDM03A C,2
#define LEDM03K B,1
#define LEDM03(STAN) if (STAN==0) HIGH(LEDM03A); else {LOW(LEDM03A);LOW(LEDM03K);}

#define LEDM04A C,3
#define LEDM04K B,1
#define LEDM04(STAN) if (STAN==0) HIGH(LEDM04A); else {LOW(LEDM04A);LOW(LEDM04K);}

#define LEDM05A C,7
#define LEDM05K B,1
#define LEDM05(STAN) if (STAN==0) HIGH(LEDM05A); else {LOW(LEDM05A);LOW(LEDM05K);}

#define LEDM06A D,6
#define LEDM06K B,2
#define LEDM06(STAN) if (STAN==0) HIGH(LEDM06A); else {LOW(LEDM06A);LOW(LEDM06K);}

#define LEDM07A D,4
#define LEDM07K B,2
#define LEDM07(STAN) if (STAN==0) HIGH(LEDM07A); else {LOW(LEDM07A);LOW(LEDM07K);}

#define LEDM08A C,6
#define LEDM08K B,2
#define LEDM08(STAN) if (STAN==0) HIGH(LEDM08A); else {LOW(LEDM08A);LOW(LEDM08K);}


#endif /* ADDS_LEDS_EXT_H_ */
