#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "PetitFS/diskio.h"
#include "PetitFS/pff.h"

//#include "LCD/lcd44780.h"
//#include "PCD8544/pcd8544.h"


char file_name[] = "test.txt";
char bufor[128];



/*-----------------------------------------------------------------------*/
/* Main                                                                  */
int main (void)
{
	DDRD |= (1<<PD7);	// ustawiamy kierunek linii podœwietlenia LCD jako WYJŒCIE
	PORTD |= (1<<PD7);	// za³¹czamy podœwietlenie LCD - stan wysoki

	BYTE res;
	WORD s1;

	FATFS fs;			/* File system object */
	//DIR dir;			/* Directory object */
	//FILINFO fno;		/* File information */

#define SCK PB7
#define MISO PB6
#define MOSI PB5
#define CS PB3
	DDRB |= (1<<CS)|(1<<MOSI)|(1<<SCK)|(1<<CS);
	PORTB |= (1<<CS)|(1<<MISO);
	SPCR |= (1<<SPE)|(1<<MSTR);

//	PCD8544_INIT();
//	lcd_init();
//
//PCD8544_TEXT("odczyt: test.txt",normal);
//	lcd_locate(1,0);
//
	_delay_ms(100);

	uint8_t i=255;
	while( i-- && (res = disk_initialize()) );

	if( res == FR_OK ) {
		res = pf_mount(&fs);
		if( res == FR_OK ) {
			res = pf_open(file_name);
			if( res == FR_OK ) {

				res = pf_read(bufor, sizeof(bufor), &s1);

				if( res == FR_OK ) {
					bufor[s1+1] = 0;
			//		PCD8544_TEXT(bufor,normal);

					_delay_ms(3000);

//					lcd_locate(0,0);
//					lcd_str("zapis-> test.txt");

					if(bufor[0]=='t') sprintf(bufor, "TEST ****");
					else sprintf(bufor, "test ----");

					s1=4;
					res = pf_write(bufor, sizeof(bufor), &s1);
					//write_close();
					pf_write(0, 0, &s1);

					if( res != FR_OK ) {
//						lcd_locate(0,0);
//						lcd_str("write file error");
					} else {
//						lcd_locate(1,0);
//						lcd_str("OK,   zresetuj  ");
					}

					pf_mount(NULL);



				} else _delay_ms(15);//PCD8544_TEXT("read error",normal);

			} else _delay_ms(15);//PCD8544_TEXT("opern file error",normal);
		} else _delay_ms(15);//PCD8544_TEXT("mount error",normal);
	} else _delay_ms(15);//PCD8544_TEXT("disk init error",normal);

	while(1)
	{
		_delay_ms(300);
		PORTD^=(1<<7);
	}

}


