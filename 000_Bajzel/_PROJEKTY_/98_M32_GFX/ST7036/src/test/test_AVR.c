/****************************************************************************
* Biblioteka obslugi wyswietlacza alfanumerycznego, opartego na sterowniku
* ST7036 lub odpowiedniku.
* 
* Program testowy dla platformy AVR.
*
* Nazwa pliku: test_AVR.c
* Data: 16.10.2007
* Wersja: 1.0
*
* Copyright (C) 2007 by Rafal Baranowski.
*
* Niniejsze oprogramowanie udostepniane jest nieodplatnie i moze byc
* powielane, modyfikowane i wykorzystywane w celach komercyjnych pod
* warunkiem zachowania noty copyright.
* Na poprawne dzialanie i bezpieczenstwo tego oprogramowania nie jest
* udzalana zadna gwarancja.
****************************************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <ST7036.h>
#include <platf_conf.h>
#include <disp_conf.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>

#define NO_OF_TEST 16

#define KEYB_PORT PORTC
#define KEYB_DDR DDRC
#define KEYB_PIN PINC
#define KEY_0 7
#define KEY_1 6
#define KEY_2 5

#define LEDS_PORT PORTC
#define LEDS_DDR DDRC
#define LED_0 0
#define LED_1 1

#define set_led0() { LEDS_PORT |= (1<<LED_0); }
#define set_led1() { LEDS_PORT |= (1<<LED_1); }
#define rst_led0() { LEDS_PORT &= ~(1<<LED_0); }
#define rst_led1() { LEDS_PORT &= ~(1<<LED_1); }
#define get_key0() (!(KEYB_PIN & (1<<KEY_0)))
#define get_key1() (!(KEYB_PIN & (1<<KEY_1))) 
#define get_key2() (!(KEYB_PIN & (1<<KEY_2))) 

const char PROGMEM intro[] = "Test ST7036";
const char PROGMEM end[] = " Test end";
const uint8_t PROGMEM pl_charset[] = PL_CHARSET;
uint8_t pl_charset_mem[] = PL_CHARSET;

volatile uint8_t test_no = 0;

void wait_ms(int ms) {
  while(ms--)
    _delay_loop_2((uint16_t)(65535/(262.14/(F_CPU/1000000UL))));
}

void wait_key0(void) {
  while(!get_key0());
  wait_ms(10);
  while(get_key0());
  wait_ms(10);
}
    
void wait_key1(void) {
  while(!get_key1());
  wait_ms(10);
  while(get_key1());
  wait_ms(10);
}

void wait_key2(void) {
  while(!get_key2());
  wait_ms(10);
  while(get_key2());
  wait_ms(10);
}

uint8_t chk_keyb(void) {
  if (get_key0()) {
    
    if (test_no-- == 0)
      test_no = NO_OF_TEST;
    
    wait_key0();
    return 1;
  }
  
  if (get_key1()) {
    
    if (test_no++ == NO_OF_TEST)
      test_no = 0;
      
    wait_key1();
    return 1;
  }
  
  if (get_key2()) {
    wait_key2();
    return 2;
  }
  
  return 0;
}

uint8_t def_ini(char* string) {
  char num[3];
  uint8_t result;
  
  Disp_set_update(IMMEDIATELY);
  Disp_clr_txt();
  itoa(test_no, num, 10);
  Disp_putc('T');
  Disp_puts(num);
  Disp_putc(' ');
  Disp_puts(string);
  while(1) {
    result = chk_keyb();
    if (result)
      return result;
  }
}

void def_end(void) {
  char num[3];
  
  Disp_set_update(IMMEDIATELY);
  Disp_clr_txt();
  itoa(test_no, num, 10);
  Disp_putc('T');
  Disp_puts(num);
  Disp_puts_const(end);
  while(!chk_keyb());
}

int main(void) {
  
  char char_str[1];
  char long_str[10];
  uint8_t str64[64];
  uint8_t temp;
  
  LEDS_DDR |= (1<<LED_0)|(1<<LED_1);
  KEYB_PORT |= (1<<KEY_0)|(1<<KEY_1)|(1<<KEY_2);
  
  Disp_init();
  Disp_set_update(IMMEDIATELY);
  
  Disp_puts_const(intro);
  
  while(!chk_keyb());
  
  test_no = 0;
  
  while(1) {
    switch(test_no) {
      /*********************************************/
      case 0:
        if (def_ini("Init test") == 1)
          break;
        
        Disp_init();
        Disp_puts("Init 1");
        wait_ms(300);
        
        Disp_init();
        Disp_puts("Init 2");
        wait_ms(300);
          
        Disp_init();
        Disp_puts("Init 3");
        wait_ms(300);
                  
        Disp_init();
        Disp_puts("Init 4");
        wait_ms(300);
                  
        Disp_init();
        Disp_puts("Init 5");
        
        def_end();
        break;
      /*********************************************/
      case 1:
        if (def_ini("Disp on-off") == 1)
          break;
        
        Disp_set(OFF);
        wait_key2();
        Disp_set(ON);
        
        def_end();
        break;
      /*********************************************/
      case 2:
        if (def_ini("Cursor test") == 1)
          break;
        
        Disp_set_cxy(0,1);
        Disp_puts("off");
        Disp_set_cur(OFF, NO_BLINK);
        wait_key2();
        
        Disp_set_cxy(0,1);
        Disp_puts("on ");
        Disp_set_cur(ON, NO_BLINK);
        wait_key2();
        
        Disp_set_cxy(0,1);
        Disp_puts("blink");
        Disp_set_cur(ON, BLINK);
        wait_key2();
        
        def_end();
        break;
      /*********************************************/
      case 3:
        if (def_ini("INC/DEC test") == 1)
          break;
        
        Disp_set_input_mode(INC);
        Disp_set_cxy(0,1);
        Disp_puts("Test");
        wait_key2();
        
        Disp_set_input_mode(DEC);
        Disp_set_cxy(3,1);
        Disp_puts("Test");
        wait_key2();
        
        def_end();
        break;
      /*********************************************/
      case 4:
        if (def_ini("Buffer test") == 1)
          break;
        
        Disp_set_update(DEFERRED);
        Disp_clr_txt();
        Disp_puts("click!");
        wait_key2();
        
        Disp_update();
        wait_key2();
        
        Disp_clr_txt();
        Disp_puts("1234567890Test");        
        wait_key2();
        
        if (Disp_get_ovf()) {
          Disp_flush_buffer();
          Disp_clr_txt();
          Disp_puts("Buf ovf");
        }
          
        Disp_update();
        wait_key2();
        
        Disp_clr_txt();
        Disp_puts("Flush");
        Disp_update();
        wait_key2();
        
        Disp_clr_txt();
        Disp_puts("OK");
        Disp_update();
        Disp_clr_txt();
        Disp_puts("error");
        Disp_flush_buffer();
        Disp_update();
        wait_key2();
        
        Disp_set_update(IMMEDIATELY);
        Disp_clr_txt();
        Disp_puts("Immediate txt");
        wait_key2();
        
        def_end();
        break;
      /*********************************************/
      case 5:
        if (def_ini("Line test") == 1)
          break;
        
        Disp_clr_txt();
        Disp_set_cxy(2,0);
        Disp_puts("Line 1");
        #if DISP_C_ROWS_NUM >= 2 
          Disp_set_cxy(3,1);
          Disp_puts("Line 2");
        #endif
        #if DISP_C_ROWS_NUM >= 3 
          Disp_set_cxy(4,2);
          Disp_puts("Line 3");
        #endif
        
        wait_key2();
        
        def_end();
        break;
      /*********************************************/
      case 6:
        if (def_ini("Cur pos test") == 1)
          break;
        
        Disp_set_input_mode(INC);
        Disp_clr_txt();
        
        Disp_puts("2=");
        itoa(Disp_get_cx(), char_str, 10);
        Disp_puts(char_str);
        wait_key2();
        Disp_set_cxy(2,0);
        itoa(Disp_get_cx(), char_str, 10);
        Disp_puts(char_str);
        Disp_puts("  ");
        
        Disp_set_cxy(0,1);
        Disp_puts("1=");
        itoa(Disp_get_cy(), char_str, 10);
        Disp_puts(char_str);
        wait_key2();
        Disp_set_cxy(2,1);
        itoa(Disp_get_cy(), char_str, 10);
        Disp_puts(char_str);
        
        Disp_set_input_mode(DEC);
        Disp_set_cxy(10,0);
        Disp_puts("8=");
        itoa(Disp_get_cx(), char_str, 10);
        Disp_puts(char_str);
        wait_key2();
        Disp_set_cxy(8,0);
        itoa(Disp_get_cx(), char_str, 10);
        Disp_puts(char_str);
        wait_key2();
        
        def_end();
        break;
      /*********************************************/
      case 7:
        if (def_ini("Move cur/win") == 1)
          break;
        
        Disp_clr();
        Disp_puts("right");
        Disp_move_txt_cur(RIGHT);
        Disp_putc('!');
        
        Disp_set_cxy(0,1);
        Disp_puts("left ");
        Disp_move_txt_cur(LEFT);
        Disp_putc('!');
                
        wait_key2();
        
        for (int i=0; i<3; i++) {
          Disp_move_txt_window(RIGHT);
          wait_ms(300);
        }
        
        for (int i=0; i<3; i++) {
          Disp_move_txt_window(LEFT);
          wait_ms(300);
        }

        def_end();
        break;
      /*********************************************/
      case 8:
        if (def_ini("Clear test") == 1)
          break;
          
        for (int i=0; i<4; i++) {
          Disp_set_cxy(0,i);
          for (int j=0; j<40; j++)
            Disp_putc('X');
        }
        
        wait_key2();
        Disp_clr();
        Disp_puts("Cleared.");
        wait_key2();

        def_end();
        break;
      /*********************************************/
      case 9:
        if (def_ini("Cur ret test") == 1)
          break;
        
        Disp_clr();
        Disp_set_cur(ON, NO_BLINK);
        
        Disp_ret_cur();
        wait_key2();
        
        Disp_set_cxy(10,0);
        Disp_ret_cur();
        wait_key2();
        
        Disp_set_cxy(10,1);
        Disp_ret_cur();
        wait_key2();

        def_end();
        break;
      /*********************************************/
      case 10:
        if (def_ini("Read test") == 1)
          break;
          
        long_str[0] = 0;
          
        #if DISP_USE_RW_LINE == 1 && DISP_DB_WIDTH != 1
        
          Disp_set_cxy(3,0);
          Disp_gets(long_str, 10);
          Disp_set_cxy(3,1);
          Disp_puts(long_str);
          
          wait_key2();
          
          Disp_set_input_mode(DEC);
          Disp_set_cxy(13,0);
          Disp_gets(long_str, 10);
          Disp_set_cxy(13,1);
          Disp_puts(long_str);
          
        #else
        
          Disp_set_cxy(0,1);
          Disp_puts("R/W is N/A!");
        
        #endif
        
        wait_key2();

        def_end();
        break;
      /*********************************************/
      case 11:
        if (def_ini("Charset ROM") == 1)
          break;
        
        Disp_set_cxy(0,1);
        Disp_set_ctempl_const(pl_charset, PL_CHARSET_LENGTH, 0);
        Disp_putc(PL_A);
        Disp_putc(PL_C);
        Disp_putc(PL_E);
        Disp_putc(PL_L);
        Disp_putc(PL_N);
        Disp_putc(PL_O);
        Disp_putc(PL_S);
        Disp_putc(PL_Z);
        
        wait_key2();

        def_end();
        break;
      /*********************************************/
      case 12:
        if (def_ini("Charset RAM") == 1)
          break;
        
        Disp_set_cxy(0,1);
        Disp_set_ctempl(pl_charset_mem, PL_CHARSET_LENGTH, 0);
        Disp_putc(PL_A);
        Disp_putc(PL_C);
        Disp_putc(PL_E);
        Disp_putc(PL_L);
        Disp_putc(PL_N);
        Disp_putc(PL_O);
        Disp_putc(PL_S);
        Disp_putc(PL_Z);
        
        wait_key2();
        
        def_end();
        break;
      /*********************************************/
      case 13:
        if (def_ini("Charset copy") == 1)
          break;
          
        str64[0] = 0;
        
        #if DISP_USE_RW_LINE == 1 && DISP_DB_WIDTH != 1
        
          Disp_set_cxy(0,1);
          Disp_set_ctempl_const(pl_charset, PL_CHARSET_LENGTH, 0);
          Disp_get_ctempl(str64, PL_CHARSET_LENGTH, 0);
          Disp_set_ctempl(str64, PL_CHARSET_LENGTH, 0);
          Disp_putc(PL_A);
          Disp_putc(PL_C);
          Disp_putc(PL_E);
          Disp_putc(PL_L);
          Disp_putc(PL_N);
          Disp_putc(PL_O);
          Disp_putc(PL_S);
          Disp_putc(PL_Z);
        
        #else
        
          Disp_set_cxy(0,1);
          Disp_puts("R/W is N/A!");
        
        #endif
        
        wait_key2();

        def_end();
        break;
      /*********************************************/
      case 14:
        if (def_ini("Error check") == 1)
          break;
        
        Disp_init();
        Disp_clr();
        Disp_puts("Checking...");
        temp = Disp_get_error();
        
        wait_key2();
        
        Disp_init();
        Disp_clr();
        if (temp)
          Disp_puts("Error");
        else
          Disp_puts("No Error");
        
        wait_key2();

        def_end();
        break;
      /*********************************************/
      case 15:
        if (def_ini("Contrast") == 1)
          break;
        
        #if DISP_EXT_MODE == 1
          uint8_t contrast = 0;
          Disp_set_contrast(0, 0);
          
          while (!get_key2()) {
            char num[3];
            
            if (get_key0())
              contrast++;
            if (get_key1())
              contrast--;
              
            Disp_set_contrast((contrast >> 4) & 0x07, contrast & 0x0F);
            Disp_set_cxy(0,1);
            Disp_puts("   ");
            Disp_set_cxy(0,1);
            itoa(contrast, num, 10);
            Disp_puts(num);
            wait_ms(25);
          }
          
          while (get_key2());
        #endif

        def_end();
        break;   
      /*********************************************/
      case 16:
        if (def_ini("Font") == 1)
          break;
          
          Disp_clr();
        
        #if DISP_EXT_MODE == 1
          Disp_set_font(16, 0);
          Disp_set_cxy(0,0);
          Disp_puts("large");
          Disp_set_cxy(0,1);
          Disp_puts("small");
          wait_key2();
          
          Disp_set_font(16, 1);
          Disp_set_cxy(0,1);
          Disp_puts("large");
          Disp_set_cxy(0,0);
          Disp_puts("small");
          wait_key2();
          
          Disp_set_font(8, 0);
          Disp_set_cxy(0,0);
          Disp_puts("small");
          Disp_set_cxy(0,1);
          Disp_puts("small");
          Disp_set_cxy(0,2);
          Disp_puts("small");
          wait_key2();
          
        #endif

        def_end();
        break;                         
    }
  }

  return 0;
}
