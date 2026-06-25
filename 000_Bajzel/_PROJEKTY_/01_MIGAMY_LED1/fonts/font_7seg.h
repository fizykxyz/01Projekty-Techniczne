/*
 * ATMEGA32L_LCD font_std_5x8.h
 *
 *  Stworzono: 2014 03-10-2014
 *      Autor: DĂ„â€¦browski Tomasz
 *     Edycja: 07:54:06
 */

#ifndef FONT_7SEG_H_
#define FONT_7SEG_H_

#include <avr/pgmspace.h>


enum Virt_seg {vSegA,vSegB,vSegC,vSegD,vSegE,vSegF,vSegG,vSegH};
enum Real_seg {rSegA,rSegB,rSegC,rSegD,rSegE,rSegF,rSegG,rSegH}; //normal
//enum Real_seg {rSegD,rSegE,rSegF,rSegA,rSegB,rSegC,rSegG,rSegH}; //upsidedown

#define Seg(W_segment) (1<<(W_segment))


uint8_t FONT_7SEG[]  =
{0x01, 0x01, 0x20, 0x7F,				  // size W,H(X,Y) ,first char, last char

		0x00,  // znak   kod 0x20 - 32
		Seg(rSegB)|Seg(rSegH),  // znak ! kod 0x21 - 33
		Seg(rSegB)|Seg(rSegF),  // znak " kod 0x22 - 34
		0x00,  // znak # kod 0x23 - 35
		0x00,  // znak $ kod 0x24 - 36
		Seg(rSegB)|Seg(rSegE)|Seg(rSegG)|Seg(rSegH),  // znak % kod 0x25 - 37
		0x00,  // znak & kod 0x26 - 38
		Seg(rSegB),  // znak ' kod 0x27 - 39
		Seg(rSegA)|Seg(rSegD)|Seg(rSegE)|Seg(rSegF),  // znak ( kod 0x28 - 40
		Seg(rSegA)|Seg(rSegB)|Seg(rSegC)|Seg(rSegD),  // znak ) kod 0x29 - 41
		Seg(rSegA)|Seg(rSegB)|Seg(rSegF)|Seg(rSegG),  // znak * kod 0x2A - 42
		Seg(rSegB)|Seg(rSegC)|Seg(rSegG),  // znak + kod 0x2B - 43
		Seg(rSegH),  // znak , kod 0x2C - 44
		Seg(rSegG),  // znak - kod 0x2D - 45
		Seg(rSegH),  // znak . kod 0x2E - 46
		Seg(rSegB)|Seg(rSegE)|Seg(rSegG),  // znak / kod 0x2F - 47
		Seg(rSegA)|Seg(rSegB)|Seg(rSegC)|Seg(rSegD)|Seg(rSegE)|Seg(rSegF),  // znak 0 kod 0x30 - 48
		Seg(rSegB)|Seg(rSegC),  // znak 1 kod 0x31 - 49
		Seg(rSegA)|Seg(rSegB)|Seg(rSegD)|Seg(rSegE)|Seg(rSegG),  // znak 2 kod 0x32 - 50
		Seg(rSegA)|Seg(rSegB)|Seg(rSegC)|Seg(rSegD)|Seg(rSegG),  // znak 3 kod 0x33 - 51
		Seg(rSegB)|Seg(rSegC)|Seg(rSegF)|Seg(rSegG),  // znak 4 kod 0x34 - 52
		Seg(rSegA)|Seg(rSegC)|Seg(rSegD)|Seg(rSegF)|Seg(rSegG),  // znak 5 kod 0x35 - 53
		Seg(rSegA)|Seg(rSegC)|Seg(rSegD)|Seg(rSegE)|Seg(rSegF)|Seg(rSegG),  // znak 6 kod 0x36 - 54
		Seg(rSegA)|Seg(rSegB)|Seg(rSegC),  // znak 7 kod 0x37 - 55
		Seg(rSegA)|Seg(rSegB)|Seg(rSegC)|Seg(rSegD)|Seg(rSegE)|Seg(rSegF)|Seg(rSegG),  // znak 8 kod 0x38 - 56
		Seg(rSegA)|Seg(rSegB)|Seg(rSegC)|Seg(rSegF)|Seg(rSegG),  // znak 9 kod 0x39 - 57
		Seg(rSegA)|Seg(rSegD),  // znak : kod 0x3A - 58
		Seg(rSegG)|Seg(rSegH),  // znak ; kod 0x3B - 59
		Seg(rSegA)|Seg(rSegF)|Seg(rSegG),  // znak < kod 0x3C - 60
		Seg(rSegA)|Seg(rSegG),  // znak = kod 0x3D - 61
		Seg(rSegA)|Seg(rSegB)|Seg(rSegG),  // znak > kod 0x3E - 62
		Seg(rSegA)|Seg(rSegB)|Seg(rSegE)|Seg(rSegG),  // znak ? kod 0x3F - 63
		0x00,  // znak @ kod 0x40 - 64
		Seg(rSegA)|Seg(rSegB)|Seg(rSegC)|Seg(rSegE)|Seg(rSegF)|Seg(rSegG),  // znak A kod 0x41 - 65
		Seg(rSegA)|Seg(rSegB)|Seg(rSegC)|Seg(rSegD)|Seg(rSegE)|Seg(rSegF)|Seg(rSegG),  // znak B kod 0x42 - 66
		Seg(rSegA)|Seg(rSegD)|Seg(rSegE)|Seg(rSegF),  // znak C kod 0x43 - 67
		Seg(rSegA)|Seg(rSegB)|Seg(rSegC)|Seg(rSegD)|Seg(rSegE)|Seg(rSegF),  // znak D kod 0x44 - 68
		Seg(rSegA)|Seg(rSegD)|Seg(rSegE)|Seg(rSegF)|Seg(rSegG),  // znak E kod 0x45 - 69
		Seg(rSegA)|Seg(rSegE)|Seg(rSegF)|Seg(rSegG),  // znak F kod 0x46 - 70
		Seg(rSegA)|Seg(rSegC)|Seg(rSegD)|Seg(rSegE)|Seg(rSegF),  // znak G kod 0x47 - 71
		Seg(rSegB)|Seg(rSegC)|Seg(rSegE)|Seg(rSegF)|Seg(rSegG),  // znak H kod 0x48 - 72
		Seg(rSegB)|Seg(rSegC),  // znak I kod 0x49 - 73
		Seg(rSegB)|Seg(rSegC)|Seg(rSegD),  // znak J kod 0x4A - 74
		Seg(rSegA)|Seg(rSegC)|Seg(rSegE)|Seg(rSegF)|Seg(rSegG),  // znak K kod 0x4B - 75
		Seg(rSegD)|Seg(rSegE)|Seg(rSegF),  // znak L kod 0x4C - 76
		Seg(rSegA)|Seg(rSegC)|Seg(rSegE),  // znak M kod 0x4D - 77
		Seg(rSegA)|Seg(rSegB)|Seg(rSegC)|Seg(rSegE)|Seg(rSegF),  // znak N kod 0x4E - 78
		Seg(rSegA)|Seg(rSegB)|Seg(rSegC)|Seg(rSegD)|Seg(rSegE)|Seg(rSegF),  // znak O kod 0x4F - 79
		Seg(rSegA)|Seg(rSegB)|Seg(rSegE)|Seg(rSegF)|Seg(rSegG),  // znak P kod 0x50 - 80
		Seg(rSegA)|Seg(rSegB)|Seg(rSegC)|Seg(rSegD)|Seg(rSegE)|Seg(rSegF)|Seg(rSegH),  // znak Q kod 0x51 - 81
		Seg(rSegA)|Seg(rSegE)|Seg(rSegF),  // znak R kod 0x52 - 82
		Seg(rSegA)|Seg(rSegC)|Seg(rSegD)|Seg(rSegF)|Seg(rSegG),  // znak S kod 0x53 - 83
		Seg(rSegA)|Seg(rSegE)|Seg(rSegF),  // znak T kod 0x54 - 84
		Seg(rSegB)|Seg(rSegC)|Seg(rSegD)|Seg(rSegE)|Seg(rSegF),  // znak U kod 0x55 - 85
		Seg(rSegB)|Seg(rSegD)|Seg(rSegF),  // znak V kod 0x56 - 86
		Seg(rSegB)|Seg(rSegD)|Seg(rSegF)|Seg(rSegG),  // znak W kod 0x57 - 87
		Seg(rSegB)|Seg(rSegC)|Seg(rSegE)|Seg(rSegF),  // znak X kod 0x58 - 88
		Seg(rSegB)|Seg(rSegE)|Seg(rSegF)|Seg(rSegG),  // znak Y kod 0x59 - 89
		Seg(rSegA)|Seg(rSegB)|Seg(rSegD)|Seg(rSegE)|Seg(rSegG),  // znak Z kod 0x5A - 90
		Seg(rSegA)|Seg(rSegD)|Seg(rSegE)|Seg(rSegF),  // znak [ kod 0x5B - 91
		Seg(rSegC)|Seg(rSegF)|Seg(rSegG),  // znak \ kod 0x5C - 92
		Seg(rSegA)|Seg(rSegB)|Seg(rSegC)|Seg(rSegD),  // znak ] kod 0x5D - 93
		Seg(rSegA)|Seg(rSegB),  // znak ^ kod 0x5E - 94
		Seg(rSegD),  // znak _ kod 0x5F - 95
		Seg(rSegF),  // znak ` kod 0x60 - 96
		Seg(rSegA)|Seg(rSegB)|Seg(rSegC)|Seg(rSegD)|Seg(rSegE)|Seg(rSegG),  // znak a kod 0x61 - 97
		Seg(rSegC)|Seg(rSegD)|Seg(rSegE)|Seg(rSegF)|Seg(rSegG),  // znak b kod 0x62 - 98
		Seg(rSegD)|Seg(rSegE)|Seg(rSegG),  // znak c kod 0x63 - 99
		Seg(rSegB)|Seg(rSegC)|Seg(rSegD)|Seg(rSegE)|Seg(rSegG),  // znak d kod 0x64 - 100
		Seg(rSegA)|Seg(rSegB)|Seg(rSegD)|Seg(rSegE)|Seg(rSegF)|Seg(rSegG),  // znak e kod 0x65 - 101
		Seg(rSegA)|Seg(rSegE)|Seg(rSegF)|Seg(rSegG),  // znak f kod 0x66 - 102
		Seg(rSegA)|Seg(rSegB)|Seg(rSegC)|Seg(rSegD)|Seg(rSegF)|Seg(rSegG),  // znak g kod 0x67 - 103
		Seg(rSegC)|Seg(rSegE)|Seg(rSegF)|Seg(rSegG),  // znak h kod 0x68 - 104
		Seg(rSegC),  // znak i kod 0x69 - 105
		Seg(rSegC)|Seg(rSegD),  // znak j kod 0x6A - 106
		Seg(rSegA)|Seg(rSegC)|Seg(rSegE)|Seg(rSegF)|Seg(rSegG),  // znak k kod 0x6B - 107
		Seg(rSegE)|Seg(rSegF),  // znak l kod 0x6C - 108
		Seg(rSegA)|Seg(rSegC)|Seg(rSegE),  // znak m kod 0x6D - 109
		Seg(rSegC)|Seg(rSegE)|Seg(rSegG),  // znak n kod 0x6E - 110
		Seg(rSegC)|Seg(rSegD)|Seg(rSegE)|Seg(rSegG),  // znak o kod 0x6F - 111
		Seg(rSegA)|Seg(rSegB)|Seg(rSegE)|Seg(rSegF)|Seg(rSegG),  // znak p kod 0x70 - 112
		Seg(rSegC)|Seg(rSegD)|Seg(rSegE)|Seg(rSegG)|Seg(rSegH),  // znak q kod 0x71 - 113
		Seg(rSegE)|Seg(rSegG),  // znak r kod 0x72 - 114
		Seg(rSegA)|Seg(rSegC)|Seg(rSegD)|Seg(rSegF)|Seg(rSegG),  // znak s kod 0x73 - 115
		Seg(rSegD)|Seg(rSegE)|Seg(rSegF)|Seg(rSegG),  // znak t kod 0x74 - 116
		Seg(rSegC)|Seg(rSegD)|Seg(rSegE),  // znak u kod 0x75 - 117
		Seg(rSegB)|Seg(rSegD)|Seg(rSegF),  // znak v kod 0x76 - 118
		Seg(rSegB)|Seg(rSegD)|Seg(rSegF)|Seg(rSegG),  // znak w kod 0x77 - 119
		Seg(rSegB)|Seg(rSegC)|Seg(rSegE)|Seg(rSegF)|Seg(rSegG),  // znak x kod 0x78 - 120
		Seg(rSegB)|Seg(rSegC)|Seg(rSegD)|Seg(rSegF)|Seg(rSegG),  // znak y kod 0x79 - 121
		Seg(rSegA)|Seg(rSegB)|Seg(rSegD)|Seg(rSegE)|Seg(rSegG),  // znak z kod 0x7A - 122
		Seg(rSegA)|Seg(rSegD)|Seg(rSegE)|Seg(rSegF)|Seg(rSegH),  // znak { kod 0x7B - 123
		Seg(rSegE)|Seg(rSegF),  // znak | kod 0x7C - 124
		Seg(rSegA)|Seg(rSegB)|Seg(rSegC)|Seg(rSegD)|Seg(rSegH),  // znak } kod 0x7D - 125
		Seg(rSegG)|Seg(rSegH),  // znak ~ kod 0x7E - 126
		Seg(rSegA)|Seg(rSegB)|Seg(rSegF)|Seg(rSegG)  // znak  kod 0x7F - 127


};


#endif /* FONT_7SEG_H_ */
