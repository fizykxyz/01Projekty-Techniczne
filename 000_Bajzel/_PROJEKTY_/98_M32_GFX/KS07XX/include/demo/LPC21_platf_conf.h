/****************************************************************************
* Biblioteka obslugi wyswietlacza graficznego, opartego na sterowniku
* KS07XX lub odpowiedniku.
* 
* Plik konfiguracyjny platformy ARM7 LPC21xx dla programu demonstracyjnego.
*
* Nazwa pliku: LPC21_platf_conf.h
* Data: 10.10.2007
* Wersja: 1.0
*
* Copyright (C) 2007 by Rafal Baranowski.
*
* Niniejsze oprogramowanie udostepniane jest nieodplatnie i moze byc
* powielane, modyfikowane i wykorzystywane w celach komercyjnych pod
* warunkiem zachowania noty copyright.
* Na poprawne dzialanie i bezpieczenstwo tego oprogramowania nie jest
* udzalana zadna gwarancja.
****************************************************************************/

#ifndef __LPC21_PLATF_CONF_H__
#define __LPC21_PLATF_CONF_H__ 1

#include <inttypes.h>

/* 
 * Parametry systemu
 */
#define DISP_OS_PRESENT 0     // system operacyjny obecny (1) lub nie (0)

/*
 *  Ustawienia magistrali
 */
#define DISP_INTERF_TYPE 0    // typ interfejsu: 8080 (0) lub 6800 (1)
#define DISP_DB_WIDTH 8       // szerokosc magistrali (8 bitow)
#define DISP_DB_MIX 0         // magistrala typu mixed (1) lub not-mixed (0)

/*
 *  Ustawienia dla magistrali typu not-mixed (8-bitowej)
 */ 
#define DISP_DB_SET IOSET1
#define DISP_DB_CLR IOCLR1
#define DISP_DB_PIN IOPIN1
#define DISP_DB_DDR IODIR1
#define DISP_DB_OFFSET 16     // przesuniecie magistrali (numer bitu DB0)


/*
 *  Ustawienia dla magistrali typu mixed (8-bitowej)
 */
#define DISP_DB0_SET IOSET1   // linia DB 0
#define DISP_DB0_CLR IOCLR1
#define DISP_DB0_PIN IOPIN1
#define DISP_DB0_DDR IODIR1
#define DISP_DB0_NO 16
#define DISP_DB1_SET IOSET1   // linia DB 1
#define DISP_DB1_CLR IOCLR1
#define DISP_DB1_PIN IOPIN1
#define DISP_DB1_DDR IODIR1
#define DISP_DB1_NO 17 
#define DISP_DB2_SET IOSET1   // linia DB 2
#define DISP_DB2_CLR IOCLR1
#define DISP_DB2_PIN IOPIN1
#define DISP_DB2_DDR IODIR1
#define DISP_DB2_NO 18
#define DISP_DB3_SET IOSET1   // linia DB 3
#define DISP_DB3_CLR IOCLR1
#define DISP_DB3_PIN IOPIN1
#define DISP_DB3_DDR IODIR1
#define DISP_DB3_NO 19
#define DISP_DB4_SET IOSET1   // linia DB 4
#define DISP_DB4_CLR IOCLR1
#define DISP_DB4_PIN IOPIN1
#define DISP_DB4_DDR IODIR1
#define DISP_DB4_NO 20
#define DISP_DB5_SET IOSET1   // linia DB 5
#define DISP_DB5_CLR IOCLR1
#define DISP_DB5_PIN IOPIN1
#define DISP_DB5_DDR IODIR1
#define DISP_DB5_NO 21
#define DISP_DB6_SET IOSET1   // linia DB 6
#define DISP_DB6_CLR IOCLR1
#define DISP_DB6_PIN IOPIN1
#define DISP_DB6_DDR IODIR1
#define DISP_DB6_NO 22
#define DISP_DB7_SET IOSET1   // linia DB 7
#define DISP_DB7_CLR IOCLR1
#define DISP_DB7_PIN IOPIN1
#define DISP_DB7_DDR IODIR1
#define DISP_DB7_NO 23

/*
 *  Ustawienia linii specjalnych
 */
#define DISP_RS_SET IOSET0     // linia Register Select (A0)
#define DISP_RS_CLR IOCLR0
#define DISP_RS_DDR IODIR0
#define DISP_RS_NO 16
/*
#define DISP_RW_SET IOSET0     // linia Read/nWrite (interfejs 6800)
#define DISP_RW_CLR IOCLR0
#define DISP_RW_DDR IODIR0
#define DISP_RW_NO 17
#define DISP_E1_SET IOSET0     // linia Enable1 (interfejs 6800)
#define DISP_E1_CLR IOCLR0
#define DISP_E1_DDR IODIR0
#define DISP_E1_NO 18
*/
#define DISP_WR_SET IOSET0     // linia nWrite (interfejs 8080)
#define DISP_WR_CLR IOCLR0
#define DISP_WR_DDR IODIR0
#define DISP_WR_NO 17
#define DISP_RD_SET IOSET0     // linia nRead (interfejs 8080)
#define DISP_RD_CLR IOCLR0
#define DISP_RD_DDR IODIR0
#define DISP_RD_NO 18
#define DISP_CS1_SET IOSET0    // linia Chip Select1 (interfejs 8080)
#define DISP_CS1_CLR IOCLR0
#define DISP_CS1_DDR IODIR0
#define DISP_CS1_NO 19

#define DISP_RES_SET IOSET0    // linia nReset
#define DISP_RES_CLR IOCLR0
#define DISP_RES_DDR IODIR0
#define DISP_RES_NO 20

#endif // ! __LPC21_PLATF_CONF_H__
