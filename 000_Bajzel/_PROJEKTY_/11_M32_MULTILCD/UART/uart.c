/*
 * ATMEGA32L_LCD uart.c
 *
 *  Stworzono: 2014 23-09-2014
 *      Autor: D¹browski Tomasz
 *     Edycja: 09:39:17
 */



#include <avr/io.h>
#include <avr/interrupt.h>
//#include <util/delay.h>
#include "uart.h"

volatile unsigned char UART_CHAR;       //odebrana liczba X
volatile unsigned char UART_REC =0;//flaga informuj¹ca main o odebraniu liczby
static char BUF_UART[30] = "y = 0.3187x^2 + 2x - 7"; //bufor nadawania

//bufor znaków ze wzorem funkcji, który wysy³amy po starcie programu
volatile unsigned int BUF_IND;           //indeks bufora nadawania


void UART_INIT( void )
{

#ifndef BAUD
	#define BAUD 19200        //tutaj podaj ¿¹dan¹ prêdkoœæ transmisji
#endif
#include <util/setbaud.h> //linkowanie tego pliku musi byæ
                           //po zdefiniowaniu BAUD
 //ustaw obliczone przez makro wartoœci
 UBRRH = UBRRH_VALUE;
 UBRRL = UBRRL_VALUE;
 #if USE_2X
   UCSRA |=  (1<<U2X);
 #else
   UCSRA &= ~(1<<U2X);
 #endif
        //set baud rate

        //set frame format: 8data, 1 stop bit
        UCSRC = (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
        UCSRB = (1<<TXEN) | (1<<RXEN);// | (1<<RXCIE); //enable reciver and transmitter
               //za³¹czamy dodatkowe przerwania
                  // UCSRB |= (1<<RXCIE);            //RXCIE bit is set - enable recieve complete interrupt
                  // UCSRB |= (1<<TXCIE);

}


void UART_SENDB(void){

 //funkcja rozpoczyna wysy³anie, wysy³aj¹c pierwszy znak znajduj¹cy siê
 //w tablicy BUF_UART[]. Pozosta³e wyœle funkcja przerwania,
 //która zostanie wywo³ana automatycznie po wys³aniu ka¿dego znaku.

 //dodaj do tekstu wyniku znaki koñca linii (CR+LF), by na
 //ekranie terminala wyniki pojawia³y siê w nowych liniach
 unsigned char z;
 for(z=0; z<30; z++){
  if(BUF_UART[z]==0){   //czy to koniec takstu w tablicy
   //tak znaleziono koniec tekstu, dodajemy znak CR
   BUF_UART[z]   = 13;  //znak powrotu karetki CR (Carrige Return)
   BUF_UART[z+1]  = 10; //znak nowej linii LF (Line Feed)
   BUF_UART[z+2]  = 0;  //znak koñca ci¹gu tekstu w tablicy
   break;
  }
 }


 //Zaczekaj, a¿ bufor nadawania bêdzie pusty
 while (!(UCSRA & (1<<UDRE)));

 //bufor jest pusty mo¿na wys³aæ

 //nastêpny znak do wysy³ki to znak nr 1
 BUF_IND = 0;

 //w³¹cz przerwania pustego bufora UDR, co rozpocznie transmisjê
 //aktualnej zawartoœci bufora
 UCSRB |= (1<<UDRIE);

}


ISR(USART_RXC_vect)
{
 //przerwanie generowane po odebraniu bajtu
 UART_CHAR = UDR;   //zapamiêtaj odebran¹ liczbê
 UART_REC = 1; //ustaw flagê odbioru liczby dla main()
}


//--------------------------------------------------------------

ISR(USART_UDRE_vect){

 //przerwanie generowane, gdy bufor nadawania jest ju¿ pusty,
 //odpowiedzialne za wys³anie wszystkich znaków z tablicy usart_bufor[]

 //sprawdzamy, czy bajt do wys³ania jest znakiem koñca tekstu, czyli zerem
 if(BUF_UART[BUF_IND]!= 0){

  //za³aduj znak do rejestru wysy³ki i ustaw indeks na nastêpny znak
  UDR = BUF_UART[BUF_IND++];

 }else{

  //osi¹gniêto koniec napisu w tablicy usart_bufor[]
  UCSRB &= ~(1<<UDRIE); //wy³¹cz przerwania pustego bufora nadawania
 }
}


void UART_SEND(uint8_t data) { //wysy³anie jednego bajta danych przez uart
        /* Wait for empty transmit buffer*/
        while (!(UCSRA & (1 << UDRE)))
              ;
        /* Put data into buffer, sends the data*/
        UDR = data;
}

void UART_SENDSTR(char *s) { //przesy³anie stringów przez uart
        for (; *s; s++)
                UART_SEND(*s);
}

