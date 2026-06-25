/////////////////////////////////////////////////////////////////
// LedTutorial.c - plik g³ówny tutoriala LED
//
// Autor: Ravender Kompilator: WinAVR
// Strona: www.wkretak.pl
/////////////////////////////////////////////////////////////////

#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdio.h> 

#include "SEGxx.h" //mój plik nag³ówkowy



volatile uint16_t millis;



void transform(char *slowo, int czas)
{
    sprintf(digBuf,"%s",slowo);
  _delay_ms(czas);
}




int main(void)
{

initSEGxx();
// timer0 - konfig
	TCCR0 |= (1<<CS02); //|(1<<CS00); //prescaler = 1024
	TIMSK |= (1<<TOIE0); // zezwolenie na przerwanie OVF

	

//_delay_ms(300);
sei(); // Zezwolenie globalne na przerwania
    
   
transform("Dwor-23*C",200);
transform("Piec17*C",200);
transform("Hol 26*C",200);
transform("20-05-02",200);
transform("56789+-/",200);
//transform("IJKLMNO",2000);
//transform("IJKLMNO",2000);

    
while(1)
{
	//liczba =(millis/10)%10000;
//dziesiatki = liczba / 10;
//jednosci = liczba % 10;
//_delay_ms(500); // przerwa miêdzy liczeniem
//if (liczba == 99) {liczba = 0;} else {liczba++;} //jeœli doliczy do 99 to licznik siê zeruje

	sprintf(digBuf,"%08u",(millis));

}

return 0;
}


ISR( TIMER0_OVF_vect ) 
{
	TCNT0 = 256-78; // czêstotliwoœæ 100 Hz
	++millis;

    updSEGxx();

}

