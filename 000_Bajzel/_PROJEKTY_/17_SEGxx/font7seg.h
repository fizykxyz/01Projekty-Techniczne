/*
 * 000_XYZ_AVRLIBS pins.h
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 2016-08-09   
 *     Edycja: 08:57:02
 */

#ifndef ADDS_FONT7SEG_H_
#define ADDS_FONT7SEG_H_

//#include <avr/io.h>
#include <avr/pgmspace.h>

/*
enum Virt_seg {vSegA,vSegB,vSegC,vSegD,vSegE,vSegF,vSegG,vSeg_};

 #define sA (1 << vSegA)
 #define sB (1 << vSegB)
 #define sC (1 << vSegC)
 #define sD (1 << vSegD)
 #define sE (1 << vSegE)
 #define sF (1 << vSegF)
 #define sG (1 << vSegG)
 #define s_ (1 << vSeg_) // tu kropka
*/

#define startCode 32

const uint8_t font7seg[] PROGMEM = {
//order bit segments _GFEDCBA
0x00,  // code 0x20 (32) char '   '
0x82,  // code 0x21 (33) char ' ! '
0x22,  // code 0x22 (34) char ' " '
0x00,  // code 0x23 (35) char ' # ' (npr)
0x00,  // code 0x24 (36) char ' $ ' (npr)
0xD2,  // code 0x25 (37) char ' % '
0x00,  // code 0x26 (38) char ' & ' (npr)
0x02,  // code 0x27 (39) char ' ' '
0x39,  // code 0x28 (40) char ' ( '
0x0F,  // code 0x29 (41) char ' ) '
0x63,  // code 0x2A (42) char ' * '
0x46,  // code 0x2B (43) char ' + '
0x80,  // code 0x2C (44) char ' , '
0x40,  // code 0x2D (45) char ' - '
0x80,  // code 0x2E (46) char ' . '
0x52,  // code 0x2F (47) char ' / '
0x3F,  // code 0x30 (48) char ' 0 '
0x06,  // code 0x31 (49) char ' 1 '
0x5B,  // code 0x32 (50) char ' 2 '
0x4F,  // code 0x33 (51) char ' 3 '
0x66,  // code 0x34 (52) char ' 4 '
0x6D,  // code 0x35 (53) char ' 5 '
0x7D,  // code 0x36 (54) char ' 6 '
0x07,  // code 0x37 (55) char ' 7 '
0x7F,  // code 0x38 (56) char ' 8 '
0x67,  // code 0x39 (57) char ' 9 '
0x09,  // code 0x3A (58) char ' : '
0xC0,  // code 0x3B (59) char ' ; '
0x61,  // code 0x3C (60) char ' < '
0x41,  // code 0x3D (61) char ' = '
0x43,  // code 0x3E (62) char ' > '
0x53,  // code 0x3F (63) char ' ? '
0xDF,  // code 0x40 (64) char ' @ '
0x77,  // code 0x41 (65) char ' A '
0x7F,  // code 0x42 (66) char ' B '
0x39,  // code 0x43 (67) char ' C '
0x3F,  // code 0x44 (68) char ' D '
0x79,  // code 0x45 (69) char ' E '
0x71,  // code 0x46 (70) char ' F '
0x3D,  // code 0x47 (71) char ' G '
0x76,  // code 0x48 (72) char ' H '
0x06,  // code 0x49 (73) char ' I '
0x1E,  // code 0x4A (74) char ' J '
0x75,  // code 0x4B (75) char ' K '
0x38,  // code 0x4C (76) char ' L '
0x15,  // code 0x4D (77) char ' M '
0x37,  // code 0x4E (78) char ' N '
0x3F,  // code 0x4F (79) char ' O '
0x73,  // code 0x50 (80) char ' P '
0x6B,  // code 0x51 (81) char ' Q '
0x33,  // code 0x52 (82) char ' R '
0x6D,  // code 0x53 (83) char ' S '
0x78,  // code 0x54 (84) char ' T '
0x3E,  // code 0x55 (85) char ' U '
0x3E,  // code 0x56 (86) char ' V '
0x2A,  // code 0x57 (87) char ' W '
0x76,  // code 0x58 (88) char ' X '
0x6E,  // code 0x59 (89) char ' Y '
0x5B,  // code 0x5A (90) char ' Z '
0x39,  // code 0x5B (91) char ' [ '
0x64,  // code 0x5C (92) char ' \ '
0x0F,  // code 0x5D (93) char ' ] '
0x23,  // code 0x5E (94) char ' ^ '
0x08,  // code 0x5F (95) char ' _ '
0x20,  // code 0x60 (96) char ' ` '
0x5F,  // code 0x61 (97) char ' a '
0x7C,  // code 0x62 (98) char ' b '
0x58,  // code 0x63 (99) char ' c '
0x5E,  // code 0x64 (100) char ' d '
0x7B,  // code 0x65 (101) char ' e '
0x71,  // code 0x66 (102) char ' f '
0x6F,  // code 0x67 (103) char ' g '
0x74,  // code 0x68 (104) char ' h '
0x04,  // code 0x69 (105) char ' i '
0x0C,  // code 0x6A (106) char ' j '
0x75,  // code 0x6B (107) char ' k '
0x30,  // code 0x6C (108) char ' l '
0x15,  // code 0x6D (109) char ' m '
0x54,  // code 0x6E (110) char ' n '
0x5C,  // code 0x6F (111) char ' o '
0x73,  // code 0x70 (112) char ' p '
0x67,  // code 0x71 (113) char ' q '
0x50,  // code 0x72 (114) char ' r '
0x6D,  // code 0x73 (115) char ' s '
0x78,  // code 0x74 (116) char ' t '
0x1C,  // code 0x75 (117) char ' u '
0x1C,  // code 0x76 (118) char ' v '
0x2A,  // code 0x77 (119) char ' w '
0x76,  // code 0x78 (120) char ' x '
0x6E,  // code 0x79 (121) char ' y '
0x5B,  // code 0x7A (122) char ' z '
0x46,  // code 0x7B (123) char ' { '
0x30,  // code 0x7C (124) char ' | '
0x70,  // code 0x7D (125) char ' } '
0x40,  // code 0x7E (126) char ' ~ '
0x63,  // code 0x7F (127) char '  '
};







//*************************** KONIEC PLIKU NAG£ÓWKOWEGO ************




#endif /* ADDS_FONT7SEG_H_ */

