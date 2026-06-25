/****************************************************************************
* Biblioteka obslugi wyswietlacza alfanumerycznego, opartego na sterowniku
* M66004 (VFD) lub odpowiedniku.
* 
* Program testowy dla platformy ARM7 LPC21XX.
*
* Nazwa pliku: test_LPC21.c
* Data: 21.08.2007
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

#include <M66004.h>
#include <disp_conf.h>
#include <platf_conf.h>
#include <LPC21XX.h>

#define NO_OF_TEST 15

#define KEY_01_PIN IOPIN1
#define KEY_2_PIN IOPIN1
#define KEY_0 24
#define KEY_1 25
#define KEY_2 26

#define get_key0() (!(KEY_01_PIN & (1<<KEY_0)))
#define get_key1() (!(KEY_01_PIN & (1<<KEY_1)))
#define get_key2() (!(KEY_2_PIN & (1<<KEY_2)))

const char intro[] = "Test M66004";
const char end[] = " Test end";
const uint8_t pl_charset[] = PL_CHARSET;
uint8_t pl_charset_mem[] = PL_CHARSET;

volatile uint8_t test_no = 0;

void itoa(uint8_t number, char* string, uint8_t radix) {
  if (number >= radix*radix) {
    *(string++) = (number / (radix*radix)) + 0x30;
    number %= radix*radix;
    *(string++) = (number / radix) + 0x30;
    number %= radix;
  } else if (number >= radix) {
    *(string++) = (number / radix) + 0x30;
    number %= radix;
  }
  *(string++) = (number) + 0x30;
  *(string) = 0;
}

void wait_ms(int ms) {
  while(ms--)
    delay_ms(1);
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
  uint8_t temp;
  
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
        #if DISP_EN_NUM == 2
          Disp_set_cxy(0,2);
          Disp_puts("Init 1");
        #endif
        wait_ms(300);
        
        Disp_init();
        Disp_puts("Init 2");
        #if DISP_EN_NUM == 2
          Disp_set_cxy(0,2);
          Disp_puts("Init 2");
        #endif
        wait_ms(300);
          
        Disp_init();
        Disp_puts("Init 3");
        #if DISP_EN_NUM == 2
          Disp_set_cxy(0,2);
          Disp_puts("Init 3");
        #endif
        wait_ms(300);
                  
        Disp_init();
        Disp_puts("Init 4");
        #if DISP_EN_NUM == 2
          Disp_set_cxy(0,2);
          Disp_puts("Init 4");
        #endif
        wait_ms(300);
                  
        Disp_init();
        Disp_puts("Init 5");
        #if DISP_EN_NUM == 2
          Disp_set_cxy(0,2);
          Disp_puts("Init 5");
        #endif
        
        def_end();
        break;
      /*********************************************/
      case 1:
        if (def_ini("Disp on-off") == 1)
          break;
          
        #if DISP_EN_NUM == 2
          Disp_set_cxy(0,2);
          Disp_puts("Disp on-off");
          wait_key2();
        #endif
        
        Disp_set(OFF);
        wait_key2();
        Disp_set(ON);
        
        def_end();
        break;
      /*********************************************/
      case 2:
        if (def_ini("Cursor test") == 1)
          break;
        
        Disp_clr();
        
        Disp_set_cxy(0,0);
        Disp_puts("off");
        Disp_set_cur(OFF);
        wait_key2();
        
        Disp_set_cxy(0,0);
        Disp_puts("on ");
        Disp_set_cur(ON);
        wait_key2();
        
        def_end();
        break;
      /*********************************************/
      case 3:
        if (def_ini("INC/DEC test") == 1)
          break;
          
        Disp_clr();
        
        Disp_set_input_mode(INC);
        Disp_set_cxy(0,0);
        Disp_puts("Test");
        wait_key2();
        
        Disp_set_input_mode(DEC);
        Disp_set_cxy(3,0);
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
        if (def_ini("Border test") == 1)
          break;
        
        Disp_clr();
        
        Disp_set_input_mode(INC);
        Disp_set_cxy(14,0);
        Disp_puts("Test");
        wait_key2();
        
        Disp_clr();
        
        Disp_set_input_mode(DEC);
        Disp_set_cxy(1,0);
        Disp_puts("tseT");
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
        if (def_ini("Move cur") == 1)
          break;
        
        Disp_clr();
        Disp_puts("right");
        Disp_move_txt_cur(RIGHT);
        Disp_putc('!');
        
        wait_key2();
        
        Disp_clr();
        Disp_set_cxy(0,0);
        Disp_puts("left ");
        Disp_move_txt_cur(LEFT);
        Disp_putc('!');
        
        wait_key2();

        def_end();
        break;
      /*********************************************/
      case 8:
        if (def_ini("Clear test") == 1)
          break;
          
        for (int i=0; i<16; i++) {
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
        Disp_set_cur(ON);
        
        Disp_ret_cur();
        wait_key2();
        
        Disp_set_cxy(10,0);
        Disp_ret_cur();
        wait_key2();

        def_end();
        break;
      /*********************************************/
      case 10:
        if (def_ini("Set contrast") == 1)
          break;
        
        for (uint8_t i=0; i<8; i++) {
          itoa(i, char_str, 10);
          Disp_set_cxy(15,0);
          Disp_puts(char_str);
          Disp_set_contrast(i);
          wait_key2();
        }
        
        Disp_set_contrast(DISP_DEF_CONTRAST);

        def_end();
        break;
      /*********************************************/
      case 11:
        if (def_ini("Charset ROM") == 1)
          break;
        
        Disp_set_cxy(0,0);
        Disp_set_ctempl_const(pl_charset, PL_CHARSET_LENGTH, 0);
        Disp_putc(PL_A);
        Disp_putc(PL_C);
        Disp_putc(PL_E);
        Disp_putc(PL_L);
        Disp_putc(PL_N);
        Disp_putc(PL_O);
        Disp_putc(PL_S);
        Disp_putc(PL_Z);
        Disp_putc(PL_a);
        Disp_putc(PL_c);
        Disp_putc(PL_e);
        Disp_putc(PL_l);
        Disp_putc(PL_n);
        Disp_putc(PL_o);
        Disp_putc(PL_s);
        Disp_putc(PL_z);
        
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
        Disp_putc(PL_a);
        Disp_putc(PL_c);
        Disp_putc(PL_e);
        Disp_putc(PL_l);
        Disp_putc(PL_n);
        Disp_putc(PL_o);
        Disp_putc(PL_s);
        Disp_putc(PL_z);
        
        wait_key2();
        
        def_end();
        break;
      /*********************************************/
      case 13:
        if (def_ini("N/A") == 1)
          break;
        
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
        if (def_ini("N/A") == 1)
          break;
        
        wait_key2();

        def_end();
        break;                       
    }
  }

  return 0;
}
