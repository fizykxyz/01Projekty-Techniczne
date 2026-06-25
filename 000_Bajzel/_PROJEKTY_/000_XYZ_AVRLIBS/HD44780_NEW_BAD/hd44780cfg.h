#ifndef HD44780CFG_H_
#define HD44780CFG_H_

#include <avr/io.h>
#include "../ADDS/pins.h"






/* HD44780 LCD przypisanie pinów */
/* Piny sterowania */




//#define HD44780_E  PA, 7


#define HD44780_RS G2O(PC, 6)
#define HD44780_RW G2O(PC, 7)
#define HD44780_BL G2O(PA, 1)
/* Piny danych 4 bit */
#define HD44780_D4 G2O(PC,2)
#define HD44780_D5 G2O(PC, 3)
#define HD44780_D6 G2O(PC, 4)
#define HD44780_D7 G2O(PC, 5)

/* Parametry wyœwietlacza */

//#define HD44780_W 20
//#define HD44780_H 2
//

//#define HD44780_CODEPAGE HD44780_CODEPAGE_BEZPL

#endif /* HD44780CFG_H_ */
