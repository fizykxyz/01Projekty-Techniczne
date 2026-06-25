/*
 * ATMEGA32L_LCD font_std_5x8.h
 *
 *  Stworzono: 2014 03-10-2014
 *      Autor: DÄ…browski Tomasz
 *     Edycja: 07:54:06
 */

#ifndef FONT_7SEG_H_
#define FONT_7SEG_H_

#include <avr/pgmspace.h>

#ifndef SegA







#endif


uint8_t FONT_7SEG[]  =
{0x05, 0x08, 0x20, 0x91,				  // size W,H(X,Y) ,first char, last char

		SegA|SegB|SegF|SegG,  //  kod 0x2A - 42 znak * dupa
		SegB|SegC|SegG|SegP,  //  kod 0x2B - 43 znak +
		SegG,  //  kod 0x2D - 45 znak - dupa
		SegB|SegE|SegG,  //  kod 0x2F - 47 znak / dupa
		SegA|SegB|SegC|SegD|SegE|SegF,  //  kod 0x30 - 48 znak 0
		SegB|SegC,  //  kod 0x31 - 49 znak 1
		SegA|SegB|SegD|SegE|SegG,  //  kod 0x32 - 50 znak 2
		SegA|SegB|SegC|SegD|SegG,  //  kod 0x33 - 51 znak 3
		SegB|SegC|SegF|SegG,  //  kod 0x34 - 52 znak 4
		SegA|SegC|SegD|SegF|SegG,  //  kod 0x35 - 53 znak 5
		SegA|SegC|SegD|SegE|SegF|SegG,  //  kod 0x36 - 54 znak 6
		SegA|SegB|SegC,  //  kod 0x37 - 55 znak 7
		SegA|SegB|SegC|SegD|SegE|SegF|SegG,  //  kod 0x38 - 56 znak 8
		SegA|SegB|SegC|SegF|SegG,  //  kod 0x39 - 57 znak 9
		SegA|SegB|SegC|SegD|SegE|SegF|SegG,  //  kod 0x41 - 65 znak A dupa
		SegA|SegB|SegC|SegD|SegE|SegF|SegG|SegP,  //  kod 0x42 - 66 znak B
		SegA|SegD|SegE|SegF,  //  kod 0x43 - 67 znak C
		SegA|SegB|SegC|SegD|SegE|SegF,  //  kod 0x44 - 68 znak D
		SegA|SegD|SegE|SegF|SegG,  //  kod 0x45 - 69 znak E
		SegA|SegE|SegF|SegG,  //  kod 0x46 - 70 znak F
		SegA|SegC|SegD|SegE|SegF,  //  kod 0x47 - 71 znak G
		SegB|SegC|SegE|SegF|SegG,  //  kod 0x48 - 72 znak H
		SegE|SegF,  //  kod 0x49 - 73 znak I
		SegA|SegB|SegC|SegD|SegE,  //  kod 0x4A - 74 znak J
		SegB|SegC|SegE|SegF|SegG|SegP,  //  kod 0x4B - 75 znak K
		SegD|SegE|SegF,  //  kod 0x4C - 76 znak L
		SegA|SegB|SegC|SegD|SegG|SegP,  //  kod 0x4D - 77 znak M
		SegA|SegB|SegC|SegE|SegF,  //  kod 0x4E - 78 znak N
		SegA|SegB|SegC|SegD|SegE|SegF,  //  kod 0x4F - 79 znak O
		SegA|SegB|SegE|SegF|SegG,  //  kod 0x50 - 80 znak P
		SegA|SegB|SegC|SegF|SegG|SegP,  //  kod 0x51 - 81 znak Q
		SegA|SegE|SegF,  //  kod 0x52 - 82 znak R
		SegA|SegC|SegD|SegF|SegG,  //  kod 0x53 - 83 znak S
		SegA|SegB|SegC,  //  kod 0x54 - 84 znak T
		SegB|SegC|SegD|SegE|SegF,  //  kod 0x55 - 85 znak U
		SegB|SegC|SegD|SegE|SegF|SegP,  //  kod 0x56 - 86 znak V
		SegB|SegD|SegF|SegG,  //  kod 0x57 - 87 znak W
		SegB|SegC|SegE|SegF|SegG|SegP,  //  kod 0x58 - 88 znak X
		SegB|SegE|SegF|SegG,  //  kod 0x59 - 89 znak Y
		SegA|SegB|SegD|SegE|SegG,  //  kod 0x5A - 90 znak Z
		SegA|SegB|SegC|SegD|SegE|SegG,  //  kod 0x61 - 97 znak a dupa
		SegC|SegD|SegE|SegF|SegG,  //  kod 0x62 - 98 znak b
		SegD|SegE|SegG,  //  kod 0x63 - 99 znak c
		SegB|SegC|SegD|SegE|SegG,  //  kod 0x64 - 100 znak d
		SegA|SegB|SegD|SegE|SegF|SegG,  //  kod 0x65 - 101 znak e
		SegA|SegE|SegF|SegG|SegP,  //  kod 0x66 - 102 znak f
		SegA|SegB|SegC|SegD|SegF|SegG,  //  kod 0x67 - 103 znak g
		SegC|SegE|SegF|SegG,  //  kod 0x68 - 104 znak h
		SegE,  //  kod 0x69 - 105 znak i
		SegB|SegC|SegD,  //  kod 0x6A - 106 znak j
		SegC|SegE|SegF|SegG|SegP,  //  kod 0x6B - 107 znak k
		SegD|SegE,  //  kod 0x6C - 108 znak l
		SegA|SegC|SegE|SegG,  //  kod 0x6D - 109 znak m
		SegC|SegE|SegG,  //  kod 0x6E - 110 znak n
		SegC|SegD|SegE|SegG,  //  kod 0x6F - 111 znak o
		SegA|SegB|SegE|SegF|SegG,  //  kod 0x70 - 112 znak p
		SegA|SegB|SegD|SegF|SegG,  //  kod 0x71 - 113 znak q
		SegE|SegG,  //  kod 0x72 - 114 znak r
		SegC|SegD|SegF|SegG,  //  kod 0x73 - 115 znak s
		SegD|SegE|SegF|SegG,  //  kod 0x74 - 116 znak t
		SegC|SegD|SegE,  //  kod 0x75 - 117 znak u
		SegC|SegD|SegE|SegP,  //  kod 0x76 - 118 znak v
		SegA|SegC|SegD|SegE,  //  kod 0x77 - 119 znak w
		SegA|SegD|SegG,  //  kod 0x78 - 120 znak x
		SegB|SegC|SegD|SegF|SegG,  //  kod 0x79 - 121 znak y
		SegB|SegD|SegE|SegG,  //  kod 0x7A - 122 znak z
		SegA|SegC|SegD|SegF|SegG|SegP,  //  kod 0x8C - 140 znak Ś dupa
		SegC|SegD|SegF|SegG|SegP,  //  kod 0x9C - 156 znak ś dupa
		SegD|SegE|SegF|SegP,  //  kod 0xA3 - 163 znak Ł dupa
		SegA|SegB|SegC|SegE|SegF|SegG|SegP,  //  kod 0xA5 - 165 znak Ą dupa
		SegA|SegB|SegD|SegE|SegG|SegP,  //  kod 0xAF - 175 znak Ż dupa
		SegD|SegE|SegP,  //  kod 0xB3 - 179 znak ł dupa
		SegA|SegB|SegC|SegD|SegE|SegG|SegP,  //  kod 0xB9 - 185 znak ą dupa
		SegB|SegD|SegE|SegG|SegP,  //  kod 0xBF - 191 znak ż dupa
		SegA|SegD|SegE|SegF|SegP,  //  kod 0xC6 - 198 znak Ć dupa
		SegA|SegD|SegE|SegF|SegG|SegP,  //  kod 0xCA - 202 znak Ę dupa
		SegA|SegB|SegC|SegE|SegF|SegP,  //  kod 0xD1 - 209 znak Ń dupa
		SegA|SegB|SegC|SegD|SegE|SegF|SegP,  //  kod 0xD3 - 211 znak Ó dupa
		SegA|SegD|SegE|SegG,  //  kod 0xE6 - 230 znak ć dupa
		SegA|SegB|SegD|SegE|SegF|SegG|SegP,  //  kod 0xEA - 234 znak ę dupa
		SegC|SegE|SegG|SegP,  //  kod 0xF1 - 241 znak ń dupa
		SegC|SegD|SegE|SegG|SegP,  //  kod 0xF3 - 243 znak ó dupa







//Polskie znaki od 128 po kolei
,0x7e, 0x11, 0x11, 0x91, 0x7e // 0x80 Ä„
,0x3c, 0x42, 0x46, 0x43, 0x24 // 0x81 Ä†
,0x7f, 0x49, 0x49, 0xc9, 0x41 // 0x82 Ä�
,0x7f, 0x48, 0x44, 0x40, 0x40 // 0x83 Ĺ�
,0x7e, 0x08, 0x12, 0x21, 0x7e // 0x84 Ĺ�
,0x3c, 0x42, 0x46, 0x43, 0x3c // 0x85 Ă“
,0x44, 0x4a, 0x4a, 0x4b, 0x32 // 0x86 Ĺš
,0x62, 0x56, 0x4b, 0x46, 0x42 // 0x87 Ĺą
,0x62, 0x52, 0x4b, 0x46, 0x42 // 0x88 Ĺ»
,0x20, 0x54, 0x54, 0xd4, 0x78 // 0x89 Ä…
,0x38, 0x44, 0x46, 0x45, 0x20 // 0x8a Ä‡
,0x38, 0x54, 0x54, 0xd4, 0x18 // 0x8b Ä™
,0x00, 0x51, 0x7f, 0x44, 0x00 // 0x8c Ĺ‚
,0x7c, 0x08, 0x06, 0x05, 0x78 // 0x8d Ĺ„
,0x38, 0x44, 0x46, 0x45, 0x38 // 0x8e Ăł
,0x48, 0x54, 0x56, 0x55, 0x20 // 0x8f Ĺ›
,0x44, 0x64, 0x56, 0x4d, 0x44 // 0x90 Ĺş
,0x44, 0x64, 0x55, 0x4c, 0x44 // 0x91 ĹĽ
};


#endif /* FONT_7SEG_H_ */
