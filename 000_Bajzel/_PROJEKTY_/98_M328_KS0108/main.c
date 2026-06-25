/*
 * 98_M328_KS0108 main.c
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 2016-09-24   
 *     Edycja: 21:42:41
 */



#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/pgmspace.h>

#include "font5x8.h"

// port szyny danych
#define LCD_DATA_PORT PORTD
#define LCD_DATA_PIN PIND
#define LCD_DATA_DDR DDRD
// port sygna³ów steruj¹cych
#define LCD_CTRL_PORT PORTC
#define LCD_CTRL_PIN PINC
#define LCD_CTRL_DDR DDRC
// sygna³y steruj¹ce
#define LCD_CS1P PC0
#define LCD_CS2P PC1
#define LCD_EN PC4
#define LCD_RW PC2
#define LCD_RS PC3


#define KS0108_SCREEN_WIDTH		128
#define KS0108_SCREEN_HEIGHT	64


#define DISPLAY_SET_Y       0x40
#define DISPLAY_SET_X       0xB8
#define DISPLAY_START_LINE  0xC0
#define DISPLAY_ON_CMD		0x3E
  #define ON	0x01
  #define OFF	0x00
#define DISPLAY_STATUS_BUSY	0x80




// makroinstrukcje ustawienia stanu na linii CS1
#define SET_CS1() (LCD_CTRL_PORT |= (1 << LCD_CS1P))
#define CLR_CS1() (LCD_CTRL_PORT &= ~(1 << LCD_CS1P))
// makroinstrukcje ustawienia stanu na linii CS2
#define SET_CS2() (LCD_CTRL_PORT |= (1 << LCD_CS2P))
#define CLR_CS2() (LCD_CTRL_PORT &= ~(1 << LCD_CS2P))
// makroinstrukcje ustawienia stanu na linii EN
#define SET_EN() (LCD_CTRL_PORT |= (1 << LCD_EN))
#define CLR_EN() (LCD_CTRL_PORT &= ~(1 << LCD_EN))
// makroinstrukcje ustawienia stanu na linii RW
#define SET_RW() (LCD_CTRL_PORT |= (1 << LCD_RW))
#define CLR_RW() (LCD_CTRL_PORT &= ~(1 << LCD_RW))
// makroinstrukcje ustawienia stanu na linii RS
#define SET_RS() (LCD_CTRL_PORT |= (1 << LCD_RS))
#define CLR_RS() (LCD_CTRL_PORT &= ~(1 << LCD_RS))
// makroinstrukcje ustawiajace odpowiedni¹ kombinacjê sygna³ów CS1 i CS2
#define LCD_CS0() CLR_CS1();SET_CS2();
#define LCD_CS1() SET_CS1();CLR_CS2();
#define LCD_NOCS() SET_CS1();SET_CS2();

unsigned char lcd_x, lcd_y;

void lcdInit(void)
{
LCD_DATA_DDR = 0xFF;
LCD_CTRL_DDR = 0xFF;
}

void delay(void)
{
asm("nop");asm("nop");
}

void lcdWait(void)
{
LCD_DATA_DDR = 0x00; // ustawienie portu danych w tryb wejœciowy
CLR_RS(); // niski stan na linii RS -> odczyt rejestru statusu
SET_RW(); // wysoki stan na linii RW -> odczyt z wyœwietlacza
do { //pêtla
delay(); // opóŸnienie
SET_EN(); // ustaw liniê EN
delay(); // opóŸnienie
CLR_EN(); // wyzeruj liniê EN
} while((LCD_DATA_PIN & DISPLAY_STATUS_BUSY)); // powtarzaj do
// wyzerowania flagi BUSY
}

void lcdWriteCmd(unsigned char x)
{
lcdWait();
CLR_RS();
CLR_RW();
LCD_DATA_DDR = 0xFF;
LCD_DATA_PORT = x;
SET_EN();
delay();
CLR_EN();
}

void lcdWriteData(unsigned char data)
{
if(lcd_x < 64) // jeœli wspó³rzêdna x wyœwietlacza < 64
{LCD_CS0()} // to zapisujemy do pierwszego kontrolera
else // w przeciwnym razie
{LCD_CS1()} // zapisujemy do drugiego kontrolera
lcdWait(); // oczekiwanie na gotowoœæ kontrolera
SET_RS(); // wysoki stan na linii RS -> dane
CLR_RW(); // niski stan na linii RW -> zapis
LCD_DATA_DDR = 0xFF; // port danych -> wyjœcie
LCD_DATA_PORT = data; // wystawienie na port danej
SET_EN(); // wysoki stan na linii EN
delay(); // opóŸnienie
CLR_EN(); // niski stan na linii EN
lcd_x++; // zwiêkszenie wspó³rzêdnej x wyœwietlacza (pomocniczej)
if(lcd_x > 127) // jesli koniec ekranu
lcd_x = 0; // to wyzeruj wspó³rzêdn¹ x
LCD_NOCS();
}

unsigned char lcdReadData(void)
{
unsigned char data;
if(lcd_x < 64) // jeœli wspó³rzêdna x wyœwietlacza < 64
{LCD_CS0()} // to odczytujemy z pierwszego kontrolera
else // w przeciwnym razie
{LCD_CS1()} // odczytujemy z drugiego kontrolera
lcdWait(); // oczekiwanie na gotowoœæ kontrolera
SET_RS(); // wysoki stan na linii RS -> dane
SET_RW(); // wysoki stan na linii RW -> odczyt
SET_EN(); // wysoki stan na linii EN
delay(); // opóŸnienie
LCD_DATA_DDR = 0x00; // ustawienie portu danych w tryb wejsciowy
data = LCD_DATA_PIN; // odczyt danych z portu
CLR_EN(); // niski stan na linii EN
lcd_x++; // zwiêkszenie wspó³rzêdnej x wyœwietlacza
if(lcd_x > 127) // jesli koniec ekranu
lcd_x = 0; // to wyzeruj wspó³rzêdn¹ x
LCD_NOCS();
return data;
}

void lcdGoTo(unsigned char x, unsigned char y)
{
lcd_x = x; // przypisanie wspó³rzêdym globalnym nowych wartoœci
lcd_y = y;
if(lcd_x > 63) // jeœli wspó³rzêdna pozioma jest wiêksza od 64 to
{
LCD_CS1(); // uaktywnienie drugiego kontrolera
lcdWriteCmd(DISPLAY_SET_X | lcd_y); // zapis wspó³rzêdnej pionowej
lcdWriteCmd(DISPLAY_SET_Y | (lcd_x - 64)); // zapis wspó³rzêdnej poziomej
}
else // w przeciwnym razie
{
LCD_CS0(); // uatywnienie pierwszego kontrolera
lcdWriteCmd(DISPLAY_SET_X | lcd_y); // zapis wspó³rzêdnej pionowej
lcdWriteCmd(DISPLAY_SET_Y | lcd_x); // zapis wspó³rzêdnej poziomej
LCD_CS1(); // uaktywnienie drugiego kontrolera
lcdWriteCmd(DISPLAY_SET_X | lcd_y); // zapis wspó³rzêdnej pionowej
lcdWriteCmd(DISPLAY_SET_Y | 0 ); // wyzerowanie wspó³rzêdnej poziomej
}
LCD_CS0(); // uaktywnienie pierwszego kontrolera
lcdWriteCmd(DISPLAY_START_LINE | 0); //
LCD_CS1(); // uaktywnienie drugiego kontrolera
lcdWriteCmd(DISPLAY_START_LINE | 0);
LCD_NOCS();
}

void lcdOn(void)
{
LCD_CS0(); // aktywny pierwszy kontroler
lcdWriteCmd(DISPLAY_ON_CMD | ON); // w³¹czenie wyœwietlania danych
LCD_CS1(); // aktywny drugi kontroler
lcdWriteCmd(DISPLAY_ON_CMD | ON); // w³¹czenie wyœwietlania danych
LCD_NOCS(); //
}

void lcdOff(void)
{
LCD_CS0(); // aktywny pierwszy kontroler
lcdWriteCmd(DISPLAY_ON_CMD | OFF); // wy³¹czenie wyœwietlania danych
LCD_CS1(); // aktywny drugi kontroler
lcdWriteCmd(DISPLAY_ON_CMD | OFF); // wy³¹czenie wyœwietlania danych
LCD_NOCS(); //
}

void lcdCls(void)
{
unsigned char x, y; // pomocnicze zmienne
for (y = 0; y < 8; y++) // 8-krotne powtórzenie pêtli
{
lcdGoTo(0,y); // ustawienie wspó³rzêdnej y wyœwietlacza
for (x = 0; x < 128; x++) // 128-krotne powtórzenie pêtli
lcdWriteData(0x05); // zapis do wyœwietlacza
}
lcdGoTo(0,0); // ustawienie pocz¹tkowych wspó³rzêdnych
}


void lcdWriteChar(char x)
{
char i;
x -= 32; // konwersja kodu znaku
for(i = 0; i < 5; i++)
lcdWriteData(pgm_read_byte(font5x8 + (5 * x) + i)); // zapis do wyœwietlacza
//5 kolejnych bajtów tworz¹cych znak
lcdWriteData(0); // odstêp miêdzy znakami
}


void lcdWriteString(char * s)
{
while(*s) // dopóki znak wskazywany przez s jest ró¿ny od zera
lcdWriteChar(*s++); // zapis znaku
}

//void lcdWriteStringPgm(const char * s)
//{
//char c; // pomocnicza zmienna
//while(c = pgm_read_byte(s++)) // dopóki znak wskazywany przez s jest ró¿ny od
//// zera
//lcdWriteChar(c); // zapis znaku
//}
//

void lcdLocate(unsigned char x, unsigned char y)
{
lcdGoTo(x * 6, y); // zapis
}



void lcdSetPixel(unsigned char x, unsigned char y)
{
char temp; // zmienna pomocnicza
lcdGoTo(x, y/8); // ustawienie wspó³rzêdnych
temp = lcdReadData(); //
temp = lcdReadData(); // podwójny odczyt danych
lcdGoTo(x, y/8); // ponowne ustawienie wspó³rzêdnych
lcdWriteData(temp | (1 << (y % 8))); // zapis odpowiednio zmodyfikowanej wartoœci
}

void lcdClrPixel(unsigned char x, unsigned char y)
{
char temp;
lcdGoTo(x, y/8);
temp = lcdReadData();
temp = lcdReadData();
lcdGoTo(x, y/8);
lcdWriteData(temp & (0xFF - (1 << (y % 8))));
}



int main(void)  //rozpoczynamy
{

 lcdInit();

 lcdCls();
 lcdWriteString("TEST wr");
 lcdSetPixel(20,50);

	while(1) //pêtla nieskoñczona
	{



	}
}
