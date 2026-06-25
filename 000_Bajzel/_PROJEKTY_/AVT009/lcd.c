/*
 * AVT009 lcd.c
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 2020-04-21   
 *     Edycja: 10:08:53
 */




#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <stdio.h>

#include "makra.h"
#include "harddef.h"
#include "lcd.h"
#include "locale.h"

// Definicje sta³ych komen steruj¹cych
#define LCDC_CLS				0x01
#define LCDC_HOME				0x02
#define LCDC_MODE				0x04
	#define LCDC_MODER				0x02
	#define LCDC_MODEL				0x00
	#define LCDC_MODEMOVE			0x01
#define LCDC_ON					0x08
	#define LCDC_ONDISPLAY			0x04
	#define LCDC_ONCURSOR			0x02
	#define LCDC_ONBLINK			0x01
#define LCDC_SHIFT				0x10
	#define LCDC_SHIFTDISP			0x08
	#define LCDC_SHIFTR				0x04
	#define LCDC_SHIFTL				0x00
#define LCDC_FUNC				0x20
	#define LCDC_FUN8b				0x10
	#define LCDC_FUN4b				0x00
	#define LCDC_FUN2L				0x08
	#define LCDC_FUN1L				0x00
	#define LCDC_FUN5x10			0x04
	#define LCDC_FUN5x7				0x00
#define LCDC_CGA				0x40
#define LCDC_DDA				0x80

//deklaracja funkcji pisz¹cej
static int lcd_put(char c, FILE* f);

//zmienne
static FILE lcd_fLCD_temp =
		FDEV_SETUP_STREAM(lcd_put, NULL,
		_FDEV_SETUP_WRITE);
#define lcd_fLCD (&lcd_fLCD_temp)

static char lcd_buffer[LCD_SX*LCD_SY];
static uint8_t lcd_curpos;
static uint8_t lcd_spec[8];
static uint8_t lcd_status;

//funkcje niskiego poziomu LCD
#define lcd_epulse() {PORT(LCD_CTRLPORT) |= 1<<LCD_E;_delay_us(0.25 ); PORT(LCD_CTRLPORT) &=~(1<<LCD_E);}

// Odbiór danych - funkcja wewn.
static inline uint8_t lcd_Get(void)
{
	uint8_t dana;
	DDR(LCD_DATAPORT) = 0x00; //PORT as INPUT
	PORT(LCD_CTRLPORT) |= 1<<LCD_E; //ACTIVE EN.
	_delay_us(0.5);
	dana = PIN(LCD_CTRLPORT); //GET DATA
	PORT(LCD_CTRLPORT) &=~(1<<LCD_E); //DEAC.EN.
	return dana;
}

uint8_t lcd_GetBF(void)
{
	//Wyprowadzenie do odcytu statusu
	PORT(LCD_CTRLPORT) |= 1<<LCD_RW;
	PORT(LCD_CTRLPORT) &=~(1<<LCD_RS);
	_delay_us(0.25);
	return lcd_Get();
}

// Czeka na wyzerowanie flagi zajêtoœci
void lcd_WaitBF(void)
{
	// Bit zajêtoœci to bit najstarszy
	while (0 !=(0x80 & lcd_GetBF())) {};
}

// Wys³anie danych - funkcja wewn.
static void lcd_Send(uint8_t dana)
{
	DDR(LCD_DATAPORT) = 0xFF; //PORT AS OUTPUT
	PORT(LCD_DATAPORT) = dana; //SEND DATA
	lcd_epulse();
}

// wys³anie danej
void lcd_SendData(uint8_t dana)
{
	lcd_WaitBF();
	PORT(LCD_CTRLPORT) &=~(1<<LCD_RW);
	PORT(LCD_CTRLPORT) |= 1<<LCD_RS;
	_delay_us(0.25);
	lcd_Send(dana);
}

// wys³anie instrukcji steruj¹cej
void lcd_SendInstr(uint8_t dana)
{
	lcd_WaitBF();
	PORT(LCD_CTRLPORT) &=~(1<<LCD_RW | 1<<LCD_RS);
	_delay_us(0.25);
	lcd_Send(dana);
}

static int lcd_put(char c, FILE* f)
{
	//Zabezpieczenie przed przepe³nieniem
	if(lcd_curpos >= ELEMS(lcd_buffer)) lcd_curpos = 0;
	lcd_buffer[lcd_curpos++] = c;
	return 0;
}

void lcd_Init(void)
{
	lcd_Cls();
	PORT(LCD_CTRLPORT) &=~(1<<LCD_RW | 1<<LCD_RS);
	_delay_us(0.25);
	lcd_Send(LCDC_FUNC | LCDC_FUN8b);
	_delay_ms(4.3);
	lcd_Send(LCDC_FUNC | LCDC_FUN8b);
	_delay_us(200);
	lcd_Send(LCDC_FUNC | LCDC_FUN8b);
	// Ju¿ mo¿na sprawdzaæ BF
#if LCD_SY == 1
	lcd_SendInstr(LCDC_FUNC | LCDC_FUN8b | LCDC_FUN1L);
#else
	lcd_SendInstr(LCDC_FUNC | LCDC_FUN8b | LCDC_FUN2L);
#endif
	lcd_SendInstr(LCDC_ON);
	lcd_SendInstr(LCDC_CLS);
	lcd_SendInstr(LCDC_MODE | LCDC_MODER);
	lcd_WaitBF();
}

void lcd_Cls(void)
{
	lcd_curpos = 0;
	memset(lcd_buffer, ' ', sizeof(lcd_buffer));
}

void lcd_SetStatus(uint8_t status)
{
	lcd_status = status;
}

inline void lcd_UpdateStatus(void)
{
	lcd_SendInstr(LCDC_ON | lcd_status);
}

static uint8_t lcd_LineStart(uint8_t line)
{
#if LCD_SY == 1
	return LCD_LINE1;
#elif LCD_SY == 2
	if (line == 0) return LCD_LINE1;
	else return LCD_LINE2;
#elif LCD_SY == 4
	if (line = 0) return LCD_LINE1;
	else if (line == 1) return LCD_LINE2;
	else if (line == 2) return LCD_LINE3;
	else return LCD_LINE4;
#else
#error Unknown display type!
#endif
}

inline void lcd_GoToAdr(uint8_t adr)
{
	lcd_curpos = adr;
}

void lcd_UpdateCurPos(void)
{
	uint8_t adres=
		lcd_LineStart(lcd_curpos / LCD_SX) +
		(lcd_curpos % LCD_SX);
	lcd_SendInstr(LCDC_DDA | adres);
}

static uint8_t lcd_IsSpec(char c)
{
	return (c >= 0x80) && (c <= 0x9F);
}

static inline uint8_t lcd_Spec2Index(char c)
{
	return c-0x80;
}

static uint8_t lcd_GetSpec(uint8_t s_index)
{
	uint8_t a;
	for (a=0; a<ELEMS(lcd_spec); a++)
	{
		if(lcd_spec[a] == 0xFF) break; //0xFF as END
		else if(lcd_spec[a] = s_index) return a;
	}
	return pgm_read_byte(&(local_lcdspec[s_index].cAlt));
}

#define LCD_SPECNF(spec) (spec > 7)

uint8_t lcd_PrepareSpec(void)
{
	char *pbuffer = lcd_buffer;
	uint8_t n;
	uint8_t cnt=0;
	char znak;
	// zerowanie tablicy znaków
	memset(lcd_spec, 0xFF, sizeof(lcd_spec));
	//dodawanie wpisów
	for(n=0; n<ELEMS(lcd_buffer); n++)
	{
		znak = *pbuffer++;
		// Jeœli znaleziony znak specjalny
		if(lcd_IsSpec(znak))
		{
			//Przeszukanie tablicy
			znak = lcd_Spec2Index(znak);
			//Jeœli nie znaleziono - dodawanie
			if(LCD_SPECNF(lcd_GetSpec(znak)))
			{

				// dodaj do tablicy tylko jeœli
				// nie wyst¹pi³o przepe³nienie
				if(cnt < ELEMS(lcd_spec)) lcd_spec[cnt] = znak;
				// licznik zwi¹kszany zawsze
				// dla statystyki
				++cnt;
			}
		}
	}
	return cnt;
}

void lcd_Update(void)
{
	lcd_PrepareSpec();
	lcd_UpdateCGRAM();
	lcd_SendInstr(LCDC_ON |
		(lcd_status &
		LCD_STATUS_DISP));
	lcd_UpdateText();
	lcd_UpdateCurPos();
	lcd_UpdateStatus();
}

inline FILE* lcd_GetFile(void)
{
	return lcd_fLCD;
}

void lcd_UpdateCGRAM(void)
{
	//Ustawiamy adres w mod.na pocz. definicji znaków
	lcd_SendInstr(LCDC_CGA);
	uint8_t a;
	for(a=0; a<ELEMS(lcd_spec); a++)
	{
		if(lcd_spec[a] == 0xFF) break; //0xFF as END of DATA
		// WskaŸnik na pocz.danych wygl¹du znaku
		uint8_t *pdata = local_lcdspec[lcd_spec[a]].matrix;
		// Wysy³ane kolejne bajty/indeksy
		uint8_t bait;
		for(bait=0; bait<8; bait++)
		{
			lcd_SendData(pgm_read_byte(pdata++));
		}
	}
}

void lcd_UpdateText(void)
{
	uint8_t x, y;
	char *pbuffer = lcd_buffer;
	// calculate LINE by LINE
	for(y=0; y<LCD_SY; y++)
	{
		lcd_SendInstr(LCDC_DDA | lcd_LineStart(y));
		// Send all chars in this line
		for (x=0; x<LCD_SX; x++)
		{
			uint8_t znak = *pbuffer++;
			if(lcd_IsSpec(znak))
			{
				znak = lcd_GetSpec(lcd_Spec2Index(znak));
			}
			lcd_SendData(znak);
		}
	}
}

