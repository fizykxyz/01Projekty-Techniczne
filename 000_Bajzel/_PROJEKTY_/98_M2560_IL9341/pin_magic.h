#ifndef _pin_magic_
#define _pin_magic_

#include "tft_lcd.h"

// Pixel read operations require a minimum 400 nS delay from RD_ACTIVE
// to polling the input pins.  At 16 MHz, one machine cycle is 62.5 nS.
// This code burns 7 cycles (437.5 nS) doing nothing; the RJMPs are
// equivalent to two NOPs each, final NOP burns the 7th cycle, and the
// last line is a radioactive mutant emoticon.
#define DELAY7        \
  asm volatile(       \
    "rjmp .+0" "\n\t" \
    "rjmp .+0" "\n\t" \
    "rjmp .+0" "\n\t" \
    "nop"      "\n"   \
    ::);

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined (__AVR_ATmega328__) || defined(__AVR_ATmega8__)

#ifdef USE_ADAFRUIT_SHIELD_PINOUT

#else // Uno w/Breakout board

#define write8inline(d) {                          \
    PORTD = (PORTD & 0b00000011) | ((d) & 0b11111100); \
    PORTB = (PORTB & 0b11111100) | ((d) & 0b00000011); \
    WR_STROBE; }
#define read8inline(result) {                       \
    RD_ACTIVE;                                        \
    DELAY7;                                           \
    result = (PIND & 0b11111100) | (PINB & 0b00000011); \
    RD_IDLE; }
#define setWriteDirInline() { DDRD |=  0b11111100; DDRB |=  0b00000011; }
#define setReadDirInline()  { DDRD &= ~0b11111100; DDRB &= ~0b00000011; }

#endif

// As part of the inline control, macros reference other macros...if any
// of these are left undefined, an equivalent function version (non-inline)
// is declared later.  The Uno has a moderate amount of program space, so
// only write8() is inlined -- that one provides the most performance
// benefit, but unfortunately also generates the most bloat.  This is
// why only certain cases are inlined for each board.
#define write8 write8inline

#elif defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__) 

#ifdef USE_ADAFRUIT_SHIELD_PINOUT

#define write8inline(d) {                                              \
    PORTH = (PORTH&B10000111)|(((d)&B11000000)>>3)|(((d)&B00000011)<<5); \
    PORTB = (PORTB&B01001111)|(((d)&B00101100)<<2);                      \
    PORTG = (PORTG&B11011111)|(((d)&B00010000)<<1);                      \
    WR_STROBE; }
#define read8inline(result) {                                      \
    RD_ACTIVE;                                                       \
    DELAY7;                                                          \
    result = ((PINH & B00011000) << 3) | ((PINB & B10110000) >> 2) | \
             ((PING & B00100000) >> 1) | ((PINH & B01100000) >> 5);  \
    RD_IDLE; }
#define setWriteDirInline() {                                   \
    DDRH |=  B01111000; DDRB |=  B10110000; DDRG |=  B00100000; }
#define setReadDirInline()  {                                   \
    DDRH &= ~B01111000; DDRB &= ~B10110000; DDRG &= ~B00100000; }

#else // Mega w/Breakout board

#define write8inline(d)   { PORTA = (d); WR_STROBE; }
#define read8inline(result) { \
    RD_ACTIVE;                  \
    DELAY7;                     \
    result = PINA;              \
    RD_IDLE; }

#endif

#elif defined(__AVR_ATmega32U4__)

#ifdef USE_ADAFRUIT_SHIELD_PINOUT


#else // Leonardo w/Breakout board


#endif

#elif defined(__SAM3X8E__)

#ifdef USE_ADAFRUIT_SHIELD_PINOUT


#define read8inline(result) { \
RD_ACTIVE;
delayMicroseconds(1);
result = (((PIOC->PIO_PDSR & (1<<23)) >> (23-7)) | ((PIOC->PIO_PDSR & (1<<24)) >> (24-6)) |
		((PIOB->PIO_PDSR & (1<<27)) >> (27-5)) | ((PIOC->PIO_PDSR & (1<<26)) >> (26-4)) |
		((PIOD->PIO_PDSR & (1<< 7)) >> ( 7-3)) | ((PIOC->PIO_PDSR & (1<<29)) >> (29-2)) |
		((PIOC->PIO_PDSR & (1<<21)) >> (21-1)) | ((PIOC->PIO_PDSR & (1<<22)) >> (22-0)));
RD_IDLE;}

#define setWriteDirInline() { \
   PIOD->PIO_MDDR |=  0x00000080; /*PIOD->PIO_SODR =  0x00000080;*/ PIOD->PIO_OER |=  0x00000080; PIOD->PIO_PER |=  0x00000080; \
   PIOC->PIO_MDDR |=  0x25E00000; /*PIOC->PIO_SODR =  0x25E00000;*/ PIOC->PIO_OER |=  0x25E00000; PIOC->PIO_PER |=  0x25E00000; \
   PIOB->PIO_MDDR |=  0x08000000; /*PIOB->PIO_SODR =  0x08000000;*/ PIOB->PIO_OER |=  0x08000000; PIOB->PIO_PER |=  0x08000000; }

#define setReadDirInline() { \
	  pmc_enable_periph_clk( ID_PIOD ) ;	  pmc_enable_periph_clk( ID_PIOC ) ;	  pmc_enable_periph_clk( ID_PIOB ) ; \
   PIOD->PIO_PUDR |=  0x00000080; PIOD->PIO_IFDR |=  0x00000080; PIOD->PIO_ODR |=  0x00000080; PIOD->PIO_PER |=  0x00000080; \
   PIOC->PIO_PUDR |=  0x25E00000; PIOC->PIO_IFDR |=  0x25E00000; PIOC->PIO_ODR |=  0x25E00000; PIOC->PIO_PER |=  0x25E00000; \
   PIOB->PIO_PUDR |=  0x08000000; PIOB->PIO_IFDR |=  0x08000000; PIOB->PIO_ODR |=  0x08000000; PIOB->PIO_PER |=  0x08000000; }

#else // Due w/Breakout board

#define setWriteDirInline() { \
	    PIOC->PIO_MDDR |=  0x000001FE; /*PIOC->PIO_SODR |=  0x000001FE;*/ PIOC->PIO_OER |=  0x000001FE; PIOC->PIO_PER |=  0x000001FE; }

#define setReadDirInline() { \
		pmc_enable_periph_clk( ID_PIOC ) ; \
		PIOC->PIO_PUDR |=  0x000001FE; PIOC->PIO_IFDR |=  0x000001FE; PIOC->PIO_ODR |=  0x000001FE; PIOC->PIO_PER |=  0x000001FE; }
#endif
#endif

#if !defined(__SAM3X8E__)
// Stuff common to all Arduino AVR board types:

#ifdef USE_ADAFRUIT_SHIELD_PINOUT

#else // Breakout board

 // When using the TFT breakout board, control pins are configurable.
#define RD_ACTIVE  LCD_PORT &= ~(1 << LCD_RD_PIN);// *rdPort &=  rdPinUnset
#define RD_IDLE    LCD_PORT |= (1 << LCD_RD_PIN);// *rdPort |=  rdPinSet
#define WR_ACTIVE  LCD_PORT &= ~(1 << LCD_WR_PIN);// *wrPort &=  wrPinUnset
#define WR_IDLE    LCD_PORT |= (1 << LCD_WR_PIN);// *wrPort |=  wrPinSet
#define CD_COMMAND LCD_PORT &= ~(1 << LCD_CD_PIN);// *cdPort &=  cdPinUnset
#define CD_DATA    LCD_PORT |= (1 << LCD_CD_PIN);// *cdPort |=  cdPinSet
#define CS_ACTIVE  LCD_PORT &= ~(1 << LCD_CS_PIN);// *csPort &=  csPinUnset
#define CS_IDLE    LCD_PORT |= (1 << LCD_CS_PIN);// *csPort |=  csPinSet

#endif
#endif

// Data write strobe, ~2 instructions and always inline
#define WR_STROBE { WR_ACTIVE; WR_IDLE; }

// These higher-level operations are usually functionalized,
// except on Mega where's there's gobs and gobs of program space.

// Set value of TFT register: 8-bit address, 8-bit value
#define writeRegister8inline(a, d) { \
  CD_COMMAND; write8(a); CD_DATA; write8(d); }

// Set value of TFT register: 16-bit address, 16-bit value
// See notes at top about macro expansion, hence hi & lo temp vars
#define writeRegister16inline(a, d) { \
  uint8_t hi, lo; \
  hi = (a) >> 8; lo = (a); CD_COMMAND; write8(hi); write8(lo); \
  hi = (d) >> 8; lo = (d); CD_DATA   ; write8(hi); write8(lo); }

// Set value of 2 TFT registers: Two 8-bit addresses (hi & lo), 16-bit value
#define writeRegisterPairInline(aH, aL, d) { \
  uint8_t hi = (d) >> 8, lo = (d); \
  CD_COMMAND; write8(aH); CD_DATA; write8(hi); \
  CD_COMMAND; write8(aL); CD_DATA; write8(lo); }

#endif // _pin_magic_
