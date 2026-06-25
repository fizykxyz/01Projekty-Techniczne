/*
 * ATMEGA32L_LCD pcd8544.h
 *
 *  Stworzono: 2014 02-10-2014
 *      Autor: D¹browski Tomasz
 *     Edycja: 22:54:27
 */

#ifndef PCD8544_H_
#define PCD8544_H_

//pliki za³¹czone

#include <stdlib.h>
#include "../ADDS/tools.h"
#include "pcd8544cfg.h"



//definicje efektów
#define under 128
#define lead2 64
#define lead1 32
#define lead3 lead1|lead2
#define stroke 16
#define bold 8
#define whole 4
#define trail2 2
#define trail1 1
#define trail3 trail1|trail2

#define normal 0 //wspólna dla efektów i trybu ekranu oraz pixeli

#define invert 1
#define blank 2
#define allon 3
#define alpha 4

#define getrot 0
#define rot0 1
#define rot90 2
#define rot180 3
#define rot270 4

//rozmiary w MODE czcionek
#define size1x 0
#define size2x 8
#define size3x 16
#define size4x size2x|size3x

#define PCD8544_COM 0 //komendy
#define PCD8544_DAT 1 //dane

//typ pamiêci
#define RAM 0
#define FLASH 1


#define swap(x,y) {int8_t z=x;x=y;y=z;}

void _PCD8544_8BIT(uint8_t DC, uint8_t OCTET);
extern void PCD8544_INIT(void);
void PCD8544_CONTRAST(uint8_t CONTRAST);
void PCD8544_MODE(uint8_t MODE);
void PCD8544_CLR(void);
void PCD8544_XY8(uint8_t PX, uint8_t PY8);
void PCD8544_HOME(void);
void PCD8544_CHAR(uint8_t CHAR, uint8_t EFFECT);
void PCD8544_TEXT(char * TEXT, uint8_t EFFECT);

#if PCD8544_GFX == 1
void PCD8544_GFXCLR(uint8_t IMG[],uint8_t MODE,int8_t WIDTH,int8_t HEIGHT8);
void PCD8544_GFX2LCD(uint8_t MEMTYPE,uint8_t IMG[],int8_t POSX,int8_t POSY8,int8_t WIDTH,int8_t HEIGHT8);
uint8_t PCD8544_GFXROT(uint8_t ROT); //odczyt = 0 > ustawia 1-0st,2-90st,3-180st,4-270st
void PCD8544_GFXPIXEL(uint8_t IMG[],uint8_t MODE, int8_t POSX,int8_t POSY,int8_t WIDTH,int8_t HEIGHT8);
void PCD8544_GFXLINE(uint8_t IMG[],uint8_t MODE,int8_t POSX1, int8_t POSY1, int8_t POSX2, int8_t POSY2,int8_t WIDTH,int8_t HEIGHT8);
void PCD8544_GFXBOX(uint8_t IMG[],uint8_t MODE,int8_t POSX1, int8_t POSY1, int8_t POSX2, int8_t POSY2,int8_t WIDTH,int8_t HEIGHT8);
void PCD8544_GFXBAR(uint8_t IMG[],uint8_t MODE,int8_t POSX1, int8_t POSY1, int8_t POSX2, int8_t POSY2,int8_t WIDTH,int8_t HEIGHT8);
void PCD8544_GFXCIRCLE(uint8_t IMG[],uint8_t MODE,int8_t POSX, int8_t POSY, int8_t RADIUS,int8_t WIDTH,int8_t HEIGHT8);
void PCD8544_GFXCHAR(uint8_t IMG[],uint8_t MODE,uint8_t FONT[],int8_t POSX, int8_t POSY,int8_t WIDTH,int8_t HEIGHT8,uint8_t CHAR, uint8_t EFFECT);
void PCD8544_GFXTEXT(uint8_t IMG[],uint8_t MODE,uint8_t FONT[],int8_t POSX, int8_t POSY,int8_t WIDTH,int8_t HEIGHT8,char * TEXT, uint8_t EFFECT);

#endif


#endif /* PCD8544_H_ */
