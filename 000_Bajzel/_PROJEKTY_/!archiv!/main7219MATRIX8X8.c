/*
 * 00_XYZ_AVRLIBS main7219.c
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 2016-09-26   
 *     Edycja: 11:52:06
 */



#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include "MAX7219/max7219.h"


#define LED180 0
#define COMMON_ANODE 1

#if LED180 == 0
#define segA (1<<6)
#define segB (1<<5)
#define segC (1<<4)
#define segD (1<<3)
#define segE (1<<2)
#define segF (1<<1)
#define segG (1<<0)
#define seg_ (1<<7)
#else
#define segA (1<<3)
#define segB (1<<2)
#define segC (1<<1)
#define segD (1<<6)
#define segE (1<<5)
#define segF (1<<4)
#define segG (1<<0)
#define seg_ (1<<7)
#endif



/*
*********************************************************************************************************
* LED Segments:         a
*                     ----
*                   f|    |b
*                    |  g |
*                     ----
*                   e|    |c
*                    |    |
*                     ----  o dp
*                       d
*   Register bits:
*      bit:  7  6  5  4  3  2  1  0
*           dp  a  b  c  d  e  f  g
*********************************************************************************************************
*/
//				dABCDEFG
uint8_t dec[] =
{ segA|segB|segC|segD|segE|segF,
  segB|segC,
  segA|segB|segD|segE|segG,
  segA|segB|segC|segD|segG,
  segB|segC|segF|segG,
  segA|segC|segD|segF|segG,
  segA|segC|segD|segE|segF|segG,
  segA|segB|segC,
  segA|segB|segC|segD|segE|segF|segG,
  segA|segB|segC|segD|segF|segG
};

uint8_t DigValues[] =	{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000};


void Display(int dig, int number);


int main(void)  //rozpoczynamy
{

MAX7219_INIT();
//MAX7219_WREG(MAX7219_REGDIGIT0,dec[3]);
//MAX7219_WREG(MAX7219_REGDIGIT2,dec[1]);
//MAX7219_WREG(MAX7219_REGDIGIT3,dec[4]);
//MAX7219_WREG(MAX7219_REGDIGIT7,dec[5]);
_delay_ms(2000);


uint16_t liczba=38457;

uint8_t poz=8;

while (liczba)
{
	Display(poz,liczba%10);
	liczba/=10;
	poz--;
}

//  Display(MAX7219_REGDIGIT5,8);
//	Display(MAX7219_REGDIGIT6,9);
//	Display(MAX7219_REGDIGIT7,7);


//MAX7219_WREG(6,0b00110000);


	while(1) //pêtla nieskoñczona
	{
		for (uint16_t i=0;i<=59153;i++)
		{
//			MAX7219_BRIGHTNESS(i+5);
//	MAX7219_WREG(MAX7219_REGDIGIT7,dec[i] );
			uint8_t poz=8;
			liczba=i;
			while (liczba)
			{
				Display(poz,liczba%10);
				liczba/=10;
				poz--;
			}


//			Display(MAX7219_REGDIGIT0,i);
		_delay_ms(10);
		}

	}
}


//void TurnOnAnimation (void){
//		for ( int z = 0; z &lt;= 1; z++ ){
//			int a = 1;
//			for ( int x = 0; x &lt;= 8; x++){
//				for ( int y = 0; y &lt;= 7; y++){
//					_delay_ms(5);
//					SPI_SEND(y,a);
//				}
//				a = a * 2;
//			}
//		}
//}

/*

			 ROW 1

			   A
		  ###########
		  #         #
ROW 6 - F #    7    # B - ROW 2
		  #    G    #
		  ###########
		  #         #
ROW 5 - E #         # C - ROW 3
		  #         #
		  ###########
		       D
		     ROW 4

	*/
// DigValues stores 8 byte for the display information

// Display function
void Display(int dig, int number)
{
	// Array with information about which bit has to be set to display the correct number
	// numbers[0] -&gt; Displays 0
	// numbers[4] -&gt; Displays 4
	// ...
    dig=9-dig;
	// Dig positioning byte
	char bPosition = 0x00000001;
//	uint8_t Values =0;
	// If [param]dig position is greater than 1
//	if ( dig > 1){
		// shift bPosition left to the correct digit ([param]dig -1 )
		bPosition = bPosition << (dig - 1) ;
//	}

	// iterate through the numbers array
	for(uint8_t x = 0; x < 8; x++){
		// check if bit is high at the current location of our selected number
		if ( dec[number] & (0x80 >> x) )
		{
			// if bitX of numbers is high just binary OR bPosition to the current DigValues value
			DigValues[x] |= bPosition;
//			Values |= bPosition;
		}else{
			// if bitX of numbers is low, invert bPosition and binary AND it with current DigValues value to set bit low
			DigValues[x] = (0b11111111 ^ bPosition) & DigValues[x] ;
//			Values = (0b11111111 ^ bPosition) & Values ;
		}
		// Send your information to max7219
		MAX7219_WREG(x,DigValues[x]);
//		MAX7219_WREG(x,Values);
	}
}

//void main (void)
//{
//
//	/* Initialize SPI */
//	spi_init();
//
//	/* Initialize MAX7219 */
//	max7219_Init();
//
//	/* Run a tiny animation */
//	TurnOnAnimation();
//
//	/* Write data to display */
//	Display(1,8);
//	Display(1,1);
//	Display(2,2);
//	Display(3,3);
//	Display(4,4);
//	Display(5,5);
//	Display(6,6);
//	Display(7,7);
//	Display(8,5);
//	Display(8,7);
//
//	while(1)
//	{
//
//	}
//}
//
//*/
