/*
 * 09_ATMEGA163L LEDS_7SEG.h
 *
 *  Stworzono: 2015 27 cze 2015
 *      Autor: D¹browski Tomasz
 *     Edycja: 20:33:49
 */

#ifndef ADDS_LEDS_7SEG_H_
#define ADDS_LEDS_7SEG_H_

#include <avr/pgmspace.h>

#define updown 0
#define DIGITS 4

/********** mapowanie pinów updown/inne konfiguracje np.
 * (clearbit do animacji, gdzie 0 oznacza czysty,1-segA, 2-segB, 4-segA)
 * Na jednym OCTET mo¿emy zapisaæ 32 konfiguracje 256/(8 kombinacji);
 * Konfiguracje: 0-default clearbit, 1-normal ASCI/dec/hex , 2- 180 degree rotate of 1), 3-31 - custom
 * Tworzymy wirtualn¹ tablicê w jednym OKTECIE , np uint8_t ARRAYLED;
 * oraz makro MAPLED(nr) (((nr)*8)
 *
 
  
 
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
 
 
 
 *
 *
 */



#if (updown==0)
#define SegA (1<<5)
#define SegB (1<<3)
#define SegC (1<<0)
#define SegD (1<<1)
#define SegE (1<<7)
#define SegF (1<<4)
#define SegG (1<<6)

#define Dig1 D,2
#define Dig2 B,3
#define Dig3 B,1
#define Dig4 B,2

#else
#define SegA (1<<1)
#define SegB (1<<7)
#define SegC (1<<4)
#define SegD (1<<5)
#define SegE (1<<3)
#define SegF (1<<0)
#define SegG (1<<6)

#define Dig3 D,2
#define Dig2 B,3
#define Dig1 B,1
#define Dig4 B,2

#endif




#define Dig_1(STAN) if (STAN==0) {HIGH(Dig1);} else {LOW(Dig1);}
#define Dig_2(STAN) if (STAN==0) {HIGH(Dig2);} else {LOW(Dig2);}
#define Dig_3(STAN) if (STAN==0) {HIGH(Dig3);} else {LOW(Dig3);}
#define Dig_4(STAN) if (STAN==0) {HIGH(Dig4);} else {LOW(Dig4);}




const uint8_t ZNAK_7SEG[] PROGMEM = {
		SegA|SegB|SegC|SegD|SegE|SegF,
		SegB|SegC,
		SegA|SegB|SegD|SegE|SegG,
		SegA|SegB|SegC|SegD|SegG,
		SegB|SegC|SegF|SegG,
		SegA|SegC|SegD|SegF|SegG,
		SegA|SegC|SegD|SegE|SegF|SegG,
		SegA|SegB|SegC,
		SegA|SegB|SegC|SegD|SegE|SegF|SegG,
		SegA|SegB|SegC|SegF|SegG

		/*
		 * 65	SegA|SegB|SegC|SegE|SegF|SegG    // kod 65 znak A
97	SegA|SegB|SegC|SegD|SegE|SegG    // kod 97 znak a
165	SegA|SegB|SegC|SegE|SegF|SegG|Dot    // kod 165 znak ¥
185	SegA|SegB|SegC|SegD|SegE|SegG|Dot    // kod 185 znak ¹
66	SegA|SegB|SegC|SegD|SegE|SegF|SegG|Dot    // kod 66 znak B
98	|SegC|SegD|SegE|SegF|SegG    // kod 98 znak b
67	SegA|SegD|SegE|SegF    // kod 67 znak C
99	|SegD|SegE|SegG    // kod 99 znak c
198	SegA|SegD|SegE|SegF|Dot    // kod 198 znak Æ
230	SegA|SegD|SegE|SegG    // kod 230 znak æ
68	SegA|SegB|SegC|SegD|SegE|SegF    // kod 68 znak D
100	|SegB|SegC|SegD|SegE|SegG    // kod 100 znak d
69	SegA|SegD|SegE|SegF|SegG    // kod 69 znak E
101	SegA|SegB|SegD|SegE|SegF|SegG    // kod 101 znak e
202	SegA|SegD|SegE|SegF|SegG|Dot    // kod 202 znak Ê
234	SegA|SegB|SegD|SegE|SegF|SegG|Dot    // kod 234 znak ê
70	SegA|SegE|SegF|SegG    // kod 70 znak F
102	SegA|SegE|SegF|SegG|Dot    // kod 102 znak f
71	SegA|SegC|SegD|SegE|SegF    // kod 71 znak G
103	SegA|SegB|SegC|SegD|SegF|SegG    // kod 103 znak g
72	|SegB|SegC|SegE|SegF|SegG    // kod 72 znak H
104	|SegC|SegE|SegF|SegG    // kod 104 znak h
73	|SegE|SegF    // kod 73 znak I
105	|SegE    // kod 105 znak i
74	SegA|SegB|SegC|SegD|SegE    // kod 74 znak J
106	|SegB|SegC|SegD    // kod 106 znak j
75	|SegB|SegC|SegE|SegF|SegG|Dot    // kod 75 znak K
107	|SegC|SegE|SegF|SegG|Dot    // kod 107 znak k
76	|SegD|SegE|SegF    // kod 76 znak L
108	|SegD|SegE    // kod 108 znak l
163	|SegD|SegE|SegF|Dot    // kod 163 znak £
179	|SegD|SegE|Dot    // kod 179 znak ³
77	SegA|SegB|SegC|SegD|SegG|Dot    // kod 77 znak M
109	SegA|SegC|SegE|SegG    // kod 109 znak m
78	SegA|SegB|SegC|SegE|SegF    // kod 78 znak N
110	|SegC|SegE|SegG    // kod 110 znak n
209	SegA|SegB|SegC|SegE|SegF|Dot    // kod 209 znak Ñ
241	|SegC|SegE|SegG|Dot    // kod 241 znak ñ
79	SegA|SegB|SegC|SegD|SegE|SegF    // kod 79 znak O
111	|SegC|SegD|SegE|SegG    // kod 111 znak o
211	SegA|SegB|SegC|SegD|SegE|SegF|Dot    // kod 211 znak Ó
243	|SegC|SegD|SegE|SegG|Dot    // kod 243 znak ó
80	SegA|SegB|SegE|SegF|SegG    // kod 80 znak P
112	SegA|SegB|SegE|SegF|SegG    // kod 112 znak p
81	SegA|SegB|SegC|SegF|SegG|Dot    // kod 81 znak Q
113	SegA|SegB|SegD|SegF|SegG    // kod 113 znak q
82	SegA|SegE|SegF    // kod 82 znak R
114	|SegE|SegG    // kod 114 znak r
83	SegA|SegC|SegD|SegF|SegG    // kod 83 znak S
115	|SegC|SegD|SegF|SegG    // kod 115 znak s
140	SegA|SegC|SegD|SegF|SegG|Dot    // kod 140 znak Œ
156	|SegC|SegD|SegF|SegG|Dot    // kod 156 znak œ
84	SegA|SegB|SegC    // kod 84 znak T
116	|SegD|SegE|SegF|SegG    // kod 116 znak t
85	|SegB|SegC|SegD|SegE|SegF    // kod 85 znak U
117	|SegC|SegD|SegE    // kod 117 znak u
86	|SegB|SegC|SegD|SegE|SegF|Dot    // kod 86 znak V
118	|SegC|SegD|SegE|Dot    // kod 118 znak v
87	|SegB|SegD|SegF|SegG    // kod 87 znak W
119	SegA|SegC|SegD|SegE    // kod 119 znak w
88	|SegB|SegC|SegE|SegF|SegG|Dot    // kod 88 znak X
120	SegA|SegD|SegG    // kod 120 znak x
89	|SegB|SegE|SegF|SegG    // kod 89 znak Y
121	|SegB|SegC|SegD|SegF|SegG    // kod 121 znak y
90	SegA|SegB|SegD|SegE|SegG    // kod 90 znak Z
122	|SegB|SegD|SegE|SegG    // kod 122 znak z
175	SegA|SegB|SegD|SegE|SegG|Dot    // kod 175 znak ¯
191	|SegB|SegD|SegE|SegG|Dot    // kod 191 znak ¿
43	|SegB|SegC|SegG|Dot    // kod 43 znak +
45	|SegG    // kod 45 znak -
42	SegA|SegB|SegF|SegG    // kod 42 znak *
47	|SegB|SegE|SegG    // kod 47 znak /
48	SegA|SegB|SegC|SegD|SegE|SegF    // kod 48 znak 0
49	|SegB|SegC    // kod 49 znak 1
50	SegA|SegB|SegD|SegE|SegG    // kod 50 znak 2
51	SegA|SegB|SegC|SegD|SegG    // kod 51 znak 3
52	|SegB|SegC|SegF|SegG    // kod 52 znak 4
53	SegA|SegC|SegD|SegF|SegG    // kod 53 znak 5
54	SegA|SegC|SegD|SegE|SegF|SegG    // kod 54 znak 6
55	SegA|SegB|SegC    // kod 55 znak 7
56	SegA|SegB|SegC|SegD|SegE|SegF|SegG    // kod 56 znak 8
57	SegA|SegB|SegC|SegF|SegG    // kod 57 znak 9
		 *
		 */

};

#endif /* ADDS_LEDS_7SEG_H_ */
