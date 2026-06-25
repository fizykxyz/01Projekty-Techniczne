/*
 * 01_ARRAY_PINS main.c
 *
 *	    Autor: D¹browski Tomasz
 *  Stworzono: 2016-08-01
 *     Edycja: 08:28:46
 */
#include "cfg4project.h"
#define T_INTRO 5000 // czas pomiêdzy ekranami intro
#define INTRO 1


//************************** PROGRAM W£AŒCIWY - WYKORZYSTANIE ******************

THD44780 wyswT[] =
{
//			{16,2,G2O(PA,7)},
//			{16,2,G2O(PA,6)},
//			{16,4,G2O(PA,5)},
//			{20,4,G2O(PA,4)},
		{ 16, 2, G2O(PA, 7) } };

/*
 uint8_t znak[]  =
 {
 0b00100,
 0b01010,
 0b01010,
 0b01110,
 0b01110,
 0b11111,
 0b11111,
 0b01110
 };

 */

uint8_t lcd, x, y;

void movecursor(uint8_t LEN)
{
	y++;
	if (y >= wyswT[lcd].LIN)
	{
		y = 0;
		if (((LEN + LEN - 1) + x) < (wyswT[lcd].KOL))
		{
			x += LEN;
		}
		else
		{
			lcd++;
			if (lcd >= SIZE(wyswT))
				lcd = 0;
			HD44780_NR(lcd, wyswT);
			//HD44780_CUSTOMCHAR(0,znak);
			x = 0;
		}

	}
	HD44780_XY(x, y);
}

int main(void)  //rozpoczynamy
{

	HD44780_START(wyswT, SIZE(wyswT));
	HD44780_NR(0, wyswT);
	//HD44780_CUSTOMCHARSET(HD44780_CHARSPLW);

	//PCD8544_START(wyswG,SIZE(wyswG));
	//PCD8544_CLR();
//	PCD8544_CONTRAST(0xB1);
//	PCD8544_MODE(invert);

#if INTRO == 1
	HD44780_XY(2,0);
	HD44780_TEXT("uC-");
	HD44780_TEXT(strupr(STRING(MCU))); //HD44780_CHAR('0'+strlen(STRING(MCU)));
	HD44780_XY(2,1);
	HD44780_TEXT("@ ");
	HD44780_TEXT(STRING(F_CPU));
	HD44780_TEXT("Hz");
	_delay_ms(T_INTRO);
	HD44780_XY(0,2);
	HD44780_TEXT("RAM:");
	itoa(RAMEND-RAMSTART,buf,10);
	HD44780_TEXT(buf);
	HD44780_TEXT(" bytes");
	HD44780_XY(0,3);
	HD44780_CHAR('0'+SIZE(wyswT));
	HD44780_TEXT(strupr(" wy\x06wietlacz|ey"));
//	itoa(get_mem_unused(),buf,10);
//	itoa(RAMEND-RAMSTART,buf,10);
//	HD44780_TEXT(buf);
	_delay_ms(T_INTRO);
	HD44780_CLR();
	HD44780_CHAR('v');
	HD44780_TEXT(STRING(__DATE__));
	HD44780_XY(0,1);
	HD44780_TEXT(STRING(__TIME__));
	_delay_ms(T_INTRO);
	HD44780_NR(0,wyswT);
	HD44780_CLR();
	HD44780_TEXT(strupr("23.45\x01 35.22\x01 51.23\x03"));

#endif

	//HD44780_CUSTOMCHAR(1, linia+2);
	HD44780_CLR();
	//HD44780_TEXT(strupr("\x08\x01\x02\x03\x04\x05\x06\x07"));
	HD44780_TEXT("KU1PA");
	_delay_ms(700);
	while (1) //pêtla nieskoñczona
	{

	}

}

