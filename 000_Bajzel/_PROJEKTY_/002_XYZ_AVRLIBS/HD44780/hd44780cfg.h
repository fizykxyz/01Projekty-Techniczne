#ifndef HD44780CFG_H_
#define HD44780CFG_H_

#include <avr/io.h>
#include "../ADDS/pins.h"

extern static THD44780 HD44780_E={20,2,G2O(PA, 7)};

#ifndef HD44780_RS
	#define HD44780_RS G2O(PC, 6)
#endif
#ifndef HD44780_RW
	#define HD44780_RW G2O(PC, 7)
#endif
#ifndef HD44780_BL
	#define HD44780_BL G2O(PC, 5)
#endif


/* Piny danych 4 bit */
#ifndef HD44780_D4
	#define HD44780_D4 G2O(PD, 4)
#endif
#ifndef HD44780_D5
#define HD44780_D5 G2O(PD, 5)
#endif
#ifndef HD44780_D6
#define HD44780_D6 G2O(PD, 6)
#endif
#ifndef HD44780_D7
#define HD44780_D7 G2O(PD, 7)
#endif


/* Parametry wyœwietlacza */



#define HD44780_CODEPAGE HD44780_CODEPAGE_BEZPL

#endif /* HD44780CFG_H_ */
