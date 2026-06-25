#ifndef HD44780CFG_H_
#define HD44780CFG_H_

#include <avr/io.h>
#include "../ADDS/pins.h"


typedef struct
{
	uint8_t KOL;//il.kolumn
	uint8_t LIN;//il.linii
    uint8_t EN;//linia sygna³owa Enable
} THD44780;


extern THD44780 H44780_E;

/* HD44780 LCD przypisanie pinów */
/* Piny sterowania */

static THD44780 HD44780_E={20,2,G2O(PA, 7)};


#define HD44780_RS G2O(PC, 6)
#define HD44780_RW G2O(PC, 7)
#define HD44780_BL G2O(PC, 5)
/* Piny danych 4 bit */
#define HD44780_D4 G2O(PD, 4)
#define HD44780_D5 G2O(PD, 5)
#define HD44780_D6 G2O(PD, 6)
#define HD44780_D7 G2O(PD, 7)

/* Parametry wyœwietlacza */



//#define HD44780_CODEPAGE HD44780_CODEPAGE_BEZPL

#endif /* HD44780CFG_H_ */
