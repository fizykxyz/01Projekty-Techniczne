/*
 * ATMEGA32L_LCD w7seg.h
 *
 *  Stworzono: 2014 24-09-2014
 *      Autor: D¹browski Tomasz
 *     Edycja: 08:19:40
 */

 
 
 /*
char readKey() {
  for(int x = 0; x < NUM_ROWS; x++) {
    for(int row = 0; row < NUM_ROWS; row++) digitalWrite(ROWS[row], x != row);
    for(int y = 0; y < NUM_COLS; y++) {
      if(!digitalRead(COLS[y])) return KEYS[x][y];
    }
  }
  return 0;
}
Jak widaæ nic skomplikowanego. Iterujemy po wszystkich wierszach oraz w ka¿dej iteracji wiersza iterujemy po kolumnach odczytuj¹c ich stany. Przed sprawdzeniem kolumn musimy zapewniæ, ¿e tylko jeden wiersz bêdzie w stanie NISKIM, a inne w stanie WYSOKIM. Ca³¹ robotê robi tu dla nas pêtla

    for(int row = 0; row < NUM_ROWS; row++) digitalWrite(ROWS[row], x != row);
W ka¿dej jej iteracji x jest niezmienne, a zmienna row zmienia siê od 0 do NUM_ROWS-1. Z tego te¿ powodu po wykonaniu siê wszystkich iteracji tej pêtli warunek logiczny x != row bêdzie fa³szem tylko raz (wtedy, kiedy x bêdzie równe row), wiêc tylko jeden wiersz ustawi nam siê w stan NISKI.
 
 */
 
 
 
 
#ifndef W7SEG_H_
#define W7SEG_H_


char* BUF_7SEG="STOP"; //bufor wyœwietlacza - iloœæ znaków
volatile uint8_t segm[]={2,3,4,5};
#define SEG7DDR DDRC
#define SEG7PORT PORTC
#define SEG7DDRSEG DDRD
#define SEG7PORTSEG PORTD


const uint8_t ZNAK_7SEG[] PROGMEM = {
		0x63,  //kod 42 znak *
		0xC6,  //kod 43 znak +
		0x00, //kod 44 znak pusty
		0x40,  //kod 45 znak -
		0x00, //kod 46 znak pusty
		0x52,  //kod 47 znak /
		0x3F,  //kod 48 znak 0
		0x06,  //kod 49 znak 1
		0x5B,  //kod 50 znak 2
		0x4F,  //kod 51 znak 3
		0x66,  //kod 52 znak 4
		0x6D,  //kod 53 znak 5
		0x7D,  //kod 54 znak 6
		0x07,  //kod 55 znak 7
		0x7F,  //kod 56 znak 8
		0x67,  //kod 57 znak 9
		0x00, //kod 58 znak pusty
		0x00, //kod 59 znak pusty
		0x00, //kod 60 znak pusty
		0b01000001, //kod 61 =
		0x00, //kod 62 znak pusty
		0x00, //kod 63 znak pusty
		0x00, //kod 64 znak pusty
		0x77,  //kod 65 znak A
		0xFF,  //kod 66 znak B
		0x39,  //kod 67 znak C
		0x3F,  //kod 68 znak D
		0x79,  //kod 69 znak E
		0x71,  //kod 70 znak F
		0x3D,  //kod 71 znak G
		0x76,  //kod 72 znak H
		0x30,  //kod 73 znak I
		0x1F,  //kod 74 znak J
		0xF6,  //kod 75 znak K
		0x38,  //kod 76 znak L
		0xCF,  //kod 77 znak M
		0x37,  //kod 78 znak N
		0x3F,  //kod 79 znak O
		0x73,  //kod 80 znak P
		0xE7,  //kod 81 znak Q
		0x31,  //kod 82 znak R
		0x6D,  //kod 83 znak S
		0x07,  //kod 84 znak T
		0x3E,  //kod 85 znak U
		0xBE,  //kod 86 znak V
		0x6A,  //kod 87 znak W
		0xF6,  //kod 88 znak X
		0x72,  //kod 89 znak Y
		0x5B,  //kod 90 znak Z
		0x00, //kod 91 znak pusty
		0x00, //kod 92 znak pusty
		0x00, //kod 93 znak pusty
		0x00, //kod 94 znak pusty
		0x00, //kod 95 znak pusty
		0x00, //kod 96 znak pusty
		0x5F,  //kod 97 znak a
		0x7C,  //kod 98 znak b
		0x58,  //kod 99 znak c
		0x5E,  //kod 100 znak d
		0x7B,  //kod 101 znak e
		0xF1,  //kod 102 znak f
		0x6F,  //kod 103 znak g
		0x74,  //kod 104 znak h
		0x10,  //kod 105 znak i
		0x0E,  //kod 106 znak j
		0xF4,  //kod 107 znak k
		0x18,  //kod 108 znak l
		0x55,  //kod 109 znak m
		0x54,  //kod 110 znak n
		0x5C,  //kod 111 znak o
		0x73,  //kod 112 znak p
		0x6B,  //kod 113 znak q
		0x50,  //kod 114 znak r
		0x6C,  //kod 115 znak s
		0x78,  //kod 116 znak t
		0x1C,  //kod 117 znak u
		0x9C,  //kod 118 znak v
		0x1D,  //kod 119 znak w
		0x49,  //kod 120 znak x
		0x6E,  //kod 121 znak y
		0x5A,  //kod 122 znak z
		0x5A,  //kod 123 znak z
		0x5A,  //kod 124 znak z
		0x5A,  //kod 125 znak z
		0b01000010,  //kod 126 znak up
		0b01000100  //kod 127 znak down

};


#endif /* W7SEG_H_ */
