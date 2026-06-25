#ifndef _ADAFRUIT_GFX_H
#define _ADAFRUIT_GFX_H

#include <stdint.h>

#define swap(a, b) { int16_t t = a; a = b; b = t; }

void tft_drawPixel(int16_t x, int16_t y, uint16_t color);
void tft_drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
//void tft_drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
//void tft_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void tft_drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
//void tft_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
//void tft_fillScreen(uint16_t color);
void tft_invertDisplay(uint8_t i);

void tft_drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void tft_drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
		uint16_t color);
void tft_fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void tft_fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
		int16_t delta, uint16_t color);
void tft_drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2,
		int16_t y2, uint16_t color);
void tft_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2,
		int16_t y2, uint16_t color);
void tft_drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius,
		uint16_t color);
void tft_fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius,
		uint16_t color);
void tft_drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w,
		int16_t h, uint16_t color);
/*void tft_drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w,
		int16_t h, uint16_t color, uint16_t bg);*/
void tft_drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w,
		int16_t h, uint16_t color);
void tft_drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color,
		uint16_t bg, uint8_t size);
void tft_String (char * str);
void tft_Int (int data);

void tft_setCursor(int16_t x, int16_t y);
void tft_setTextColor(uint16_t c);
//void tft_setTextColor(uint16_t c, uint16_t bg);
void tft_setTextSize(uint8_t s);
void tft_setTextWrap(uint8_t w);
//void tft_setRotation(uint8_t r);

void tft_write(uint8_t);

int16_t height(void);
int16_t width(void);

uint8_t getRotation(void);

/*

const int16_t
WIDTH, HEIGHT;   // This is the 'raw' display w/h - never changes
int16_t _width, _height, // Display w/h as modified by current rotation
		cursor_x, cursor_y;
uint16_t textcolor, textbgcolor;
uint8_t textsize, rotation;
uint8_t wrap; // If set, 'wrap' text at right edge of display
*/


#endif // _ADAFRUIT_GFX_H
