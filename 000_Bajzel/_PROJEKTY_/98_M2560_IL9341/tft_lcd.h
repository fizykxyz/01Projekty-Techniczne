// IMPORTANT: SEE COMMENTS @ LINE 15 REGARDING SHIELD VS BREAKOUT BOARD USAGE.

// Graphics library by ladyada/adafruit with init code from Rossum
// MIT license

#ifndef TFT_LCD_H
#define TFT_LCD_H

#include "gfx.h"

#define USE_LCD_RESET 1


#define LCD_DDR		DDRC
#define LCD_PORT	PORTC

#define LCD_RESET_DDR	DDRD
#define LCD_RESET_PORT	PORTD

#define LCD_RESET_LOW (LCD_RESET_PORT &= ~(1 << LCD_RESET_PIN))
#define LCD_RESET_HIGH (LCD_RESET_PORT |= (1 << LCD_RESET_PIN))


// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS_PIN PC3//A3 // Chip Select goes to Analog 3
#define LCD_CD_PIN PC2 // Command/Data goes to Analog 2
#define LCD_WR_PIN PC1 // LCD Write goes to Analog 1
#define LCD_RD_PIN PC0 // LCD Read goes to Analog 0

#define LCD_RESET_PIN PD0//A4 // Can alternately just connect to Arduino's reset pin

//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7

#define LCD_WIDTH	320
#define LCD_HEIGHT	240

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// **** IF USING THE LCD BREAKOUT BOARD, COMMENT OUT THIS NEXT LINE. ****
// **** IF USING THE LCD SHIELD, LEAVE THE LINE ENABLED:             ****

//#define USE_ADAFRUIT_SHIELD_PINOUT 1

#define read8(x) x=read8fn()

void tft_init(void);//(uint16_t id);
void tft_drawPixel(int16_t x, int16_t y, uint16_t color);
void tft_drawFastHLine(int16_t x0, int16_t y0, int16_t w, uint16_t color);
void tft_drawFastVLine(int16_t x0, int16_t y0, int16_t h, uint16_t color);
void tft_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t c);
void tft_fillScreen(uint16_t color);
void tft_reset(void);
void setRegisters8(uint8_t *ptr, uint8_t n);
void setRegisters16(uint16_t *ptr, uint8_t n);
void tft_setRotation(uint8_t x);
// These methods are public in order for BMP examples to work:
void setAddrWindow(int x1, int y1, int x2, int y2);
void pushColors(uint16_t *data, uint8_t len, uint8_t first);

uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
uint16_t readPixel(int16_t x, int16_t y);
uint16_t tft_readID(void);
uint32_t readReg(uint8_t r);
void write8(uint8_t value);

void setWriteDir(void);

void setReadDir(void);

void writeRegister8(uint8_t a, uint8_t d);

void writeRegister16(uint16_t a, uint16_t d);

void writeRegister24(uint8_t a, uint32_t d);
void writeRegister32(uint8_t a, uint32_t d);
void writeRegisterPair(uint8_t aH, uint8_t aL, uint16_t d);

void setLR(void);
void flood(uint16_t color, uint32_t len);

uint8_t driver;

uint8_t read8fn(void);

// For compatibility with sketches written for older versions of library.
// Color function name was changed to 'color565' for parity with 2.2" LCD
// library.
#define Color565 color565

#endif
