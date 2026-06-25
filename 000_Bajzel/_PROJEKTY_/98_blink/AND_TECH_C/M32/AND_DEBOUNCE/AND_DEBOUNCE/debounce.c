/*
 * debounce.c
 *
 * Created: 2014-02-15 15:20:38
 *  Author: Piotr
 */ 
#include "debounce.h"

void empty(void){};//funkcja pusta - brak akcji
//sprawdzanie menu w przerwaniu
void menu_check(void){
	static uint8_t kbd_state=0;
	static uint8_t kbd_time=0;
	static uint8_t last_kbd;
	uint8_t joy=menu_read();//odczytujemy wciœniêty przycisk
	if(kbd_time)kbd_time--;//jeœli trzeba odmierzaæ czas to go odmierzamy
	if(joy==0){//jeœli joystick puszczono
		if(kbd_state==2){//i by³ odmierzony czas klikniêcia
			click[last_kbd]();//to wywo³ujemy funkcjê klikniêcia
		}
		kbd_state=0;//i resetujemy system
		}else{//w przeciwnym wypadku
		if(kbd_state==0){//jeœli to nowe przyciœniêcie
			kbd_state=1;//przechodzimy do fazy redukowania drgañ styków
			kbd_time=CLICK_MIN;//ustawiamy czas redukcji drgañ
			last_kbd=joy;//i zapamiêtujemy aktualnie wciœniêty przycisk
			}else if(kbd_state==1){//jeœli jesteœmy w trybie eleminiacji drgañ styków
			if(last_kbd==joy){//jeœli nadal ten sam przycisk
				if(kbd_time==0){//jeœli ju¿ up³yn¹³ ca³y czas
					kbd_state=2;//to przechodzimy do opcji wyzwalania klikniêcia i odmierzenia czasu trzymania
					kbd_time=HOLD_MIN;//ustawiamy czas przytrzymania
				}
				}else{//jeœli zmieni³ siê przycisk
				kbd_state=0;//to resetujemy system
			}
			}else if(kbd_state==2){//jeœli oczekujemy na przytrzymanie
			if(last_kbd==joy){//jeœli nadal ten sam przycisk
				if(kbd_time==0){//jeœli ju¿ up³yn¹³ ca³y czas
					kbd_state=3;//to przechodzimy do opcji powtarzania klikniêæ
					kbd_time=HOLD_REP;//ustawiamy czas przytrzymania
					hold[last_kbd]();//wywo³ujemy funkcjê przytrzymania
				}
				}else{//jeœli zmieni³ siê przycisk
				kbd_state=0;//to resetujemy system
			}
			}else if(kbd_state==3){
			if(last_kbd==joy){//jeœli nadal ten sam przycisk
				if(kbd_time==0){//jeœli ju¿ up³yn¹³ ca³y czas
					kbd_state=3;//to przechodzimy do opcji powtarzania klikniêæ
					kbd_time=HOLD_REP;//ustawiamy czas przytrzymania
					hold[last_kbd]();//wywo³ujemy funkcjê przytrzymania
				}
				}else{//jeœli zmieni³ siê przycisk
				kbd_state=0;//to resetujemy system
			}
		}
	}
}

void menu_init(){
	for(uint8_t i=0;i<=NUM_BTNS;i++){
		hold[i]=empty;
		click[i]=empty;
	}
}