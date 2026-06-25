#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#define C1 15288
#define C1d 14431
#define D1 13621
#define D1d 12856
#define E1 12134
#define F1 11453
#define F1d 10811
#define G1 10204
#define G1d 9631
#define A1 9090
#define A1d 8580
#define H1 8099
#define C2 7648
#define C2d 7220
#define D2 6814
#define D2d 6430
#define E2 6069
#define F2 5730
#define G2 5102
#define G2d 4815
#define A2 4545
#define A2d 4291
#define H2 4049
#define C3 3822
#define P 0
#define End 1
#define N32 125.000
#define N16 250.000
#define N8 500.000
#define N4 1000.000
#define N2 2000.000
#define N1 4000.000

volatile int track=0;
volatile int NoteLong = 250;
volatile int loaded=0;
volatile int i = 1;
volatile int Bit = 0x00;

volatile char flag;
volatile int count=0;
volatile char status = 1;


const int tanki[] PROGMEM={160.000,P,N32,C2,N32,D2,N32,D2d,N32,C2,N32,D2,N32,D2d,N32,D2d,N32,F2,N32,G2,N32,D2d,N32,F2,N32,G2,N32,F2,N32,G2,N32,A2,N32,F2,
            N32,G2,N32,A2,N32,G2d,N32,A2d,N32,C3,N32,G2d,N32,A2d,N32,C3,N32,C3,N32,P,N32,C3,N32,C3,N32,C3,N32,C3,N32,End}; //мелодия танчиков


const int mario[] PROGMEM={100.000,P,N16,E2,N32,E2,N32,P,N32,E2,N16,P,N32,C2,N32,E2,N32,P,N32,G2,N32,P,N32,P,N16,G1,N32,P,N16,P,N16,
                C2,N32,P,N16,G1,N32,P,N16,E1,N16,P,N16,P,N32,A1,N32,P,N32,H1,N32,P,N32,A1d,N32,A1,N32,P,N32,
                G1,N32,E2,N32,G2,N32,A2,N32,P,N32,F2,N32,G2,N32,P,N32,E2,N32,P,N32,C2,N32,D2,N32,H1,N32,End};  //мелодия марио


const int miniPolka[] PROGMEM={120.000,P,N16,C2,N16,E2,N16,G2,N16,E2,N16,G2,N16,F2,N16,D2,N8,G2,N16,F2,N16,D2,N8,G2,N16,E2,N16,C2,N8,
                C2,N16,E2,N16,G2,N16,E2,N16,A2,N16,G2,N16,F2,N8,G2,N16,F2,N16,E2,N16,D2,N16,C2,N8,End};


const int bumer[] PROGMEM={170.000,P,N16,E2,N16,G2,N8,P,N8,P,N8,G2,N16,E2,N8,P,N8,P,N8,A2,N16,G2,N16,A2,N16,G2,N16,A2,N16,G2,N16,A2,N16,G2,N16,A2,N16,H2,N8,P,N8,End};


const int nokia[30] PROGMEM={120.000,P,N16,E2,N16,D2,N16,F1d,N8,G1d,N8,C2d,N16,H1,N16,D1,N8,E1,N8,H1,N16,A1,N16,C1d,N8,E1,N8,A1,N8,End};


const int mortal[66] PROGMEM={140.000,A1,N16,A1,N16,C2,N16,A1,N16,D2,N16,A1,N16,E2,N16,D2,N16,C2,N16,C2,N16,E2,N16,C2,N16,G2,N16,C2,N16,E2,N16,C2,N16,
                G1,N16,G1,N16,H1,N16,G1,N16,C2,N16,G1,N16,D2,N16,C2,N16,F1,N16,F1,N16,A1,N16,F1,N16,C2,N16,F1,N16,C2,N16,H1,N16,End};


const int kuznechik[] PROGMEM={130.000,P,N16,A2,N8,E2,N8,A2,N8,E2,N8,A2,N8,G2d,N8,G2,N8,P,N8,G2d,N8,E2,N8,G2d,N8,E2,N8,G2d,N8,A2,N8,End};

const int *playlist[]={
                    kuznechik,
                    tanki,
                    mario,
                    mortal,
                    nokia,
                    miniPolka,
                    bumer
                };


//interrupt [TIM1_COMPA] void timer1_compa_isr(void)
ISR (TIMER1_COMPA_vect)
{
    PORTC^=(1<<1);
}

// Timer 2 overflow interrupt service routine
ISR (TIMER2_OVF_vect)
{
    if(count)count--;


}

void play()
{
    OCR1AH = (char) (Bit >> 8);          //записываем новую ноту в OCR1A
    OCR1AL = (char) Bit;
    TIMSK |= 1 << OCIE1A;                // начали воспроизведение
    _delay_ms(250);                // длина ноты
    //_delay_ms(NoteLong);                // длина ноты
    TIMSK &= ~(1 << OCIE1A);            //закончилась нота
    _delay_ms(40);
    i = i + 2;
}

void pause()
{
    TIMSK &= ~(1<<OCIE1A);      // запрещаем прерывания
    //_delay_ms(NoteLong);
    _delay_ms(250);                // длина ноты

    TIMSK |= 1<<OCIE1A;        // разрешаем прерывания
    i+=2;
}


void stop()
{
    TIMSK &= ~(1<<OCIE1A);    // запрещаем прерывания
    Bit=0x00;
    PORTC&=~(1<<1);
    i=1;
    _delay_ms(200);

    if(++track>=7)            //если последний, то начать заново
    {
        track=0;
    }
}
//void Loading_melody()
//{
//      i=1;
//      while(playlist[track][i]!=1)
//      {
//      playlist[track][i+1]=playlist[track][i+1]/(playlist[track][0]/60.000);
//          // playlist[track][i+1]=pgm_read_word(playlist[track][i+1])/(pgm_read_word(playlist[track][0])/60.000);
//      i++;
//          if(playlist[track][i]!=1)
//          {
//          i++;
//          }
//      }
//      i=1;
//}

int main(void)
{

PORTC=0x00;
DDRC=0b000000011;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 8000,000 kHz
// Mode: CTC top=OCR1A
// OC1A output: Discon.
// OC1B output: Discon.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer 1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: On
// Compare B Match Interrupt: Off
//TCCR1A=0x00;
//TCCR1B=0x09;
TCCR1B |=1<<WGM12 | 1<<CS10;
//TCNT1H=0x00;
//TCNT1L=0x00;
//ICR1H=0x00;
//ICR1L=0x00;
//OCR1AH=0x00;
OCR1AL=0xFB;
//OCR1BH=0x00;
//OCR1BL=0x00;

// Timer/Counter 2 initialization
ASSR=0x00;
TCCR2|= 1 << WGM20 | 1 << WGM21;  // Fast PWM mode
TCCR2|= 1 << CS22;                // clk/256 - делим основную частоту МК на 256 (предделидель для таймера)
TCNT2=0x00;
OCR2=0x00;

//TIMSK=0x10;
TIMSK |= 1<<TOIE2 | 1<<OCIE1A; // разрешаем прерываемя по совпадению А от теймера/счетчика Т1
sei();

    while (1)
    {

        while (pgm_read_word(&(playlist[track][i])) != 1)      //читаем данные с массива мелодии до тех пор
        {                                                                                        //пока не увидим признак окончания мелодии

            Bit = pgm_read_word(&(playlist[track][i]));                  //считываю ноту
            NoteLong = pgm_read_word(&(playlist[track][i+1]));  //считываю длительность ноты

            if (Bit == 0)
            {
                pause();
            }
            else
            {
                play();
            }
        }

        stop();
    }
}
