//*******************************************************
//*****           HEADER FILE : TIMERS.H         ********
//*******************************************************
	#include <avr/io.h>

#define __enable_tr0_interrupt()   TIMSK |= (1<<TOIE0)
#define __disable_tr0_interrupt()  TIMSK &= ~(1<<TOIE0)
#define __start_timout()           ( TCNT0=0 ,\
	                               __enable_tr0_interrupt() )

#define __enable_tr1_interrupt()   TIMSK |= (1<<OCIE1A)
#define __disable_tr1_interrupt()  TIMSK &= ~(1<<OCIE1A)
/*
#define __start_timout()           ( TCNT1=0 ,\
	                               __enable_tr1_interrupt() )
*/

#define __toggle_led() (PORTC & (1<<2)) ? (PORTC &= ~(1<<2)) : (PORTC |= (1<<2))

void _init_timer1(void);
void _init_timer0(void);

