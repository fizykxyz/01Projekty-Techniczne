/****************************************************************************
* Biblioteka obslugi wyswietlacza graficznego, opartego na sterowniku
* PCD8544 lub odpowiedniku.
* 
* Program testowy dla platformy ARM7 LPC21XX.
*
* Nazwa pliku: test_LPC21.c
* Data: 20.10.2007
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

#include <PCD8544.h>
#include <disp_conf.h>
#include <platf_conf.h>
#include <LPC21XX.h>

#define NO_OF_TEST 21

#define KEY_01_PIN IOPIN1
#define KEY_2_PIN IOPIN1
#define KEY_0 24
#define KEY_1 25
#define KEY_2 26

#define get_key0() (!(KEY_01_PIN & (1<<KEY_0)))
#define get_key1() (!(KEY_01_PIN & (1<<KEY_1)))
#define get_key2() (!(KEY_2_PIN & (1<<KEY_2)))

const char intro[] = "Test PCD8544";
const char end[] = " Test end";
const char test[] = "Test";
const uint8_t charset[] = CHARSET;

uint8_t pict1[] = { /* width 50, length 7 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01,
  0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x02, 0xA0, 0xAA, 0xAA, 0xAA, 0x00, 0x01,
  0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  
uint8_t pictc[168];
  
const uint8_t cpict2[] = { /* width 16, length 20*/
  0x00, 0x00,
  0xAE, 0x7A,
  0x5E, 0x75,
  0xAE, 0x7A,
  0x5E, 0x75,
  0xAE, 0x7A,
  0x5E, 0x75,
  0xAE, 0x7A,
  0x5E, 0x75,
  0xAE, 0x7A,
  0x5E, 0x75,
  0xAE, 0x7A,
  0x5E, 0x75,
  0xAE, 0x7A,
  0x5E, 0x75,
  0xAE, 0x7A,
  0x5E, 0x75,
  0xAE, 0x7A,
  0x5E, 0x75,
  0x00, 0x00};

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
  Disp_clr();
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
  Disp_clr();
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
  Disp_set_ctempl_const(charset);
  Disp_set_update(IMMEDIATELY);
  
  Disp_box(0,0,83,47,ON,NO);
  Disp_set_cxy(1, 2);
  Disp_puts_const(intro);
  
  Disp_set_pxy(42,36);
  Disp_circle(10,ON);
  
  while(!chk_keyb());
  
  test_no = 0;
  
  while(1) {
    switch(test_no) {
      /*********************************************/
      case 0:
        if (def_ini("Init") == 1)
          break;
        
        Disp_init();
        Disp_puts("I1");
        wait_ms(300);
        
        Disp_init();
        Disp_puts("I2");
        wait_ms(300);
          
        Disp_init();
        Disp_puts("I3");
        wait_ms(300);
                  
        Disp_init();
        Disp_puts("I4");
        wait_ms(300);
                  
        Disp_init();
        Disp_puts("I5");
        
        def_end();
        break;
      /*********************************************/
      case 1:
        if (def_ini("Disp set") == 1)
          break;
        
        Disp_set(OFF, OFF);
        wait_key2();
        Disp_set(ON, OFF);
        wait_key2();
        Disp_set(ON, ON);
        wait_key2();
        
        Disp_set(ON, OFF);
        
        def_end();
        break;
      /*********************************************/
      case 2:
        if (def_ini("Curs") == 1)
          break;
        
        Disp_set_cxy(0,1);
        Disp_puts("off");
        Disp_set_cur(OFF);
        wait_key2();
        
        Disp_set_cxy(0,1);
        Disp_puts("on ");
        Disp_set_cur(ON);
        wait_key2();
        
        for (uint8_t i=0; i<30; i++) {
          Disp_move_txt_cur(RIGHT);
          wait_ms(200);
          if (get_key2()) {
            while(get_key2());
            wait_ms(10);
            break;
          }
        }
        for (uint8_t i=0; i<30; i++) {
          Disp_move_txt_cur(LEFT);
          wait_ms(200);
          if (get_key2()) {
            while(get_key2());
            wait_ms(10);
            break;
          }
        }
          
        for (uint8_t i=0; i<3; i++) {
         for (uint8_t j=0; j<6; j++) {
            Disp_set_cxy(i,j);
            wait_ms(200);
            if (get_key2()) {
              while(get_key2());
              wait_ms(10);
              break;
            }
          }
          if (get_key2()) {
            while(get_key2());
            wait_ms(10);
            break;
          }
        }
        
        Disp_set_cxy(0,0);
        for (uint8_t i=0; i<14; i++) {
          Disp_putc('X');
        }
        wait_key2();      
       
        def_end();
        break;
      /*********************************************/
      case 3:
        if (def_ini("INC/DEC") == 1)
          break;
        
        Disp_set_cur(ON);
        
        Disp_set_input_mode(INC);
        Disp_set_cxy(0,1);
        Disp_puts_const(test);
        wait_key2();
        
        Disp_set_input_mode(DEC);
        Disp_set_cxy(3,1);
        Disp_puts_const(test);
        wait_key2();
        
        def_end();
        break;
      /*********************************************/
      case 4:
        if (def_ini("Buf") == 1)
          break;
        
        Disp_set_cur(OFF);
        Disp_update();
        
        Disp_set_update(DEFERRED);
        Disp_set_cxy(0,1);
        Disp_puts("clk!");
        wait_key2();
        
        Disp_update();
        wait_key2();
        
        Disp_clr();
        Disp_update();
        Disp_puts_const(test);        
        wait_key2();
        
        if (Disp_get_ovf()) {
          Disp_flush_buffer();
          Disp_clr();
          Disp_puts("OVF!");
        }
          
        Disp_update();
        wait_key2();
        
        Disp_clr();
        Disp_puts("Flush");
        Disp_update();
        wait_key2();
        
        Disp_clr();
        Disp_puts("OK");
        Disp_update();
        Disp_set_cxy(0,0);
        Disp_puts("error");
        Disp_flush_buffer();
        Disp_update();
        wait_key2();
        
        Disp_set_update(IMMEDIATELY);
        Disp_clr();
        Disp_puts("Immed.");
        wait_key2();
        
        def_end();
        break;
      /*********************************************/
      case 5:
        if (def_ini("Lines") == 1)
          break;
        
        Disp_clr();
        for (uint8_t i=0; i<DISP_C_ROWS_NUM; i++) {
          itoa(i, char_str, 10);
          Disp_set_cxy(i,i);
          Disp_puts("Line ");
          Disp_puts(char_str);
        }      
        
        wait_key2();
        
        def_end();
        break;
      /*********************************************/
      case 6:
        if (def_ini("Cur pos") == 1)
          break;
        
        Disp_set_input_mode(INC);
        Disp_set_cur(ON);
        Disp_clr();
        
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
        if (def_ini("Move C/W") == 1)
          break;
          
        Disp_set_cur(ON);
        
        Disp_clr();
        Disp_puts("RT");
        Disp_move_txt_cur(RIGHT);
        Disp_putc('!');
        
        Disp_set_cxy(0,1);
        Disp_puts("LT ");
        Disp_move_txt_cur(LEFT);
        Disp_putc('!');
        
        wait_key2();

        def_end();
        break;
      /*********************************************/
      case 8:
        if (def_ini("Clear") == 1)
          break;
          
        for (int i=0; i<DISP_C_ROWS_NUM; i++) {
          Disp_set_cxy(0,i);
          for (int j=0; j<DISP_C_COLS_NUM; j++)
            Disp_putc('X');
        }
        
        wait_key2();
        Disp_clr();
        Disp_puts("Clr.");
        wait_key2();

        def_end();
        break;
      /*********************************************/
      case 9:
        if (def_ini("Cur ret") == 1)
          break;
        
        Disp_clr();
        Disp_set_cur(ON);
        
        Disp_ret_cur();
        wait_key2();
        
        Disp_set_cxy(3,2);
        Disp_ret_cur();
        wait_key2();
        
        Disp_set_cxy(12,3);
        Disp_ret_cur();
        wait_key2();

        def_end();
        break;
      /*********************************************/
      case 10:
        if (def_ini("Pic R/W") == 1)
          break;
          
        for (uint8_t i=0; i<8; i++) {
          for (uint8_t j=0; j<8; j++) {
            Disp_clr();
            Disp_set_pxy(10+i,j);
            Disp_put_picture(pict1, 50, 7);
            Disp_set_pxy(10+i,j);
            Disp_get_picture(pictc, 50, 7);
            Disp_set_pxy(10+i,j+10);
            Disp_put_picture(pictc, 50, 7);
            wait_ms(200);    
            if (get_key2())
             break;      
          }
          if (get_key2()) {
            while(get_key2());
            wait_ms(10);
            break;
          }
        }
        
        for (uint8_t i=0; i<8; i++) {
          for (uint8_t j=0; j<8; j++) {
            Disp_box(0,0,83,47,ON,YES);
            Disp_set_pxy(10+i,j);
            Disp_put_picture(pict1, 50, 7);
            Disp_set_pxy(10+i,j);
            Disp_get_picture(pictc, 50, 7);
            Disp_set_pxy(10+i,j+10);
            Disp_put_picture(pictc, 50, 7);
            wait_ms(200);
            if (get_key2())
             break;      
          }
          if (get_key2()) {
            while(get_key2());
            wait_ms(10);
            break;
          }
        }
        
        for (uint8_t i=0; i<5; i++) {
          Disp_clr();
          Disp_set_pxy(30,i);
          Disp_put_picture_const(cpict2, 16, 20);
          Disp_set_pxy(30,i);
          Disp_get_picture(pictc, 16, 20);
          Disp_set_pxy(30,i+21);
          Disp_put_picture(pictc, 16, 20);
          wait_ms(200);
          if (get_key2()) {
            while(get_key2());
            wait_ms(10);
            break;
          }
        }

        for (uint8_t i=0; i<5; i++) {
          Disp_box(0,0,83,47,ON,YES);
          Disp_set_pxy(30,i);
          Disp_put_picture_const(cpict2, 16, 20);
          Disp_set_pxy(30,i);
          Disp_get_picture(pictc, 16, 20);
          Disp_set_pxy(30,i+21);
          Disp_put_picture(pictc, 16, 20);
          wait_ms(200);
          if (get_key2()) {
            while(get_key2());
            wait_ms(10);
            break;
          }
        }

        def_end();
        break;
      /*********************************************/
      case 11:
        if (def_ini("CG ROM") == 1)
          break;
        
        Disp_set_cxy(0,1);
        Disp_putc(PL_A);
        Disp_putc(PL_C);
        Disp_putc(PL_E);
        Disp_putc(PL_L);
        Disp_putc(PL_N);
        Disp_putc(PL_O);
        Disp_putc(PL_S);
        Disp_putc(PL_Z);
        Disp_set_cxy(0,2);
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
        if (def_ini("Pic WR ROM") == 1)
          break;
        
        for (uint8_t i=0; i<25; i++) {
          Disp_box(0,0,83,47,ON,YES);
          Disp_set_pxy(30,i);
          Disp_put_picture_const(cpict2, 16, 20);
          wait_ms(200);
          if (get_key2()) {
            while(get_key2());
            wait_ms(10);
            break;
          }
        }
        
        def_end();
        break;
      /*********************************************/
      case 13:
        if (def_ini("Pic play") == 1)
          break;
        
        Disp_set_update(DEFERRED);
        Disp_set_pxy(10,10);
        Disp_lineto(20,30, ON);
        Disp_lineto(30,10, ON);
        Disp_lineto(40,30, ON);
        Disp_lineto(50,10, ON);
        Disp_lineto(60,30, ON);
        Disp_lineto(70,10, ON);
        Disp_lineto(70,30, ON);
        Disp_lineto(60,10, ON);
        Disp_lineto(50,30, ON);
        Disp_lineto(40,10, ON);
        Disp_lineto(30,30, ON);
        Disp_lineto(20,10, ON);
        Disp_lineto(10,30, ON);
        Disp_lineto(10,10, ON);
        Disp_lineto(70,10, ON);
        Disp_lineto(70,30, ON);
        Disp_lineto(10,30, ON);
        Disp_set_update(IMMEDIATELY);
        wait_key2();
        
        Disp_set_pxy(10,10);
        Disp_get_picture(pictc,61,21);

        Disp_clr();
        Disp_set_pxy(0,0);
        Disp_put_picture(pictc,61,21);
        Disp_set_pxy(0,21);
        Disp_put_picture(pictc,61,21);
        wait_ms(1000);
        Disp_set_pxy(20,0);
        Disp_put_picture(pictc,61,21);
        Disp_set_pxy(20,21);
        Disp_put_picture(pictc,61,21);
        wait_ms(1000);
        Disp_set_pxy(10,10);
        Disp_put_picture(pictc,61,21);
        wait_key2();
        
        Disp_box(0,0,83,47,ON,YES);
        Disp_set_pxy(0,0);
        Disp_put_picture(pictc,61,21);
        Disp_set_pxy(0,21);
        Disp_put_picture(pictc,61,21);
        wait_ms(1000);
        Disp_set_pxy(20,0);
        Disp_put_picture(pictc,61,21);
        Disp_set_pxy(20,21);
        Disp_put_picture(pictc,61,21);
        wait_ms(1000);
        Disp_set_pxy(10,10);
        Disp_put_picture(pictc,61,21);
        wait_key2();

        def_end();
        break;
      /*********************************************/
      case 14:
        if (def_ini("Err chk") == 1)
          break;
        
        Disp_init();
        Disp_clr();
        Disp_puts("...");
        temp = Disp_get_error();
        
        wait_key2();
        
        Disp_init();
        Disp_clr();
        if (temp)
          Disp_puts("Err");
        else
          Disp_puts("No Err");
        
        wait_key2();

        def_end();
        break;
      /*********************************************/
      case 15:
        if (def_ini("H. lines") == 1)
          break;
          
        Disp_clr();
        Disp_set_pxy(1, 1);
        Disp_hlineto(1, ON);
        Disp_set_pxy(1, 2);
        Disp_hlineto(2, ON);
        Disp_set_pxy(1, 3);
        Disp_hlineto(3, ON);
        Disp_set_pxy(1, 4);
        Disp_hlineto(2, ON);
        Disp_hlineto(4, ON);
        wait_key2();
        
        Disp_clr();
        Disp_set_pxy(1, 1);
        Disp_hlineto(1, ON);
        Disp_set_pxy(2, 2);
        Disp_hlineto(1, ON);
        Disp_set_pxy(3, 3);
        Disp_hlineto(1, ON);
        Disp_set_pxy(4, 4);
        Disp_hlineto(3, ON);
        Disp_hlineto(1, ON);
        wait_key2();
        
        Disp_box(0,0,83,47,ON,YES);
        Disp_set_pxy(1, 1);
        Disp_hlineto(1, OFF);
        Disp_set_pxy(1, 2);
        Disp_hlineto(2, OFF);
        Disp_set_pxy(1, 3);
        Disp_hlineto(3, OFF);
        Disp_set_pxy(1, 4);
        Disp_hlineto(2, OFF);
        Disp_hlineto(4, OFF);
        wait_key2();
        
        Disp_box(0,0,83,47,ON,YES);
        Disp_set_pxy(1, 1);
        Disp_hlineto(1, OFF);
        Disp_set_pxy(2, 2);
        Disp_hlineto(1, OFF);
        Disp_set_pxy(3, 3);
        Disp_hlineto(1, OFF);
        Disp_set_pxy(4, 4);
        Disp_hlineto(3, OFF);
        Disp_hlineto(1, OFF);
        wait_key2();
        
        Disp_set_update(DEFERRED);
        Disp_clr();
        for (uint8_t i=0; i<48; i++) {
          Disp_set_pxy(16, i);
          Disp_hlineto(16+i, ON);
        }
        Disp_update();
        wait_key2();
        
        Disp_box(0,0,83,47,ON,YES);
        for (uint8_t i=0; i<48; i++) {
          Disp_set_pxy(16, i);
          Disp_hlineto(16+i, OFF);
        }
        Disp_update();
        wait_key2();
        
        Disp_clr();
        for (uint8_t i=0; i<48; i++) {
          Disp_set_pxy(80, i);
          Disp_hlineto(80-i, ON);
        }
        Disp_update();
        wait_key2();
        
        Disp_box(0,0,83,47,ON,YES);
        for (uint8_t i=0; i<48; i++) {
          Disp_set_pxy(80, i);
          Disp_hlineto(80-i, OFF);
        }
        Disp_update();
        wait_key2();
        
        Disp_clr();
        for (uint8_t i=0; i<48; i++) {
          Disp_set_pxy(i+10, i);
          Disp_hlineto(57-i, ON);
        }
        Disp_update();
        wait_key2();
        
        Disp_box(0,0,83,47,ON,YES);
        for (uint8_t i=0; i<48; i++) {
          Disp_set_pxy(i+10, i);
          Disp_hlineto(57-i, OFF);
        }
        Disp_set_update(IMMEDIATELY);
        wait_key2();

        def_end();
        break;     
      /*********************************************/
      case 16:
        if (def_ini("V. lines") == 1)
          break;
          
        Disp_clr();
        Disp_set_pxy(1, 1);
        Disp_vlineto(1, ON);
        Disp_set_pxy(2, 1);
        Disp_vlineto(2, ON);
        Disp_set_pxy(3, 1);
        Disp_vlineto(3, ON);
        Disp_set_pxy(4, 1);
        Disp_vlineto(2, ON);
        Disp_vlineto(4, ON);
        wait_key2();
        
        Disp_clr();
        Disp_set_pxy(1, 1);
        Disp_vlineto(1, ON);
        Disp_set_pxy(2, 2);
        Disp_vlineto(1, ON);
        Disp_set_pxy(3, 3);
        Disp_vlineto(1, ON);
        Disp_set_pxy(4, 4);
        Disp_vlineto(3, ON);
        Disp_vlineto(1, ON);
        wait_key2();
        
        Disp_box(0,0,83,47,ON,YES);
        Disp_set_pxy(1, 1);
        Disp_vlineto(1, OFF);
        Disp_set_pxy(2, 1);
        Disp_vlineto(2, OFF);
        Disp_set_pxy(3, 1);
        Disp_vlineto(3, OFF);
        Disp_set_pxy(4, 1);
        Disp_vlineto(2, OFF);
        Disp_vlineto(4, OFF);
        wait_key2();
        
        Disp_box(0,0,83,47,ON,YES);
        Disp_set_pxy(1, 1);
        Disp_vlineto(1, OFF);
        Disp_set_pxy(2, 2);
        Disp_vlineto(1, OFF);
        Disp_set_pxy(3, 3);
        Disp_vlineto(1, OFF);
        Disp_set_pxy(4, 4);
        Disp_vlineto(3, OFF);
        Disp_vlineto(1, OFF);
        wait_key2();
        
        Disp_clr();
        Disp_set_update(DEFERRED);
        for (uint8_t i=0; i<48; i++) {
          Disp_set_pxy(8+i, 0);
          Disp_vlineto(i, ON);
        }
        Disp_update();
        wait_key2();
        
        Disp_box(0,0,83,47,ON,YES);
        for (uint8_t i=0; i<48; i++) {
          Disp_set_pxy(8+i, 0);
          Disp_vlineto(i, OFF);
        }
        Disp_update();
        wait_key2();
        
        Disp_clr();
        for (uint8_t i=0; i<48; i++) {
          Disp_set_pxy(8+i, 47);
          Disp_vlineto(i, ON);
        }
        Disp_update();
        wait_key2();
        
        Disp_box(0,0,83,47,ON,YES);
        for (uint8_t i=0; i<48; i++) {
          Disp_set_pxy(8+i, 47);
          Disp_vlineto(i, OFF);
        }
        Disp_update();
        wait_key2();
        
        Disp_clr();
        for (uint8_t i=0; i<48; i++) {
          Disp_set_pxy(8+i, i);
          Disp_vlineto(47-i, ON);
        }
        Disp_update();
        wait_key2();
        
        Disp_box(0,0,83,47,ON,YES);
        for (uint8_t i=0; i<48; i++) {
          Disp_set_pxy(8+i, i);
          Disp_vlineto(47-i, OFF);
        }
        Disp_set_update(IMMEDIATELY);
        wait_key2();

        def_end();
        break;   
      /*********************************************/
      case 17:
        if (def_ini("Rand lines") == 1)
          break;
        
        Disp_clr();
        Disp_set_pxy(0,0);
        Disp_lineto(5,47,ON);
        Disp_lineto(10,0,ON);
        Disp_lineto(20,47,ON);
        Disp_lineto(30,0,ON);
        Disp_lineto(50,47,ON);
        Disp_lineto(70,0,ON);
        Disp_lineto(83,47,ON);
        wait_key2();
        
        Disp_clr();
        Disp_set_pxy(0,0);
        Disp_lineto(83,5,ON);
        Disp_lineto(0,10,ON);
        Disp_lineto(83,20,ON);
        Disp_lineto(0,30,ON);
        Disp_lineto(83,47,ON);
        wait_key2();
        
        Disp_clr();
        Disp_set_pxy(0,0);
        Disp_lineto(83,47,ON);
        Disp_set_pxy(83,0);
        Disp_lineto(0,47,ON);
        wait_key2();
        
        Disp_clr();
        Disp_set_pxy(41,23);
        Disp_lineto(41,47,ON);
        Disp_set_pxy(41,23);
        Disp_lineto(41,0,ON);
        Disp_set_pxy(41,23);
        Disp_lineto(83,23,ON);
        Disp_set_pxy(41,23);
        Disp_lineto(0,23,ON);
        Disp_set_pxy(41,23);
        Disp_lineto(0,0,ON);
        Disp_set_pxy(41,23);
        Disp_lineto(0,47,ON);
        Disp_set_pxy(41,23);
        Disp_lineto(83,0,ON);
        Disp_set_pxy(41,23);
        Disp_lineto(83,47,ON);
        wait_key2();

        def_end();
        break;   
      /*********************************************/
      case 18:
        if (def_ini("Pix p/g") == 1)
          break;
        
        uint8_t temp;
        
        Disp_clr();
        Disp_set_pxy(10,10);
        Disp_putp(ON);
        Disp_set_pxy(20,10);
        Disp_putp(ON);
        Disp_set_pxy(10,20);
        Disp_putp(ON);
        Disp_set_pxy(20,20);
        Disp_putp(ON);
        for (uint8_t i=0; i<=10; i++)
          for (uint8_t j=0; j<=10; j++) {
            Disp_set_pxy(10+i, 10+j);
            temp = Disp_getp();
            Disp_set_pxy(30+10+i, 10+j);
            Disp_putp(temp);
          }
        wait_key2();
        
        Disp_box(0,0,83,47,ON,YES);
        Disp_set_pxy(10,10);
        Disp_putp(OFF);
        Disp_set_pxy(20,10);
        Disp_putp(OFF);
        Disp_set_pxy(10,20);
        Disp_putp(OFF);
        Disp_set_pxy(20,20);
        Disp_putp(OFF);
        for (uint8_t i=0; i<=10; i++)
          for (uint8_t j=0; j<=10; j++) {
            Disp_set_pxy(10+i, 10+j);
            temp = Disp_getp();
            Disp_set_pxy(30+10+i, 10+j);
            Disp_putp(temp);
          }
        wait_key2();

        def_end();
        break;   
      /*********************************************/
      case 19:
        if (def_ini("Box") == 1)
          break;
         
        Disp_clr();
        Disp_box(1,1,1,1,ON,NO);
        Disp_box(10,1,11,2,ON,NO);
        Disp_box(1,1+5,1,1+5,ON,NO);
        Disp_box(11,2+5,10,1+5,ON,NO);
        Disp_box(1+20,1,1+20,1,ON,YES);
        Disp_box(10+20,1,11+20,2,ON,YES);
        Disp_box(1+20,1+5,1+20,1+5,ON,YES);
        Disp_box(11+20,2+5,10+20,1+5,ON,YES);
        wait_key2();
               
        Disp_clr();
        Disp_box(1,1,82,46,ON,NO);
        Disp_box(80,44,3,3,ON,NO);
        Disp_box(5,5,78,42,ON,NO);
        Disp_box(76,40,7,7,ON,NO);
        Disp_box(9,9,74,38,ON,NO);
        Disp_box(72,36,11,11,ON,NO);
        wait_key2();
        
        Disp_clr();
        Disp_box(1,1,82,46,ON,YES);
        Disp_box(80,44,3,3,OFF,YES);
        Disp_box(5,5,78,42,ON,YES);
        Disp_box(76,40,7,7,OFF,YES);
        Disp_box(9,9,74,38,ON,YES);
        Disp_box(72,36,11,11,OFF,YES);
        wait_key2();

        def_end();
        break;   
      /*********************************************/
      case 20:
        if (def_ini("Txt WR") == 1)
          break;
        
        Disp_clr();
        Disp_set_cxy(DISP_C_COLS_NUM-2,DISP_C_ROWS_NUM-1);
        Disp_puts("RBLT");
        Disp_set_cxy(DISP_C_COLS_NUM-2,0);
        Disp_puts("RT");
        Disp_set_cxy(0,DISP_C_ROWS_NUM-1);
        Disp_puts("LB");
        Disp_set_cxy(DISP_C_COLS_NUM-1,DISP_C_ROWS_NUM/2-1);
        Disp_puts("12");
        wait_key2();
        
        Disp_clr();
        Disp_set_input_mode(DEC);
        Disp_set_cxy(1,0);
        Disp_puts("TLBR");
        Disp_set_cxy(DISP_C_COLS_NUM-1,0);
        Disp_puts("TR");
        Disp_set_cxy(1,DISP_C_ROWS_NUM-1);
        Disp_puts("BL");
        Disp_set_cxy(0,DISP_C_ROWS_NUM/2);
        Disp_puts("21");
        wait_key2();

        def_end();
        break;   
      /*********************************************/
      case 21:
        if (def_ini("Contrast") == 1)
          break;
        
        Disp_clr();
        Disp_box(10,10,73,23,ON,YES);
        uint8_t contrast = 0;
        Disp_set_contrast(contrast);
        while (!get_key2()) {
          char num[3];
          
          if (get_key0())
            Disp_set_contrast(++contrast);
          if (get_key1())
            Disp_set_contrast(--contrast);
          Disp_set_cxy(0,0);
          Disp_puts("   ");
          Disp_set_cxy(0,0);
          itoa(contrast, num, 10);
          Disp_puts(num);
          wait_ms(25);
        }
        
        while (get_key2());

        def_end();
        break;                 
    }
  }

  return 0;
}
