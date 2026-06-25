/*
 * virtualOLED.h
 *
 *      Author: artur85
 */

#ifndef VIRTUALOLED_H_
#define VIRTUALOLED_H_

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

void vOLED_Init();
void vOLED_Pixel(int x, int y, uint8_t bw);
void vOLED_cls();
void vOLED_display();
void vOLED_bitmap(int x, int y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color);
#endif /* VIRTUALOLED_H_ */

