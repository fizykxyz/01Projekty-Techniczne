/****************************************************************************
* Biblioteka obslugi wyswietlacza graficznego, opartego na sterowniku
* T6963C lub odpowiedniku.
*
* Nazwa pliku: T6963C.c
* Data: 11.08.2007
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

#include <inttypes.h>
#include "T6963C.h"
#include "disp_conf.h"
#include "platf_conf.h"

#define EMPTY   0x80
#define COMMAND 0x10

#define GRAPH   0
#define TEXT    1
#define OTHER   2

/*
 * Definicje etykiet dla komend
 */
#define POINTER_CUR     0x21
#define POINTER_OFFSET  0x22
#define POINTER_ADDR    0x24

#define ADDR_TEXT_HOME  0x40
#define ADDR_TEXT_AREA  0x41
#define ADDR_GRAPH_HOME 0x42
#define ADDR_GRAPH_AREA 0x43

#define MODE_CGROM      0x80
#define MODE_CGRAM      0x88
#define M_OR            0x00
#define M_EXOR          0x01
#define M_AND           0x03
#define M_ATTR          0x04

#define DISP_MODE       0x90
#define D_GRAPH         0x08
#define D_TEXT          0x04
#define D_CURSOR        0x02
#define D_CUR_BLINK     0x01

#define CURSOR_LEN      0xA0

#define DATA_A_WR       0xB0
#define DATA_A_RD       0xB1
#define DATA_A_RST      0xB2

#define DATA_WR         0xC0
#define DATA_RD         0xC1
#define D_RW_PTR_NOCH   0x04
#define D_RW_PTR_DOWN   0x02

#define SCREEN_PEEK     0xE0
#define SCREEN_COPY     0xE8

#define BIT_RST         0xF0
#define BIT_SET         0xF8

/*
 * Definicje etykiet bitow stanu
 */
#define STA0 0x01
#define STA1 0x02
#define STA2 0x04
#define STA3 0x08
#define STA4 0x10
#define STA5 0x20
#define STA6 0x40
#define STA7 0x80

/*
 * Definicje makr.
 */


/*
 * Definicje zmiennych prywatnych
 */
static volatile uint8_t error = 1;
static volatile uint8_t overflow = 0;
static uint8_t update = 0;
static uint8_t cur_x;
static uint8_t cur_y;
static uint8_t pix_x;
static uint8_t pix_y;
static uint8_t graph_txt_pointer;

static uint8_t entry_mode;
static uint8_t settings;
static uint16_t txt_shift;

static uint8_t fifo[DISP_BUFFER_LEN * 2];
static volatile uint8_t* fifo_rd_ptr;
static volatile uint8_t* fifo_wr_ptr;


/*
 * Definicje funkcji prywatnych
 */
static void update_cursor(void);
 
static void write_data(uint8_t data, uint8_t condition);
__inline__ static void send_cmd(uint8_t cmd, uint8_t condition);

static uint8_t wait_if_busy(uint8_t condition);
static uint8_t read_data(uint8_t condition);

static uint8_t read_buffer(uint8_t* data);
static void write_buffer(uint8_t data, uint8_t condition);

/*
 * Inicjalizuje wyswietlacz.
 * 
 * zwraca:
 *    0 - jesli inicjalizacja przebiegla poprawnie
 *    1 - jesli wystapil blad
 */
uint8_t Disp_init(void) {
  init_ports();
  
  set_wr();
  set_rd();
  #if DISP_USE_CE == 1
    set_ce();
  #endif

  delay_ms(10);
  set_res();
    
  error = 0;
  overflow = 0;
  settings = DISP_MODE | D_GRAPH | D_TEXT;
  entry_mode = INC;
  cur_x = 0;
  cur_y = 0;
  pix_x = 0;
  pix_y = 0;
  txt_shift = 0;
  
  Disp_flush_buffer();
  
  write_buffer(MODE_CGROM | M_OR, COMMAND | STA0 | STA1);
  
  write_buffer((uint8_t)((DISP_GRAPH_HOME & 0x00FF) >> 0), STA0 | STA1);
  write_buffer((uint8_t)((DISP_GRAPH_HOME & 0xFF00) >> 8), STA0 | STA1);
  write_buffer(ADDR_GRAPH_HOME, COMMAND | STA0 | STA1);
  
  write_buffer((uint8_t)DISP_P_BYTES_NUM, STA0 | STA1);
  write_buffer(0, STA0 | STA1);
  write_buffer(ADDR_GRAPH_AREA, COMMAND | STA0 | STA1);
  
  write_buffer((uint8_t)((DISP_TEXT_HOME & 0x00FF) >> 0), STA0 | STA1);
  write_buffer((uint8_t)((DISP_TEXT_HOME & 0xFF00) >> 8), STA0 | STA1);
  write_buffer(ADDR_TEXT_HOME, COMMAND | STA0 | STA1);
  
  write_buffer((uint8_t)DISP_C_COLS_NUM, STA0 | STA1);
  write_buffer(0, STA0 | STA1);
  write_buffer(ADDR_TEXT_AREA, COMMAND | STA0 | STA1);
  
  write_buffer(DISP_CGRAM_HOME >> 11, STA0 | STA1);
  write_buffer(0, STA0 | STA1);
  write_buffer(POINTER_OFFSET, COMMAND | STA0 | STA1);
  
  graph_txt_pointer = OTHER;
  Disp_clr();
  
  write_buffer(settings, COMMAND | STA0 | STA1);
  
  return error | overflow;
}

/*
 * Sprawdza poprawnosc funkcjonowania wyswietlacza.
 * 
 * zwraca:
 *    0 - jesli wyswietlacz pracuje poprawnie
 *    1 - jesli wystapil blad (np. timeout)
 */
uint8_t Disp_get_error(void) {
  return error;
}

/*
 * Okresla atrybuty wyswietlacza.
 * 
 * parametry:
 *    on    - wlaczony (ON) badz wylaczony (OFF)
 */
void Disp_set(uint8_t on) {
  if (on == ON)
    settings |= D_GRAPH | D_TEXT;
  else
    settings &= ~(D_GRAPH | D_TEXT);
    
  write_buffer(settings, COMMAND | STA0 | STA1);
}

/*
 * Okresla atrybuty kursora znakowego.
 * 
 * parametry:
 *    on    - wlaczony (ON) badz wylaczony (OFF)
 *            lub wysokosc kursora (1..8)
 *    blink - migajacy (BLINK) badz niemigajacy (NO_BLINK)
 */
void Disp_set_cur(uint8_t on, uint8_t blink) {
  if (on != OFF) {
    settings |= D_CURSOR;
    
    write_buffer(cur_x, STA0 | STA1);
    write_buffer(cur_y, STA0 | STA1);
    write_buffer(POINTER_CUR, COMMAND | STA0 | STA1);
    
    write_buffer(CURSOR_LEN | ((on - 1) & 0x07) , COMMAND | STA0 | STA1);
  } else
    settings &= ~(D_CURSOR);
    
  if (blink == BLINK)
    settings |= (D_CUR_BLINK);
  else
    settings &= ~(D_CUR_BLINK);
  
  write_buffer(settings, COMMAND | STA0 | STA1);
}

/*
 * Okresla tryb wprowadzania.
 * 
 * parametry:
 *    input_mode - autoinkrementacja adresu przy zapisie i odczycie (INC)
 *                 badz autodekrementacja (DEC)
 */
void Disp_set_input_mode(uint8_t input_mode) {
  entry_mode = input_mode;
}

/*
 * Okresla tryb pracy generatora znakow.
 * 
 * parametry:
 *    templ_mem - pamiec generatora znakow:
 *                ROMRAM - ROM+RAM
 *                RAM - tylko RAM
 *    disp_mode - tryb wyswietlania znakow:
 *                OR - funkcja "or" tekstu i grafiki
 *                AND - funkcja "and" tekstu i grafiki
 *                XOR - funkcja "xor" tekstu i grafiki
 *                TEXT_ONLY - wyswietlanie grafiki wylaczone,
 *                  przestrzen pamieci grafiki przyporzadkowana
 *                  przestrzeni atrybutow tekstu
 */
void Disp_set_cg_mode(uint8_t templ_mem, uint8_t disp_mode) {
  uint8_t temp = MODE_CGROM;
  
  if (templ_mem == RAM)
    temp |= MODE_CGRAM;
  
  if (disp_mode == OR)
    temp |= M_OR;
  else if (disp_mode == AND)
    temp |= M_AND;
  else if (disp_mode == XOR)
    temp |= M_EXOR;
  else
    temp |= M_ATTR;
    
  write_buffer(temp, COMMAND | STA0 | STA1);
}

/*
 * Przesyla do wyswietlacza dane z bufora komunikacyjnego.
 * W przypadku implementacji w systemie operacyjnym niniejsza
 * funkcja powinna byc wykonywana cyklicznie jako zadanie.
 * Moze byc ona takze wywolywana przez dowolne, cykliczne przerwanie.
 * 
 * zwraca:
 *    0 - jesli wyswietlacz pracuje poprawnie
 *    1 - jesli wystapil blad (np. timeout)
 */
uint8_t Disp_update(void) {
  uint8_t condition;
  uint8_t data = 0;
  
  while (1) {
    condition = read_buffer(&data);
  
    if (condition == EMPTY)
      break;
    
    if (!(condition & COMMAND))
      write_data(data, condition);
    else
      send_cmd(data, condition & (~COMMAND));
  }
  
  return error || overflow;
}

/*
 * Zmienia tryb odswiezania wyswietlacza.
 * 
 * parametry:
 *    new_update - okresla, czy komendy i dane maja byc
 *        przesylane natychmiast (IMMEDIATELY) czy wylacznie w trakcie
 *        wykonywania procedury Disp_update (DEFERRED)
 */
void Disp_set_update(uint8_t new_update) {
  update = new_update;
  
  if (update == IMMEDIATELY)
    Disp_update();
}

/*
 * Sprawdza tryb odswiezania wyswietlacza.
 * 
 * zwraca:
 *    IMMEDIATELY - jesli komendy i dane maja byc przesylane natychmiast
 *    DEFERRED    - jesli komendy i dane przesylane wylacznie w trakcie
 *                  wykonywania procedury Disp_update()
 */
uint8_t Disp_get_update(void) {
  return update;
}

/*
 * Zeruje bufor komunikacyjny.
 */
void Disp_flush_buffer(void) {
  fifo_rd_ptr = fifo;
  fifo_wr_ptr = fifo;
  overflow = 0;
}

/*
 * Sprawdza czy zaszlo przepelnienie bufora.
 * 
 * zwraca:
 *    0 - jesli bufor funkcjonuje poprawnie
 *    1 - jesli zaszlo przepelnienie bufora 
 */
uint8_t Disp_get_ovf(void) {
  return overflow;
}

/*
 * Zmienia pozycje kursora znakowego.
 * 
 * parametry:
 *    cx - numer kolumny znakow
 *    cy - numer linii znakow
 */
void Disp_set_cxy(uint8_t cx, uint8_t cy) {
  if ((cur_x == cx) && (cur_y == cy) && (graph_txt_pointer == TEXT))
    return;
  
  cur_x = cx;
  cur_y = cy;
  
  if (settings & D_CURSOR) {
    write_buffer(cx, STA0 | STA1);
    write_buffer(cy, STA0 | STA1);
    write_buffer(POINTER_CUR, COMMAND | STA0 | STA1);
  }
  
  uint16_t addr;
  
  addr = DISP_TEXT_HOME + cx;
  
  #if DISP_SCR_Y_NUM > 1
    if (cy >= DISP_C_ROWS_NUM/DISP_SCR_Y_NUM) {
      cy -= DISP_C_ROWS_NUM/DISP_SCR_Y_NUM;
      addr += DISP_2SCR_OFFSET; 
    }
  #endif
  
  addr += DISP_C_COLS_NUM*cy;
  
  graph_txt_pointer = TEXT;
  write_buffer((uint8_t)((addr & 0x00FF) >> 0), STA0 | STA1);
  write_buffer((uint8_t)((addr & 0xFF00) >> 8), STA0 | STA1);
  write_buffer(POINTER_ADDR, COMMAND | STA0 | STA1);
}

/*
 * Odczytuje aktualna pozycje kursora znakowego.
 * 
 * parametry:
 *    cx - wskaznik docelowej zmiennej numeru kolumny znakow
 *    cy - wskaznik docelowej zmiennej numeru linii znakow
 */
void Disp_get_cxy(uint8_t* cx, uint8_t* cy) {
  *cx = cur_x;
  *cy = cur_y;
}

/*
 * Odczytuje aktualna pozycje kursora znakowego w osi x.
 * 
 * zwraca:
 *    numer kolumny znakow
 */
uint8_t Disp_get_cx(void) {
  return cur_x;
}

/*
 * Odczytuje aktualna pozycje kursora znakowego w osi y.
 * 
 * zwraca:
 *    numer linii znakow
 */
uint8_t Disp_get_cy(void) {
  return cur_y;
}

/*
 * Przesuwa kursor znakowy o jedna pozycje.
 * 
 * parametry:
 *    dir - w prawo (RIGHT) lub w lewo (LEFT)
 */
void Disp_move_txt_cur(uint8_t dir) {
  uint8_t cx = cur_x;
  uint8_t cy = cur_y;
  
  if (dir == RIGHT) {
    if (++cx == DISP_C_COLS_NUM) {
      cx = 0;
      if (++cy == DISP_C_ROWS_NUM)
        cy = 0;
    }
  } else {
    if (cx-- == 0) {
      cx = DISP_C_COLS_NUM - 1;
      if (cy-- == 0)
        cy = DISP_C_ROWS_NUM - 1;
    }
  }
        
  Disp_set_cxy(cx, cy);
}

/*
 * Przesuwa okno o jedna pozycje.
 * 
 * parametry:
 *    dir - w prawo (RIGHT) lub w lewo (LEFT)
 */ 
void Disp_move_txt_window(uint8_t dir) {

  if (dir == RIGHT)
    txt_shift--;
  else
    txt_shift++;
  
  write_buffer((uint8_t)(((DISP_TEXT_HOME + txt_shift) & 0x00FF) >> 0), STA0 | STA1);
  write_buffer((uint8_t)(((DISP_TEXT_HOME + txt_shift) & 0xFF00) >> 8), STA0 | STA1);
  write_buffer(ADDR_TEXT_HOME, COMMAND | STA0 | STA1);
}

/*
 * Przesuwa okno w gore lub w dol.
 * 
 * parametry:
 *    dir - w gore (UP) lub w dol (DOWN)
 */ 
void Disp_scroll_txt_window(uint8_t dir) {

  if (dir == UP)
    txt_shift += DISP_C_COLS_NUM;
  else
    txt_shift -= DISP_C_COLS_NUM;
  
  write_buffer((uint8_t)(((DISP_TEXT_HOME + txt_shift) & 0x00FF) >> 0), STA0 | STA1);
  write_buffer((uint8_t)(((DISP_TEXT_HOME + txt_shift) & 0xFF00) >> 8), STA0 | STA1);
  write_buffer(ADDR_TEXT_HOME, COMMAND | STA0 | STA1);
}

/*
 * Czysci pole tekstowe wyswietlacza.
 */
void Disp_clr_txt(void) {
  txt_shift = 0;
  write_buffer((uint8_t)((DISP_TEXT_HOME & 0x00FF) >> 0), STA0 | STA1);
  write_buffer((uint8_t)((DISP_TEXT_HOME & 0xFF00) >> 8), STA0 | STA1);
  write_buffer(ADDR_TEXT_HOME, COMMAND | STA0 | STA1);
  
  graph_txt_pointer = OTHER;
  write_buffer((uint8_t)((DISP_TEXT_HOME & 0x00FF) >> 0), STA0 | STA1);
  write_buffer((uint8_t)((DISP_TEXT_HOME & 0xFF00) >> 8), STA0 | STA1);
  write_buffer(POINTER_ADDR, COMMAND | STA0 | STA1);
  
  write_buffer(DATA_A_WR, COMMAND | STA0 | STA1);
  
  for (uint16_t i = 0; i < (DISP_C_ROWS_NUM/DISP_SCR_Y_NUM) * DISP_C_COLS_NUM; i++)
    write_buffer(BLANK_CHR, STA3);
  
  write_buffer(DATA_A_RST, COMMAND | STA3);
  
  #if DISP_SCR_Y_NUM > 1
    write_buffer((uint8_t)(((DISP_TEXT_HOME + DISP_2SCR_OFFSET) & 0x00FF) >> 0), STA0 | STA1);
    write_buffer((uint8_t)(((DISP_TEXT_HOME + DISP_2SCR_OFFSET) & 0xFF00) >> 8), STA0 | STA1);
    write_buffer(POINTER_ADDR, COMMAND | STA0 | STA1);
    
    write_buffer(DATA_A_WR, COMMAND | STA0 | STA1);

    for (uint16_t i = 0; i < (DISP_C_ROWS_NUM/DISP_SCR_Y_NUM) * DISP_C_COLS_NUM; i++)
      write_buffer(BLANK_CHR, STA3);
    
    write_buffer(DATA_A_RST, COMMAND | STA3);
  #endif
  
  entry_mode = INC;
  Disp_set_cxy(0, 0);
}

/*
 * Czysci pole graficzne wyswietlacza.
 */
void Disp_clr_graph(void) {
  graph_txt_pointer = OTHER;
  write_buffer((uint8_t)((DISP_GRAPH_HOME & 0x00FF) >> 0), STA0 | STA1);
  write_buffer((uint8_t)((DISP_GRAPH_HOME & 0xFF00) >> 8), STA0 | STA1);
  write_buffer(POINTER_ADDR, COMMAND | STA0 | STA1);
  
  write_buffer(DATA_A_WR, COMMAND | STA0 | STA1);
  
  for (uint16_t i = 0; i < (DISP_P_ROWS_NUM/DISP_SCR_Y_NUM) * DISP_P_COLS_NUM / DISP_CHAR_WIDTH; i++)
    write_buffer(0x00, STA3);
  
  write_buffer(DATA_A_RST, COMMAND | STA3);
  
  #if DISP_SCR_Y_NUM > 1
    write_buffer((uint8_t)(((DISP_GRAPH_HOME + DISP_2SCR_OFFSET) & 0x00FF) >> 0), STA0 | STA1);
    write_buffer((uint8_t)(((DISP_GRAPH_HOME + DISP_2SCR_OFFSET) & 0xFF00) >> 8), STA0 | STA1);
    write_buffer(POINTER_ADDR, COMMAND | STA0 | STA1);
    
    write_buffer(DATA_A_WR, COMMAND | STA0 | STA1);
    
    for (uint16_t i = 0; i < (DISP_P_ROWS_NUM/DISP_SCR_Y_NUM) * DISP_P_COLS_NUM / DISP_CHAR_WIDTH; i++)
      write_buffer(0x00, STA3);
    
    write_buffer(DATA_A_RST, COMMAND | STA3);
  #endif
  
  entry_mode = INC;
  Disp_set_pxy(0, 0);
}

/*
 * Czysci zawartość wyswietlacza.
 */
__inline__ void Disp_clr(void) {
  Disp_clr_graph();
  Disp_clr_txt();
}

/*
 * Przywraca kursor i okno do pozycji poczatkowej.
 */
void Disp_ret_cur(void) {
  txt_shift = 0;
  write_buffer((uint8_t)((DISP_TEXT_HOME & 0x00FF) >> 0), STA0 | STA1);
  write_buffer((uint8_t)((DISP_TEXT_HOME & 0xFF00) >> 8), STA0 | STA1);
  write_buffer(ADDR_TEXT_HOME, COMMAND | STA0 | STA1);
  
  pix_x = 0;
  pix_y = 0;
  
  entry_mode = INC;
  Disp_set_cxy(0, 0);
}

/*
 * Wyswietla znak na aktualnej pozycji kursora.
 * 
 * parametry:
 *    character - kod znaku do wyswietlenia
 */
void Disp_putc(char character) {
  if (graph_txt_pointer != TEXT)
    Disp_set_cxy(cur_x, cur_y);
  
  write_buffer(character - 0x20, STA0 | STA1);
  write_buffer((entry_mode == DEC) ? \
                (DATA_WR | D_RW_PTR_DOWN) : \
                DATA_WR, COMMAND | STA0 | STA1);
  
  update_cursor();
}

/*
 * Pobiera znak z aktualnej pozycji kursora.
 * 
 * zwraca:
 *    kod znaku na aktualnej pozycji kursora
 */
char Disp_getc(void) {
  
  #if DISP_UPDATE_ON_READ == 1
    Disp_update();
  #endif
  
  if (graph_txt_pointer != TEXT) {
    Disp_set_cxy(cur_x, cur_y);
    Disp_update();
  }
  
  send_cmd((entry_mode == DEC) ? \
                (DATA_RD | D_RW_PTR_DOWN) : \
                DATA_RD, STA0 | STA1);
    
  char temp = (char)read_data(STA0 | STA1);
  
  update_cursor();
  
  return temp + 0x20;
}

/*
 * Wyswietla lancuch znakow poczawszy od aktualnej pozycji kursora.
 * 
 * parametry:
 *    string - wskaznik poczatku lancucha znakow
 */
void Disp_puts(char* string) {
  if (*string == 0)
    return;
    
  if (graph_txt_pointer != TEXT)
    Disp_set_cxy(cur_x, cur_y);
  
  if (entry_mode == INC) {
    write_buffer(DATA_A_WR, COMMAND | STA0 | STA1);
  
    while (*string) {
      write_buffer(*(string++) - 0x20, STA3);
      cur_x++;
    }
    
    write_buffer(DATA_A_RST, COMMAND | STA3);
    
    cur_y += cur_x / (uint8_t)DISP_C_COLS_NUM;
    cur_x %= (uint8_t)DISP_C_COLS_NUM;
  
    if (settings & D_CURSOR) {
      write_buffer(cur_x, STA0 | STA1);
      write_buffer(cur_y, STA0 | STA1);
      write_buffer(POINTER_CUR, COMMAND | STA0 | STA1);
    }
  } else {

    while (*string) {
      Disp_putc(*string++);
    }
  }
}

/*
 * Wyswietla staly lancuch znakow poczawszy od aktualnej pozycji kursora.
 * 
 * parametry:
 *    string - wskaznik poczatku lancucha znakow
 */
void Disp_puts_const(const char* string) {
  #if DISP_PLATFORM == AVR

    if (get_const((const uint8_t*)(string)) == 0)
      return;
      
    if (graph_txt_pointer != TEXT)
      Disp_set_cxy(cur_x, cur_y);
      
    if (entry_mode == INC) {
      write_buffer(DATA_A_WR, COMMAND | STA0 | STA1);
    
      while (get_const((const uint8_t*)(string))) {
        write_buffer(get_const((const uint8_t*)(string++)) - 0x20, STA3);
        cur_x++;
      }
      
      write_buffer(DATA_A_RST, COMMAND | STA3);
      
      cur_y += cur_x / (uint8_t)DISP_C_COLS_NUM;
      cur_x %= (uint8_t)DISP_C_COLS_NUM;
    
      if (settings & D_CURSOR) {
        write_buffer(cur_x, STA0 | STA1);
        write_buffer(cur_y, STA0 | STA1);
        write_buffer(POINTER_CUR, COMMAND | STA0 | STA1);
      }
    } else {
  
      while (get_const((const uint8_t*)(string))) {
        Disp_putc(get_const((const uint8_t*)(string++)));
      }
    }

  #else
  
    Disp_puts((char*)string);
    
  #endif
}

/*
 * Pobiera lancuch znakow poczawszy od aktualnej pozycji kursora.
 * 
 * parametry:
 *    string - wskaznik poczatku docelowego lancucha znakow
 *    count  - liczba znakow do odczytania
 */
void Disp_gets(char* string, uint8_t count) {
  if (count == 0) {
    *string = 0;
    return;
  }
      
  #if DISP_UPDATE_ON_READ == 1
    Disp_update();
  #endif
  
  if (graph_txt_pointer != TEXT) {
    Disp_set_cxy(cur_x, cur_y);
    Disp_update();
  }
  
  if (entry_mode == INC) {
    cur_x += count;
    
    while (count--) {
      send_cmd(DATA_RD, STA0 | STA1);
      *(string++) = (char)(read_data(STA0 | STA1) + 0x20);
    }
    
    *string = 0;
    
    cur_y += cur_x / (uint8_t)DISP_C_COLS_NUM;
    cur_x %= (uint8_t)DISP_C_COLS_NUM;
  
    if (settings & D_CURSOR) {
      write_buffer(cur_x, STA0 | STA1);
      write_buffer(cur_y, STA0 | STA1);
      write_buffer(POINTER_CUR, COMMAND | STA0 | STA1);
    }
  } else {
    
    while (count--)
      *(string++) = Disp_getc();
      
    *string = 0;
  }
}

/*
 * Wybiera tablice szablonow znakowych.
 * 
 * parametry:
 *    templates - wskaznik poczatku tablicy szablonow
 *    count     - dlugosc tablicy szablonow w bajtach
 *    addr      - numer pierwszego szablonu do zapisania
 */
void Disp_set_ctempl(uint8_t* templates, uint16_t count, uint8_t addr) {
  if (count == 0)
    return;
    
  #if DISP_SCR_Y_NUM > 1
    uint8_t* c_templates = templates;
    uint16_t c_count = count;
  #endif
  
  graph_txt_pointer = OTHER;
  write_buffer((uint8_t)(((DISP_CGRAM_HOME | (addr << 3)) & 0x00FF) >> 0), STA0 | STA1);
  write_buffer((uint8_t)(((DISP_CGRAM_HOME | (addr << 3)) & 0xFF00) >> 8), STA0 | STA1);
  write_buffer(POINTER_ADDR, COMMAND | STA0 | STA1);
 
  write_buffer(DATA_A_WR, COMMAND | STA0 | STA1);

  while (count--)
    write_buffer(*(templates++), STA3);
  
  write_buffer(DATA_A_RST, COMMAND | STA3);
  
  #if DISP_SCR_Y_NUM > 1
    templates = c_templates;
    count = c_count;
    
    write_buffer((uint8_t)((((DISP_CGRAM_HOME + DISP_2SCR_OFFSET) | (addr << 3)) & 0x00FF) >> 0), STA0 | STA1);
    write_buffer((uint8_t)((((DISP_CGRAM_HOME + DISP_2SCR_OFFSET) | (addr << 3)) & 0xFF00) >> 8), STA0 | STA1);
    write_buffer(POINTER_ADDR, COMMAND | STA0 | STA1);
   
    write_buffer(DATA_A_WR, COMMAND | STA0 | STA1);
  
    while (count--)
      write_buffer(*(templates++), STA3);
    
    write_buffer(DATA_A_RST, COMMAND | STA3);
  #endif
}

/*
 * Wybiera stala tablice szablonow znakowych.
 * 
 * parametry:
 *    templates - wskaznik poczatku tablicy szablonow
 *    count     - dlugosc tablicy szablonow w bajtach
 *    addr      - numer pierwszego szablonu do zapisaniazapisania
 */
void Disp_set_ctempl_const(const uint8_t* templates, uint16_t count, uint8_t addr) {
    
  #if DISP_PLATFORM == AVR
  
    if (count == 0)
      return;
      
    #if DISP_SCR_Y_NUM > 1
      const uint8_t* c_templates = templates;
      uint16_t c_count = count;
    #endif
    
    graph_txt_pointer = OTHER;
    write_buffer((uint8_t)(((DISP_CGRAM_HOME | (addr << 3)) & 0x00FF) >> 0), STA0 | STA1);
    write_buffer((uint8_t)(((DISP_CGRAM_HOME | (addr << 3)) & 0xFF00) >> 8), STA0 | STA1);
    write_buffer(POINTER_ADDR, COMMAND | STA0 | STA1);
   
    write_buffer(DATA_A_WR, COMMAND | STA0 | STA1);
  
    while (count--)
      write_buffer(get_const(templates++), STA3);
    
    write_buffer(DATA_A_RST, COMMAND | STA3);
    
    #if DISP_SCR_Y_NUM > 1
      templates = c_templates; 
      count = c_count;
      
      write_buffer((uint8_t)((((DISP_CGRAM_HOME + DISP_2SCR_OFFSET) | (addr << 3)) & 0x00FF) >> 0), STA0 | STA1);
      write_buffer((uint8_t)((((DISP_CGRAM_HOME + DISP_2SCR_OFFSET) | (addr << 3)) & 0xFF00) >> 8), STA0 | STA1);
      write_buffer(POINTER_ADDR, COMMAND | STA0 | STA1);
     
      write_buffer(DATA_A_WR, COMMAND | STA0 | STA1);
    
      while (count--)
        write_buffer(get_const(templates++), STA3);
      
      write_buffer(DATA_A_RST, COMMAND | STA3);
    #endif
      
  #else
  
    Disp_set_ctempl((uint8_t*)templates, count, addr);
    
  #endif
}

/*
 * Odczytuje tablice szablonow znakowych z wyswietlacza.
 * 
 * parametry:
 *    templates - wskaznik poczatku docelowej tablicy szablonow
 *    count     - dlugosc tablicy szablonow w bajtach
 *    addr      - numer pierwszego szablonu do odczytania
 */
void Disp_get_ctempl(uint8_t* templates, uint16_t count, uint8_t addr) { 
  if (count == 0)
    return;
  
  graph_txt_pointer = OTHER;
  write_data((uint8_t)(((DISP_CGRAM_HOME | (addr << 3)) & 0x00FF) >> 0), STA0 | STA1);
  write_data((uint8_t)(((DISP_CGRAM_HOME | (addr << 3)) & 0xFF00) >> 8), STA0 | STA1);
  send_cmd(POINTER_ADDR, STA0 | STA1);
  
  while (count--) {
    send_cmd(DATA_RD, STA0 | STA1);
    *(templates++) = read_data(STA0 | STA1);
  }
}

/*
 * Zmienia pozycje kursora punktowego.
 * 
 * parametry:
 *    px - numer kolumny pikseli
 *    py - numer linii pikseli
 */
void Disp_set_pxy(uint8_t px, uint8_t py) {
  if ((pix_y == py) && ((pix_x / DISP_CHAR_WIDTH) == (px / DISP_CHAR_WIDTH))
      && (graph_txt_pointer == GRAPH)) {
    pix_x = px;
    return;
  }
  
  pix_x = px;
  pix_y = py;
  
  uint16_t addr;
  
  addr = DISP_GRAPH_HOME + px/DISP_CHAR_WIDTH;
  
  #if DISP_SCR_Y_NUM > 1
    if (py >= DISP_P_ROWS_NUM/DISP_SCR_Y_NUM) {
      py -= DISP_P_ROWS_NUM/DISP_SCR_Y_NUM;
      addr += DISP_2SCR_OFFSET; 
    }
  #endif
  
  addr += DISP_P_BYTES_NUM*py;
  
  graph_txt_pointer = GRAPH;
  write_buffer((uint8_t)((addr & 0x00FF) >> 0), STA0 | STA1);
  write_buffer((uint8_t)((addr & 0xFF00) >> 8), STA0 | STA1);
  write_buffer(POINTER_ADDR, COMMAND | STA0 | STA1);
}

/*
 * Odczytuje aktualna pozycje kursora punktowego.
 * 
 * parametry:
 *    px - wskaznik docelowej zmiennej numeru kolumny pikseli
 *    py - wskaznik docelowej zmiennej numeru linii pikseli
 */
void Disp_get_pxy(uint8_t* px, uint8_t* py) {
  *px = pix_x;
  *py = pix_y;
}

/*
 * Odczytuje aktualna pozycje kursora punktowego w osi x.
 * 
 * zwraca:
 *    numer kolumny pikseli
 */
uint8_t Disp_get_px(void) {
  return pix_x;
}

/*
 * Odczytuje aktualna pozycje kursora punktowego w osi y.
 * 
 * zwraca:
 *    numer linii pikseli
 */
uint8_t Disp_get_py(void) {
  return pix_y;
}

/*
 * Aktywuje badz deaktywuje piksel na aktualnej pozycji kursora.
 * 
 * parametry:
 *    pixel - piksel aktywny (ON) badz nieaktywny (OFF)
 */
void Disp_putp(uint8_t pixel) {
  uint8_t temp;
  
  if (graph_txt_pointer != GRAPH)
    Disp_set_pxy(pix_x, pix_y);
  
  if (pixel == ON)
    temp = BIT_SET;
  else
    temp = BIT_RST;
    
  write_buffer(temp | ((DISP_CHAR_WIDTH - 1) - (pix_x % DISP_CHAR_WIDTH)), \
              COMMAND | STA0 | STA1);
}

/*
 * Pobiera piksel z aktualnej pozycji kursora.
 * 
 * zwraca:
 *    ON  - piksel aktywny
 *    OFF - piksel nieaktywny
 */
uint8_t Disp_getp(void) {
  #if DISP_UPDATE_ON_READ == 1
    Disp_update();
  #endif
  
  if (graph_txt_pointer != GRAPH) {
    Disp_set_pxy(pix_x, pix_y);
    Disp_update();
  }
  
  send_cmd(DATA_RD | D_RW_PTR_NOCH, STA0 | STA1);
  uint8_t temp = read_data(STA0 | STA1);
  temp &= (1 << ((DISP_CHAR_WIDTH - 1) - (pix_x % DISP_CHAR_WIDTH)));
  
  return temp? ON : OFF;
}

/*
 * Rysuje linie pozioma do wyznaczonego punktu.
 * 
 * parametry:
 *    px        - wspolrzedna "x" drugiego punktu
 *    color     - linia aktywna (ON) badz nieaktywna (OFF)
 */
void Disp_hlineto(uint8_t px, uint8_t color) {
  uint8_t dx;
  
  if (px >= pix_x)
    dx = px - pix_x;
  else {
    dx = pix_x - px;
    Disp_set_pxy(px, pix_y);
  }
  
  if (graph_txt_pointer != GRAPH)
    Disp_set_pxy(pix_x, pix_y);
  
  while ((dx != 0) && (pix_x % DISP_CHAR_WIDTH != 0)) {
    Disp_putp(color);
    Disp_set_pxy(pix_x + 1, pix_y);
    dx--;
  }
  
  if (dx >= DISP_CHAR_WIDTH) {
    uint8_t line_temp = ((color == ON)? 0xFF : 0x00);
    
    write_buffer(DATA_A_WR, COMMAND | STA0 | STA1);
    
    while (dx >= DISP_CHAR_WIDTH) {
      write_buffer(line_temp, STA3);
      dx -= DISP_CHAR_WIDTH;
      pix_x += DISP_CHAR_WIDTH;
    }
    
    write_buffer(DATA_A_RST, COMMAND | STA3);
  }
  
  while (dx != 0) {
    Disp_putp(color);
    pix_x++;
    dx--;
  }
  
  Disp_putp(color);
  
  Disp_set_pxy(px, pix_y);
}

/*
 * Rysuje linie pionowa do wyznaczonego punktu.
 * 
 * parametry:
 *    py        - wspolrzedna "y" drugiego punktu
 *    color     - linia aktywna (ON) badz nieaktywna (OFF)
 */
void Disp_vlineto(uint8_t py, uint8_t color) {
  uint8_t dir;
  uint8_t dy;
  
  if (py - pix_y >= 0) {
    dy = py - pix_y;
    dir = 1;
  } else {
    dy = pix_y - py;
    dir = -1;
  }
  
  dy++;

  while (dy != 0) {
    Disp_putp(color);
    Disp_set_pxy(pix_x, pix_y + dir);
    dy--;
  }
  
  Disp_set_pxy(pix_x, py);
}
 
/*
 * Rysuje linie z aktualnej pozycji kursora do wyznaczonego punktu.
 * 
 * parametry:
 *    px, py    - wspolrzedne punktu koncowego
 *    color     - linia aktywna (ON) badz nieaktywna (OFF)
 */
#if DISP_NOC_LINE == 0
void Disp_lineto(uint8_t px, uint8_t py, \
                      uint8_t color) {
  uint8_t dx;
  uint8_t dy;
  uint8_t start_px;
  uint8_t start_py;
  uint8_t sign;
  
  if (px >= pix_x) {
    dx = px - pix_x;
    start_px = pix_x;
    sign = 1;
  } else {
    dx = pix_x - px;
    start_px = px;
    sign = -1;
  }
  if (py >= pix_y) {
    dy = py - pix_y;
    start_py = pix_y;
  } else {
    dy = pix_y - py;
    start_py = py;
    sign = -sign;
  }
  
  if (dx == 0)
    Disp_vlineto(py, color);
  else if (dy == 0)
    Disp_hlineto(px, color);
  else {
    dx++;
    dy++;
    uint16_t a;
    if (dx >= dy) {
      a = (dy << 8) / dx;
      
      if (start_px == pix_x)
        start_py = pix_y;
      else
        start_py = py;
      
      for (uint8_t i=0; i<dx; i++) {
        Disp_set_pxy(start_px + i, start_py + sign*((uint16_t)(i * a) >> 8));
        Disp_putp(color);
      }
    } else {
      a = (dx << 8) / dy;
      
      if (start_py == pix_y)
        start_px = pix_x;
      else
        start_px = px;
      
      for (uint8_t i=0; i<dy; i++) {
        Disp_set_pxy(start_px + sign*((uint16_t)(i * a) >> 8), start_py + i);
        Disp_putp(color);
      }
    }
    Disp_set_pxy(px, py);
  }
}
#endif

/*
 * Rysuje linie pomiedzy dwoma wyznaczonymi punktami.
 * 
 * parametry:
 *    px1, py1  - wspolrzedne pierwszego punktu
 *    px2, py2  - wspolrzedne drugiego punktu
 *    color     - linia aktywna (ON) badz nieaktywna (OFF)
 */
#if DISP_NOC_LINE == 0
__inline__ void Disp_line(uint8_t px1, uint8_t py1, \
                    uint8_t px2, uint8_t py2, \
                    uint8_t color) {
  Disp_set_pxy(px1, py1);
  Disp_lineto(px2, py2, color);
}
#endif

/*
 * Rysuje prostokat pomiedzy dwoma wyznaczonymi punktami.
 * 
 * parametry:
 *    px1, py1  - wspolrzedne pierwszego rogu
 *    px2, py2  - wspolrzedne drugiego rogu
 *    color     - prostokat aktywny (ON) badz nieaktywny (OFF)
 *    filled    - prostokat zaczerniony (YES) lub pusty (NO)
 */
void Disp_box(uint8_t px1, uint8_t py1, \
                   uint8_t px2, uint8_t py2, \
                   uint8_t color, uint8_t filled) {
  if (filled == YES) {
    uint8_t py0;
    uint8_t length;
    
    if (py1 <= py2) {
      length = py2 - py1;
      py0 = py1;
    } else {
      length = py1 - py2;
      py0 = py2;
    }
    
    for (uint8_t i = py0; i <= py0 + length; i++) {
      Disp_set_pxy(px1, i);
      Disp_hlineto(px2, color);
    }
    
    Disp_set_pxy(px1, py1);
  } else {
    Disp_set_pxy(px1, py1);
    Disp_hlineto(px2, color);
    Disp_vlineto(py2, color);
    Disp_hlineto(px1, color);
    Disp_vlineto(py1, color);
  }
}

/*
 * Rysuje okrag o srodku w aktualnej pozycji kursora.
 * 
 * parametry:
 *    rad   - promien okregu
 *    color - okrag aktywny (ON) badz nieaktywny (OFF)
 */
#if DISP_NOC_CIRC == 0
void Disp_circle(uint8_t rad, uint8_t color)
{
  uint8_t xc = 0;
  uint8_t yc = rad;
  int16_t i;
  
  uint8_t temp_px = pix_x;
  uint8_t temp_py = pix_y;

  i = 3 - (rad << 1);
  
  while (xc <= yc) {
    Disp_set_pxy(temp_px + xc, temp_py + yc);
    Disp_putp(color);
    Disp_set_pxy(temp_px + xc, temp_py - yc);
    Disp_putp(color);
    Disp_set_pxy(temp_px - xc, temp_py + yc);
    Disp_putp(color);
    Disp_set_pxy(temp_px - xc, temp_py - yc);
    Disp_putp(color);
    Disp_set_pxy(temp_px + yc, temp_py + xc);
    Disp_putp(color);
    Disp_set_pxy(temp_px + yc, temp_py - xc);
    Disp_putp(color);
    Disp_set_pxy(temp_px - yc, temp_py + xc);
    Disp_putp(color);
    Disp_set_pxy(temp_px - yc, temp_py - xc);
    Disp_putp(color);
    
    if (i < 0)
      i += (xc << 2) + 6;
    else
      i += ((xc - yc--) << 2) + 10;
      
    xc++;
  }
    
  Disp_set_pxy(temp_px, temp_py);
}
#endif
                   
/*
 * Wyswietla obraz poczawszy od aktualnej pozycji kursora.
 * 
 * parametry:
 *    picture  - wskaznik obrazu
 *    width    - szerokosc obrazu w pikselach (os x) 
 *    length   - dlugosc obrazu w pikselach (os y)
 */
#if DISP_NOC_PUT_PIC == 0
void Disp_put_picture(uint8_t* picture, \
                       uint8_t width, uint8_t length) {
                        
  uint8_t temp_px = pix_x;
  uint8_t temp_py = pix_y;
  uint8_t left_offset = temp_px % DISP_CHAR_WIDTH;
  uint8_t left_length = (DISP_CHAR_WIDTH - left_offset) % DISP_CHAR_WIDTH;
  uint8_t right_offset = (uint8_t)(temp_px + width) % DISP_CHAR_WIDTH;
  uint8_t width_bytes = (uint8_t)(width + 7) / 8;
  uint8_t disp_full_bytes = (uint8_t)(width - left_length) / DISP_CHAR_WIDTH;
  
  if (width < left_length)
    left_length = width;
      
  if (graph_txt_pointer != GRAPH) {
    Disp_set_pxy(temp_px, temp_py);
    Disp_update();
  }
  
  for (uint8_t y=0; y<length; y++) {
    uint8_t ptr_x = 0;
    uint8_t mem_temp;
    
    mem_temp = *(picture + y*width_bytes);
    
    for (uint8_t i=0x01; i!=(1<<left_length); i<<=1) {  
      if (mem_temp & i)
        Disp_putp(ON);
      else
        Disp_putp(OFF);
        
      Disp_set_pxy(pix_x + 1, pix_y);
    }
    
    ptr_x = left_length;
    
    if (disp_full_bytes != 0) {
      write_buffer(DATA_A_WR, COMMAND | STA0 | STA1);
    
      for (uint8_t x=0; x<disp_full_bytes; x++) {
        uint8_t line_temp = 0;
        
        for (uint8_t i=0; i<DISP_CHAR_WIDTH; i++) {
          line_temp <<= 1;
          
          if (*(picture + ptr_x/8 + y*width_bytes) & (1 << (ptr_x % 8)))
            line_temp |= 0x01;
            
          ptr_x++;
        }
        
        write_buffer(line_temp, STA3);
      }
        
      write_buffer(DATA_A_RST, COMMAND | STA3);
    }
    
    Disp_set_pxy(pix_x + disp_full_bytes*DISP_CHAR_WIDTH, pix_y);
    
    for (uint8_t i=0; i<right_offset; i++) {
      if (*(picture + ptr_x/8 + y*width_bytes) & (1 << ((ptr_x++) % 8)))
        Disp_putp(ON);
      else
        Disp_putp(OFF);
        
      Disp_set_pxy(pix_x + 1, pix_y);
    }
    Disp_set_pxy(temp_px, pix_y + 1);    
  }
  Disp_set_pxy(temp_px, temp_py);
}
#endif

/*
 * Wyswietla staly obraz poczawszy od aktualnej pozycji kursora.
 * 
 * parametry:
 *    picture  - wskaznik obrazu
 *    width    - szerokosc obrazu w pikselach (os x) 
 *    length   - dlugosc obrazu w pikselach (os y)
 */
#if DISP_NOC_PUT_CPIC == 0
void Disp_put_picture_const(const uint8_t* picture, \
                       uint8_t width, uint8_t length) {

  uint8_t temp_px = pix_x;
  uint8_t temp_py = pix_y;
  uint8_t left_offset = temp_px % DISP_CHAR_WIDTH;
  uint8_t left_length = (DISP_CHAR_WIDTH - left_offset) % DISP_CHAR_WIDTH;
  uint8_t right_offset = (uint8_t)(temp_px + width) % DISP_CHAR_WIDTH;
  uint8_t width_bytes = (uint8_t)(width + 7) / 8;
  uint8_t disp_full_bytes = (uint8_t)(width - left_length) / DISP_CHAR_WIDTH;
  
  if (width < left_length)
    left_length = width;
      
  if (graph_txt_pointer != GRAPH) {
    Disp_set_pxy(temp_px, temp_py);
    Disp_update();
  }
  
  for (uint8_t y=0; y<length; y++) {
    uint8_t ptr_x = 0;
    uint8_t mem_temp;
    
    mem_temp = get_const(picture + y*width_bytes);
    
    for (uint8_t i=0x01; i!=(1<<left_length); i<<=1) {  
      if (mem_temp & i)
        Disp_putp(ON);
      else
        Disp_putp(OFF);
        
      Disp_set_pxy(pix_x + 1, pix_y);
    }
    
    ptr_x = left_length;
    
    if (disp_full_bytes != 0) {
      write_buffer(DATA_A_WR, COMMAND | STA0 | STA1);
    
      for (uint8_t x=0; x<disp_full_bytes; x++) {
        uint8_t line_temp = 0;
        
        for (uint8_t i=0; i<DISP_CHAR_WIDTH; i++) {
          line_temp <<= 1;
          
          if (get_const(picture + ptr_x/8 + y*width_bytes) & (1 << (ptr_x % 8)))
            line_temp |= 0x01;
            
          ptr_x++;
        }
        
        write_buffer(line_temp, STA3);
      }
        
      write_buffer(DATA_A_RST, COMMAND | STA3);
    }
    
    Disp_set_pxy(pix_x + disp_full_bytes*DISP_CHAR_WIDTH, pix_y);
    
    for (uint8_t i=0; i<right_offset; i++) {
      if (get_const(picture + ptr_x/8 + y*width_bytes) & (1 << ((ptr_x++) % 8)))
        Disp_putp(ON);
      else
        Disp_putp(OFF);
        
      Disp_set_pxy(pix_x + 1, pix_y);
    }
    Disp_set_pxy(temp_px, pix_y + 1);    
  }
  Disp_set_pxy(temp_px, temp_py);
}
#endif

/*
 * Pobiera obraz poczawszy od aktualnej pozycji kursora.
 * 
 * parametry:
 *    picture  - wskaznik docelowej zmiennej obrazu
 *    width    - szerokosc obrazu w pikselach (os x) 
 *    length   - dlugosc obrazu w pikselach (os y)
 * 
 * zwraca:
 *    liczbe pobranych bajtow
 */
#if DISP_NOC_GET_PIC == 0
uint16_t Disp_get_picture(uint8_t* picture, \
                      uint8_t width, uint8_t length) {
                        
  uint8_t temp_px = pix_x;
  uint8_t temp_py = pix_y;
  uint8_t left_offset = temp_px % DISP_CHAR_WIDTH;
  uint8_t left_length = (DISP_CHAR_WIDTH - left_offset) % DISP_CHAR_WIDTH;
  uint8_t width_bytes = (uint8_t)(width + 7) / 8;
  
  if (width < left_length)
    left_length = width;

  if (graph_txt_pointer != GRAPH) {
    Disp_set_pxy(temp_px, temp_py);
    Disp_update();
  }
  
  if (length == 0 || width == 0) {
    *picture = 0;
    return 0;
  }
  
  for (uint8_t y=0; y<length; y++) {
    uint16_t line_temp = 0;
    uint8_t line_depth = left_length;
    uint8_t read_temp;
    
    if (left_length != 0) {
      
      send_cmd(DATA_RD, STA0 | STA1);
      read_temp = read_data(STA0 | STA1);
      
      for (uint8_t i = (1 << (left_length - 1)); i != 0; i >>= 1) {
        line_temp >>= 1;
        
        if (read_temp & i)
          line_temp |= 0x8000;
      }
    }

    for (uint8_t x = 0; x < width_bytes; x++) {
      while (line_depth < 8) {
        send_cmd(DATA_RD, STA0 | STA1);
        read_temp = read_data(STA0 | STA1);
    
        for (uint8_t i = (1 << (DISP_CHAR_WIDTH - 1)); i != 0; i >>= 1) {
          line_temp >>= 1;
          
          if (read_temp & i)
            line_temp |= 0x8000;
        }

        line_depth += DISP_CHAR_WIDTH;
      }
      
      *(picture + x + y*width_bytes) = (uint8_t)(line_temp >> (16 - line_depth));
      
      line_depth -= 8;
    }
    
    Disp_set_pxy(temp_px, pix_y + 1);
    Disp_update();  
  }
  Disp_set_pxy(temp_px, temp_py);
  
  return width_bytes*length;
}
#endif


/****************************************************************************
 * Deklaracje funkcji prywatnych
 ***************************************************************************/

/*
 * Uaktualnia zmienna kursora po operacji zapisu lub odczytu.
 */ 
static void update_cursor(void) {
  uint8_t cx = cur_x;
  uint8_t cy = cur_y;
  
  if (entry_mode == INC) {
    if (++cx == DISP_C_COLS_NUM) {
      cx = 0;
      if (++cy == DISP_C_ROWS_NUM) {
        cy = 0;
        Disp_set_cxy(cx, cy);
      }
      #if DISP_SCR_Y_NUM > 1
        else if (cy == DISP_C_ROWS_NUM/DISP_SCR_Y_NUM)
          Disp_set_cxy(cx, cy);
      #endif
    }
  } else {
    if (cx-- == 0) {
      cx = DISP_C_COLS_NUM - 1;
      if (cy-- == 0) {
        cy = DISP_C_ROWS_NUM - 1;
        Disp_set_cxy(cx, cy);
      }
      #if DISP_SCR_Y_NUM > 1
        else if (cy == DISP_C_ROWS_NUM/DISP_SCR_Y_NUM - 1)
          Disp_set_cxy(cx, cy);
      #endif
    }
  }
  cur_x = cx;
  cur_y = cy;
  
  if (settings & D_CURSOR) {
    write_buffer(cur_x, STA0 | STA1);
    write_buffer(cur_y, STA0 | STA1);
    write_buffer(POINTER_CUR, COMMAND | STA0 | STA1);
  }
}
 
/*
 * Oczekuje na gotowosc wyswietlacza.
 */
static uint8_t wait_if_busy(uint8_t condition) {
  uint8_t cd_old;
  uint8_t result = 0;
  uint16_t time_cnt = 0;
  
  cd_old = get_cd();

  chg_dir_db(IN);
  set_cd();
  
  while (!error) {
    rst_rd();
    
    nop();
    nop();
      
    result = read_db();
    set_rd();
    
    if ((result & condition) == condition)
      break;
    else if (++time_cnt > DISP_MAX_TIMEOUT * 1000) {
      error = 1;
      break;
    }

    nop();
  }
  
  chg_dir_db(OUT);
  
  if (!cd_old)
    rst_cd();
    
  return result;
}
 
/*
 * Przesyla 1 bajt danych do wyswietlacza.
 */
static void write_data(uint8_t data, uint8_t condition) {

  #if DISP_USE_CE == 1
    rst_ce();
  #endif
  wait_if_busy(condition);
  
  write_db(data);
  rst_wr();
  nop();
  set_wr();
  #if DISP_USE_CE == 1
    set_ce();
  #endif
}

/*
 * Przesyla komende.
 */
__inline__ static void send_cmd(uint8_t cmd, uint8_t condition) {
  set_cd();
  write_data(cmd, condition);
  rst_cd();
}

/*
 * Odczytuje 1 bajt danych z wyswietlacza.
 */
static uint8_t read_data(uint8_t condition) {
  uint8_t result;
  
  #if DISP_USE_CE == 1
    rst_ce();
  #endif
  wait_if_busy(condition);
  
  chg_dir_db(IN);  
  rst_rd();
  
  nop();
  nop();
    
  result = read_db();
      
  set_rd();
  chg_dir_db(OUT);
  #if DISP_USE_CE == 1
    set_ce();
  #endif
  
  return result;
}

/*
 * Zapisuje wartosc do bufora FIFO.
 */
static void write_buffer(uint8_t data, uint8_t condition) {
  if ((fifo_wr_ptr == fifo_rd_ptr - 2)  // jesli wystapilo przepelnienie:
     || ((fifo_wr_ptr == fifo + (DISP_BUFFER_LEN-1)*2)
         && (fifo_rd_ptr == fifo))) {
    
    #if DISP_ON_BUFFER_FULL == 1        //   przeslij zawartosc bufora
    
      Disp_update();
    
    #elif DISP_ON_BUFFER_FULL == 2      //   oczekuj na zwolnienie bufora
    
      while ((fifo_wr_ptr == fifo_rd_ptr - 2)
            || ((fifo_wr_ptr == fifo + (DISP_BUFFER_LEN-1)*2)
                && (fifo_rd_ptr == fifo)));
    
    #else                               //   pomin zapisywane wartosci
      
      overflow = 1;
      return;
  
    #endif
  }
  
  disable_interrupts();
  
  *(fifo_wr_ptr++) = condition;
  *(fifo_wr_ptr++) = data;
  
  if (fifo_wr_ptr == fifo + DISP_BUFFER_LEN*2)
      fifo_wr_ptr = fifo;
      
  revert_interrupts();
      
  if (update == IMMEDIATELY)
    Disp_update();
}

/*
 * Odczytuje wartosc z bufora FIFO.
 */
static uint8_t read_buffer(uint8_t* data) {
  if (fifo_rd_ptr == fifo_wr_ptr)
    return EMPTY;
  else {
    uint8_t condition;
    
    disable_interrupts();
    
    condition = *(fifo_rd_ptr++);
    *data = *(fifo_rd_ptr++);
    
    if (fifo_rd_ptr == fifo + DISP_BUFFER_LEN*2)
      fifo_rd_ptr = fifo;
    
    revert_interrupts();
    
    return condition;
  }
}
