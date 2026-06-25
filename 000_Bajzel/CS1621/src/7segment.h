#pragma once

#define normal 0
#define upside 1


#define PORT7SEG P1 
#define PORT7DIG P2
#define digits 4

//uint8_t seg7[]={A,B,C,D,E,F,G,P}; // don't edit/delete this assigment justify to segments
//_dataBIG uint8_t pivotSeg[2][8]={{0,1,2,3,4,5,6,7},{3,4,5,0,1,2,6,7}}; // segment bit order 0-normal, 1-upside)
uint8_t pivotSeg[2][8]={{0,1,2,3,4,5,6,7},{0,4,6,3,2,1,5,7}}; // segment bit order 0-normal, 1-upside)

uint8_t pivotDig[2][digits]={{7,6,5,4},{4,5,6,7}}; //digit pins order (0-normal, 1-upside)
uint8_t point7[digits]={0,0,0,0}; //points - commas

uint8_t glyph7[]=
{
//start from  ascii 32 code
    0b00000000, // [SPACE]
    0b01000001, // !
    0b01000100, // "
    0b01101100, // #
    0b10010110, // $
    0b10110100, // %
    0b11011110, // &
    0b00000100, // '
    0b10011100, // (
    0b11110000, // )
    0b11000110, // *
    0b00001110, // +
    0b00110000, // ,
    0b00000010, // -
    0b00000001, // .
    0b10000010, // /
    0b11111100, // 0
    0b01100000, // 1
    0b11011010, // 2
    0b11110010, // 3
    0b01100110, // 4
    0b10110110, // 5
    0b10111110, // 6
    0b11100100, // 7 opcje: 0b11100000, lub 0b11100010, lub 0b11100100,<- najlepszy bo bezkolizyjny i dobrze wygląda
    0b11111110, // 8
    0b11110110, // 9
    0b00010010, // :
    0b00110010, // ;
    0b10000110, // <
    0b10000010, // =
    0b11000010, // >
    0b11001011, // ? opcja bez kropki
    0b11011110, // @ or a 0b11111010, lepsza opcja dla @ 0b11011110,
    0b11101110, // A
    0b11111110, // B
    0b10011100, // C
    0b11111000, // D
    0b10011110, // E
    0b10001110, // F
    0b10111100, // G
    0b01101110, // H
    0b01100000, // I
    0b01111000, // J
    0b01011110, // K
    0b00011100, // L
    0b10101000, // M lub opcjonalnie 0b10101000, albo 0b11010100,,
    0b11101100, // N
    0b11111100, // O
    0b11001110, // P
    0b11010110, // Q
    0b11001100, // R
    0b10110110, // S
    0b11100000, // T
    0b01111100, // U
    0b01001110, // V opcjonalnie 0b01111100, lub 0b01110100,
    0b01111110, // W
    0b00100110, // X opcja tak jak H duze a H jak h male
    0b01110110, // Y
    0b11010010, // Z opcja jak 2
    0b10011100, // [
    0b00000110, // "\"
    0b11110000, // ]
    0b11000100, // ^
    0b00010000, // _
    0b00000100, // `
    0b11111010, // a
    0b00111110, // b
    0b00011010, // c
    0b01111010, // d
    0b11011110, // e
    0b10001110, // f
    0b11110110, // g
    0b00101110, // h
    0b00100000, // i
    0b01110000, // j
    0b10101110, // k
    0b00001100, // l
    0b00101000, // m lub opcjonalnie 0b00101000, lub 0b10101010,
    0b00101010, // n
    0b00111010, // o
    0b11001110, // p
    0b11100110, // q
    0b00001010, // r
    0b10110110, // s
    0b00011110, // t
    0b00111000, // u
    0b01000110, // v
    0b01010100, // w
    0b00100110, // x
    0b01110110, // y
    0b11010010, // z
    0b10011100, // { może dodać .
    0b00001100, // |
    0b11110000, // } może dodać .
    0b10000000, // ~ wyżej, by odróżnić od -
    0b10000110, // [DEL] dopełnienie do ASCII7 (127 znaków) - małe c górny indeks jako stopień celcjusza
};

uint8_t reorder(uint8_t octet,uint8_t bitorder[]) // reorder segmets bits to bitorder
{
uint8_t data=0;
uint8_t mask=0;
for (uint8_t i=0;i<8;i++){
mask=(octet>>(7-i)) & 1;
data|=(mask<<bitorder[i]);
}
return data;
}

// usage: PORT7SEG=~(reorder(glyph7[ascCode-32],pivotSeg[rotation7LED]) | (0x80 * !!point7[dig_disp]));
/*
if (ascCode>127)
    {
	PORT7SEG=~reorder(ascCode<<1,pivotSeg[rotation7LED]);
    }
    else
    {
	PORT7SEG=~(reorder(glyph7[ascCode-32],pivotSeg[rotation7LED]) | (0x80 * !!point7[dig_disp]));
    } 
    
    BIT_LOW(PORT7DIG,(pivotDig[rotation7LED][dig_disp]));

*/
