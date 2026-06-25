//**********************************************************
// plik hd44780.h
//
//LCD HDD44780 na 3 pinach 74HC595
//Wersja: 1.0
//Licencja: GNU GPL
//Autor: Sheep94
//Skype: asus850032
//
//szczegoly: http://mikrokontrolery.blogspot.com/2011/03/lcd-hd44780-na-3-pinach-74hc595.html
//
//Uwaga!
//Je¿eli po zaprogramowaniu mikrokontrolera wyœwietlacz nie chce ruszyæ poprawnie
//wy³¹cz zasilanie i w³¹cz ponownie
//
//**********************************************************


/*
Objêtoœæ programu po kompilacji: 520 bajtów

Pod³¹czenie wyœwietlacza do rejestru:

GND - RW

Q0 - RS
Q1 - EN
Q2 - D4
Q3 - D5
Q4 - D6
Q5 - D7
Q6 - Dowolne urz¹dzenie
Q7 - Dowolne urz¹dzenie
*/


#ifndef LCD_HD44780
#define LCD_HD44780


//******* Tutaj zdefiniuj gdzie pod³¹czy³eœ poszczególne sygna³y *************


//DS
#define pin_DS	 PD5
#define DDR_DS   DDRD
#define PORT_DS  PORTD


//STCP
#define pin_STCP 	PD6
#define DDR_STCP  	DDRD
#define PORT_STCP  	PORTD

//SHCP
#define pin_SHCP	PD7
#define DDR_SHCP  	DDRD
#define PORT_SHCP 	PORTD



//******* OpóŸnienie dla niektórych LCD *************

//Niektóre wyœwietlacze wymagaj¹ opóŸnienia pomiêdzy w³¹czeniem
//zasilania, a inicjalizacj¹. Je¿eli chcesz w³¹czyæ opóŸnienie usuñ znak
//komentarza i podaj liczbê milisekund, sugerujê oko³o 20.
//
//#define LCD_START_DELAY_MS 20




//******* Od tego miejsca ju¿ nic nie zmieniaj *************

#define DS_0 PORT_DS 		&= ~(1<<pin_DS) 
#define DS_1 PORT_DS 		|=  (1<<pin_DS)
#define STCP_0 PORT_STCP 	&= ~(1<<pin_STCP) 
#define STCP_1 PORT_STCP 	|=  (1<<pin_STCP)
#define SHCP_0 PORT_SHCP 	&= ~(1<<pin_SHCP)
#define SHCP_1 PORT_SHCP 	|=  (1<<pin_SHCP) 



///* RS */
//
//#define RS1      rejestr(0x01,LCDWRITE)
//#define RS0      rejestr(0x01,LCDERASE)
//
//
///* RW */
//
////#define RW1      rejestr(0x02,LCDWRITE)
////#define RW0      rejestr(0x02,LCDERASE)
//
//
///* E */
//
//#define E1       rejestr(0x02,LCDWRITE)
//#define E0       rejestr(0x02,LCDERASE)
//
//
///* D4 */
//
//#define D41      rejestr(0x04,LCDWRITE)
//#define D40      rejestr(0x04,LCDERASE)
//
//
///* D5 */
//
//#define D51      rejestr(0x08,LCDWRITE)
//#define D50      rejestr(0x08,LCDERASE)
//
//
///* D6 */
//
//#define D61      rejestr(0x10,LCDWRITE)
//#define D60      rejestr(0x10,LCDERASE)
//
//
///* D7 */
//
//#define D71      rejestr(0x20,LCDWRITE)
//#define D70      rejestr(0x20,LCDERASE)
//
//
///* Q7 */
//
//#define Q71      rejestr(0x80,LCDWRITE)
//#define Q70      rejestr(0x80,LCDERASE)
//
//
///* Q7 */
//
//#define Q61      rejestr(0x40,LCDWRITE)
//#define Q60      rejestr(0x40,LCDERASE)
//

/* RS */

#define RS1      rejestr(0x08,LCDWRITE)
#define RS0      rejestr(0x08,LCDERASE)


/* RW */

//#define RW1      rejestr(0x02,LCDWRITE)
//#define RW0      rejestr(0x02,LCDERASE)


/* E */

#define E1       rejestr(0x04,LCDWRITE)
#define E0       rejestr(0x04,LCDERASE)


/* D4 */

#define D41      rejestr(0x10,LCDWRITE)
#define D40      rejestr(0x10,LCDERASE)


/* D5 */

#define D51      rejestr(0x20,LCDWRITE)
#define D50      rejestr(0x20,LCDERASE)


/* D6 */

#define D61      rejestr(0x40,LCDWRITE)
#define D60      rejestr(0x40,LCDERASE)


/* D7 */

#define D71      rejestr(0x80,LCDWRITE)
#define D70      rejestr(0x80,LCDERASE)


/* Q7 */

#define Q71      rejestr(0x01,LCDWRITE)
#define Q70      rejestr(0x01,LCDERASE)


/* Q7 */

#define Q61      rejestr(0x02,LCDWRITE)
#define Q60      rejestr(0x02,LCDERASE)




#define LCD_NOP asm volatile("nop\n\t""nop\n\t" "nop\n\t" "nop\n\t" ::);

#define LCDWRITE 1
#define LCDERASE 0

#define LCDCOMMAND 0
#define LCDDATA    1

#define LCD_LOCATE(x,y)  WriteToLCD(0x80|((x)+((y%2)*0x40+(y/2)*0x14)), LCDCOMMAND)

#define LCD_CLEAR              WriteToLCD(0x01, LCDCOMMAND)
#define LCD_HOME               WriteToLCD(0x02, LCDCOMMAND)

/* IDS */

#define LCDINCREMENT           0x02
#define LCDDECREMENT           0x00
#define LCDDISPLAYSHIFT        0x01

#define LCD_ENTRY_MODE(IDS)    WriteToLCD(0x04|(IDS), LCDCOMMAND)

/* BCD */
#define LCDDISPLAY             0x04
#define LCDCURSOR              0x02
#define LCDBLINK               0x01

#define LCD_DISPLAY(DCB)       WriteToLCD(0x08|(DCB), LCDCOMMAND)

/* RL */
#define LCDLEFT                0x00
#define LCDRIGHT               0x04

#define LCD_SHIFT_DISPLAY(RL)  WriteToLCD(0x18|(RL), LCDCOMMAND)
#define LCD_SHIFT_CURSOR(RL)   WriteToLCD(0x10|(RL), LCDCOMMAND)

#define LCD_CGRAM_ADDRESS(A)   WriteToLCD(0x40|((A)&0x3f), LCDCOMMAND)
#define LCD_DDRDM_ADDRESS(A)   WriteToLCD(0x80|((A)&0x7f), LCDCOMMAND)

#define LCD_WRITE_DATA(D)      WriteToLCD((D),LCDDATA)

void rejestr(unsigned char data,unsigned char WriteOrErase);
volatile static unsigned char tempdata = 0;
void lcd_init(void);
void WriteToLCD(char v,unsigned char rs);
void lcd_puts(char *str);

#endif

