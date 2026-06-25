/*
 * debounce.h
 *
 * Created: 2014-02-15 15:20:48
 *  Author: Piotr
 */ 

#include <avr/io.h>

#ifndef _DEBOUNCE_H
#define _DEBOUNCE_H
//liczba obs³ugiwanych przycisków
#define NUM_BTNS 3
//minimalny czas dobouncingu
#define CLICK_MIN 5
//minimalny czas przytrzymania
#define HOLD_MIN 50
//powtarzanie po przytrzymaniu
#define HOLD_REP 15
extern uint8_t menu_read(void);

void menu_check(void);

void empty(void);//funkcja pusta - brak akcji
void (*click[NUM_BTNS+1])();//wskaŸniki na akcje w razie klikniêcia//={empty,empty,empty,empty,empty,empty}
void (*hold[NUM_BTNS+1])();//wskaŸniki na akcje w razie przytrzymania
void menu_init();//inicjalizacja menu

#endif