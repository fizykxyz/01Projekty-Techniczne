#ifndef HD44780CFG_H_
#define HD44780CFG_H_
/* HD44780 LCD przypisanie pinów */
/* Piny sterowania */
#define HD44780_RS C, 6
#define HD44780_E  A, 7
#define HD44780_RW C, 7
#define HD44780_BL A, 1
/* Piny danych 4 bit */
#define HD44780_D4 C, 2
#define HD44780_D5 C, 3
#define HD44780_D6 C, 4
#define HD44780_D7 C, 5

/* Parametry wyœwietlacza */

#define HD44780_W 20
#define HD44780_H 2


#define HD44780_CODEPAGE HD44780_CODEPAGE_BEZPL

#endif /* HD44780CFG_H_ */
