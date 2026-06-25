/*
 * 000_XYZ_AVRLIBS pins.h
 *
 *	    Autor: D¹browski Tomasz
 *  Stworzono: 2016-08-09
 *     Edycja: 08:57:02
 */

#ifndef ADDS_PINS_H_
#define ADDS_PINS_H_

#include <avr/io.h>


// *********************** PRZYGOTOWANIE TABLICOWEGO DOSTÊPU DO PINÓW *********************

/*
 * TODO: automatyzacja iloœci i tablicowania portów w zale¿noœci od procesora
 * 			obecnie seria M16/32/644
 *
*/

// Definicje portów umo¿liwiaj¹ce dzia³anie w formie numerycznej np.PORT(0)=PORT(pD)=PORTD

#define PORTS 4 //iloœæ portów
// Pozycja portów w tablicy adresów (AVR nie maj¹ portu I)
#define PA 0
#define PB 1
#define PC 2
#define PD 3
//#define PE 4
#define PF 5
#define PG 6
#define PH 7

#define PJ 8
#define PK 9
#define PL 10



#ifndef PORTA
	#define PORTA PORTB
#endif
// Przedefiniowanie nie istniej¹cych w danym AVR portów na PORTB, istniej¹cy
//	w ka¿dym AVR
#ifndef PORTC
	#define PORTC PORTB
#endif

#ifndef PORTD
	#define PORTD PORTB
#endif

#ifndef PORTE
	#define PORTE PORTB
#endif

#ifndef PORTF
	#define PORTF PORTB
#endif

#ifndef PORTG
	#define PORTG PORTB
#endif

#ifndef PORTH
	#define PORTH PORTB
#endif

#ifndef PORTJ
	#define PORTJ PORTB
#endif

#ifndef PORTK
	#define PORTK PORTB
#endif

#ifndef PORTL
	#define PORTL PORTB
#endif





//#define _INT2CHAR(a) ('A'+a)
//#define ucport(a) = &PORT ## _INT2CHAR(a)

#define _DDR(port) _SFR_IO8(_SFR_IO_ADDR(*(ucports[(port)]))-1) // adr. r. kier. DDRx
#define _PORT(port) _SFR_IO8(_SFR_IO_ADDR(*(ucports[(port)]))) // adr. r. wyj. PORTx
#define _PIN(port) _SFR_IO8(_SFR_IO_ADDR(*(ucports[(port)]))-2) // adr. r. wej. PINx

//#define _DDR(port) _SFR_IO8(_SFR_IO_ADDR(*(ucports(port)))-1) // adr. r. kier. DDRx
//#define _PORT(port) _SFR_IO8(_SFR_IO_ADDR(*(ucports(port)))) // adr. r. wyj. PORTx
//#define _PIN(port) _SFR_IO8(_SFR_IO_ADDR(*(ucports(port)))-2) // adr. r. wej. PINx



#define DDR(port) _DDR(port)
#define PORT(port) _PORT(port)
#define PIN(port) _PIN(port)


// Makra zmiany opisu Px,y np pD,5 na OCTET w formacie xxxxxyyy czyli port 0-31, pin 0-7 i vice-versa
#define G2O(port,pin) (((port) << 0x03)|((pin)&0x07)) //GPIO2OCTET
#define O2P(gpio) ((gpio) >> 0x03) //OCTET2PORT GPIO
#define O2B(gpio) ((gpio) & (0x07)) //OCTET2BIT-PIN GPIO
// Skrócone makro u³atwiaj¹ce dostêp do portów
#define P(a,b) G2O(p ## a,b)


// Makra obs³ugi pinów-gpio w formacie LOW(G2O(PA,3))
#define _OUTPUT(gpio)            (DDR(O2P(gpio))  |= (1 << (O2B(gpio))))
#define _INPUT(gpio)            (DDR(O2P(gpio))  &= ~(1 << (O2B(gpio))))
#define _HIGH(gpio)            (PORT(O2P(gpio))  |= (1 << (O2B(gpio))))
#define _LOW(gpio)            (PORT(O2P(gpio))  &= ~(1 << (O2B(gpio))))
#define _TOGGLE(gpio)          (PORT(O2P(gpio)) ^= (1 << (O2B(gpio))))
#define _READ(gpio)            ((PIN(O2P(gpio)) >> (O2B(gpio))) &  1)
//#define _READ(gpio)            (PIN(O2P(gpio)) & (1<<O2B(gpio)))  //ale to ju¿ podaje wagê bitu (niby to samo, ale nie!)
#define _PUT(gpio,value)    ((PORT(O2P(gpio))) = ( PORT(O2P(gpio)) & ( ~ (1 << O2B(gpio))) ) | ( ( 1 & (unsigned char)value ) << O2B(gpio)) )

//#define _PUT(type,name,bit,value)    type ## name = ( type ## name & ( ~ _BV(bit)) ) | ( ( 1 & (unsigned char)value ) << bit )

#define OUTPUT(gpio)	_OUTPUT(gpio)
#define INPUT(gpio)		 _INPUT(gpio)
#define HIGH(gpio)    	 _HIGH(gpio)
#define LOW(gpio)		_LOW(gpio)
#define TOGGLE(gpio)  	 _TOGGLE(gpio)
#define READ(gpio)   	 _READ(gpio)
#define PUT(gpio,value)	_PUT(gpio,value)



extern  volatile uint8_t *ucports[];

//*************************** KONIEC PLIKU NAG£ÓWKOWEGO ************




#endif /* ADDS_PINS_H_ */

