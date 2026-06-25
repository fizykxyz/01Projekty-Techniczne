/*
This code is from Colin O'Flynn - Copyright (c) 2002
only minor changes by M.Thomas 9/2004
*/

#include <stdint.h>

#define CRC8INIT    0x00
#define CRC8POLY    0x18              //0X18 = X^8+X^5+X^4+X^0

uint8_t crc8( uint8_t DATA[], uint16_t NBYTES)
{
	uint8_t  CRC,bit_counter,b,feedback_bit;
	uint16_t loop_count;
	CRC = CRC8INIT;

	for (loop_count = 0; loop_count < NBYTES; loop_count++)
	{
		b = DATA[loop_count];
		
		bit_counter = 8;
		do {
			feedback_bit = (CRC ^ b) & 0x01;
	
			if ( feedback_bit == 0x01 ) {
				CRC = CRC ^ CRC8POLY;
			}
			CRC = (CRC >> 1) & 0x7F;
			if ( feedback_bit == 0x01 ) {
				CRC = CRC | 0x80;
			}
		
			b = b >> 1;
			bit_counter--;
		
		} while (bit_counter > 0);
	}
	
	return CRC;
}
