/*
 * 13_M32_LED4x12 main.c
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 22 kwi 2016      
 *     Edycja: 06:10:50
 */

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

typedef struct
{
	volatile uint8_t * port;
	uint8_t pin;
} port_pin_t;

port_pin_t pinarray[] =
{
{ &PORTB, (1 << 3) },
{ &PORTD, (1 << 2) },
{ &PORTA, (1 << 5) },
{ &PORTC, (1 << 7) } };

static inline void set_pin(uint8_t n)
{
	*pinarray[n].port |= pinarray[n].pin;
}

static inline void clr_pin(uint8_t n)
{
	*pinarray[n].port &= ~pinarray[n].pin;
}

int main()
{
	DDRA = 255;
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFF;

	uint8_t i = 0;
	// Write your code here
	while (1)
	{
		for (i = 0; i < 4; i++)
		{
			_delay_ms(5);
			set_pin(i);
			_delay_ms(1);
			clr_pin(i);

		}
	}
	return 0;
}
