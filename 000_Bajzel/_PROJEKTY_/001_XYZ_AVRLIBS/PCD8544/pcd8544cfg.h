/*
 * ATMEGA32L_LCD pcd8544cfg.h
 *
 *  Stworzono: 2014 02-10-2014
 *      Autor: D¹browski Tomasz
 *     Edycja: 22:45:24
 */

#ifndef PCD8544CFG_H_
#define PCD8544CFG_H_

/* Parametry biblioteki */
#define PCD8544_GFX 1

/*  Konfiguracja pinów PCD8544 - nokia3310/5110 LCD  */
#define PCD8544_SCE B, 4
#define PCD8544_RES B, 6
#define PCD8544_DC B, 0
#define PCD8544_SDIN B, 5
#define PCD8544_SCLK B, 7

/* Parametry wyœwietlacza */
#define PCD8544_W 84
#define PCD8544_H 48

#endif /* PCD8544CFG_H_ */
