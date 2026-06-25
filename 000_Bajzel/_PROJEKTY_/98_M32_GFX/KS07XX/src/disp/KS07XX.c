/****************************************************************************
* Biblioteka obslugi wyswietlacza graficznego, opartego na sterowniku
* KS07XX lub odpowiedniku.
*
* Nazwa pliku: KS07XX.c
* Data: 10.10.2007
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
#include <KS07XX.h>
#include <disp_conf.h>
#include <platf_conf.h>

#define EMPTY   0x80
#define COMMAND 0x40
#define RDMOWR  0x20
#define LOG_OR  0x10

/*
 * Definicje etykiet dla komend
 */
#define DISP_OFF        0xAE
#define DISP_ON         0xAF

#define SET_REF_VOLT    0x81
#define SET_REF_RES     0x20
#define SET_POWER       0x28

#define SET_PTR_OFFSET  0x40
#define SET_PTR_PAGE    0xB0
#define SET_PTR_COL_MSB 0x10
#define SET_PTR_COL_LSB 0x00

#define SET_NORM_COL    0xA0
#define SET_INV_COL     0xA1

#define SET_NORM_ROW    0xC0
#define SET_INV_ROW     0xC8

#define SET_NORMAL      0xA6
#define SET_NEGATIVE    0xA7

#define SET_STATIC      0xA5
#define RST_STATIC      0xA4

#define SET_BIAS_HIGH   0xA2
#define SET_BIAS_LOW    0xA3

#define SET_INDICATOR   0xAD
#define RST_INDICATOR   0xAC

#define ENTER_RDMOWR    0xE0
#define END_RDMOWR      0xEE

#define DISP_RESET      0xE2

#if DISP_CONTROLLER == 1 // dla SSD1303 i podobnych
  #define SET_CONTRAST  0x81
  #define SET_OSC       0xD5
  #define SET_PRECHARGE 0xD9
  #define SET_ROW_ORD   0xDA
  #define SET_ALT_ORD   0x12
  #define SET_NORM_ORD  0x02
#endif

/*
 * Definicje etykiet bitow stanu
 */
#define STAT_RESET      0x10
#define STAT_ON         0x20
#define STAT_ADC        0x40
#define STAT_BUSY       0x80

/*
 * Definicje makr.
 */
#if DISP_PLATFORM == AVR
#  define FONTS(x) get_const((const uint8_t*)(fonts + (x)))
#else
#  define FONTS(x) fonts[x]
#endif

/*
 * Definicje zmiennych prywatnych
 */
static volatile uint8_t error = 1;
static volatile uint8_t overflow = 0;
static volatile uint8_t update = DEFERRED;
static uint8_t cur_x;
static uint8_t cur_y;
static volatile uint8_t pix_x;
static volatile uint8_t pix_y;

static uint8_t entry_mode;
static uint8_t cursor;
static volatile uint8_t active_module;
static uint8_t shift;

static uint8_t fifo[DISP_BUFFER_LEN * 2];
static volatile uint8_t* fifo_rd_ptr;
static volatile uint8_t* fifo_wr_ptr;

static volatile uint8_t actual_x[DISP_CHIP_NUM];
static volatile uint8_t virtual_x[DISP_CHIP_NUM];
static volatile uint8_t virtual_y[DISP_CHIP_NUM];

static uint8_t* fonts;

/*
 * Definicje funkcji prywatnych
 */
static void update_virtual_x(uint8_t no);
static void update_actual_x(uint8_t no);

#if DISP_INTERF_TYPE == 1
  static void toggle_en(uint8_t chip_no, uint8_t on);
#elif DISP_INTERF_TYPE == 0
  static void toggle_cs(uint8_t chip_no, uint8_t on);
#endif

static void write_data(uint8_t data, uint8_t chip_no);
__inline__ static void send_cmd(uint8_t cmd, uint8_t chip_no);

static uint8_t read_data(uint8_t chip_no);

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

  #if DISP_INTERF_TYPE == 0
    set_rd();
    set_wr();
  #endif
  
  set_rs();
  
  delay_dus(100);
  set_res();
  
  error = 0;
  overflow = 0;
  cur_x = 0;
  cur_y = 0;
  pix_x = 0;
  pix_y = 0;
  active_module = 0;
  shift = 0;
  cursor = OFF;
  entry_mode = INC;
  
  Disp_flush_buffer();
  Disp_clr();
  
  for (uint8_t i=0; i<DISP_CHIP_NUM; i++) {
    if (DISP_INV_COL)
        write_buffer(SET_INV_COL, COMMAND | i);
      if (DISP_INV_ROW)
        write_buffer(SET_INV_ROW, COMMAND | i);
      if (DISP_NEGATIVE)
        write_buffer(SET_NEGATIVE, COMMAND | i);
        
    #if DISP_CONTROLLER == 0 // dla KS07XX i podobnych
      if (DISP_BIASES == 1)
        write_buffer(SET_BIAS_LOW, COMMAND | i);
      if (DISP_POWER == 1) {
        write_buffer(SET_POWER | 0x04, COMMAND | i);
        delay_ms(1);
        write_buffer(SET_POWER | 0x06, COMMAND | i);
        delay_ms(1);
        write_buffer(SET_POWER | 0x07, COMMAND | i);
      }
        
      write_buffer(SET_REF_VOLT, COMMAND | i);
      write_buffer(DISP_REF_VOLT, COMMAND | i);
      write_buffer(SET_REF_RES | (DISP_REF_RES & 0x07), COMMAND | i);
    #elif DISP_CONTROLLER == 1 // dla SSD1303 i podobnych
      if (DISP_CONTRAST != 0x80) {
        write_buffer(SET_CONTRAST, COMMAND | i);
        write_buffer(DISP_CONTRAST, COMMAND | i);
      }
      if ((DISP_OSC_FREQ != 7) || (DISP_OSC_DIV != 0)) {
        write_buffer(SET_OSC, COMMAND | i);
        write_buffer((((DISP_OSC_FREQ & 0x0F) << 4) || (DISP_OSC_DIV & 0x0F)),
                              COMMAND | i);
      }
      if ((DISP_PRECHARGE1 != 2) || (DISP_PRECHARGE2 != 2)) {
        write_buffer(SET_PRECHARGE, COMMAND | i);
        write_buffer((((DISP_PRECHARGE2 & 0x0F) << 4) || (DISP_PRECHARGE1 & 0x0F)),
                              COMMAND | i);
      }
      if (DISP_ALT_ROW) {
        write_buffer(SET_ROW_ORD, COMMAND | i);
        write_buffer(SET_NORM_ORD, COMMAND | i);
      }
    #endif
    
    write_buffer(DISP_ON, COMMAND | i);
  }
  
  return error || overflow;
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
  
  uint8_t settings = DISP_OFF;
  
  if (on == ON)
    settings = DISP_ON;
    
  for (uint8_t i=0; i<DISP_CHIP_NUM; i++)
    write_buffer(settings, COMMAND | i);
}

/*
 * Steruje zasilaniem wyswietlacza.
 * 
 * parametry:
 *    on    - wlaczony (ON) badz wylaczony (OFF)
 */
void Disp_set_power(uint8_t on) {
    
  if (on == OFF)
    for (uint8_t i=0; i<DISP_CHIP_NUM; i++) {
      write_buffer(DISP_OFF, COMMAND | i);
      write_buffer(SET_STATIC, COMMAND | i);
    }
  else
    for (uint8_t i=0; i<DISP_CHIP_NUM; i++) {
      write_buffer(RST_STATIC, COMMAND | i);
      write_buffer(DISP_ON, COMMAND | i);
    }
}

/*
 * Okresla kontrast wyswietlacza. (dla KS07XX i podobnych)
 * 
 * parametry:
 *    voltage  - napiecie zasilania LCD (0...63)
 *    divider  - podzial napiecia (0...7)
 *    negative - tryb normalny (OFF) lub negatywowy (ON)
 */
#if DISP_CONTROLLER == 0
void Disp_set_contrast(uint8_t voltage, uint8_t divider, uint8_t negative) {
  for (uint8_t i=0; i<DISP_CHIP_NUM; i++) {
    write_buffer(SET_REF_VOLT, COMMAND | i);
    write_buffer(voltage, COMMAND | i);
    write_buffer(SET_REF_RES | (divider & 0x07), COMMAND | i);
    write_buffer((negative == ON)?
                 SET_NEGATIVE : SET_NORMAL, COMMAND | i);
  }
}
#endif

/*
 * Okresla kontrast wyswietlacza. (dla SSD1303 i podobnych)
 * 
 * parametry:
 *    contrast - kontrast (0...255)
 *    negative - tryb normalny (OFF) lub negatywowy (ON)
 */
#if DISP_CONTROLLER == 1
void Disp_set_contrast(uint8_t contrast, uint8_t negative) {
  for (uint8_t i=0; i<DISP_CHIP_NUM; i++) {
    write_buffer(SET_CONTRAST, COMMAND | i);
    write_buffer(contrast, COMMAND | i);
    write_buffer((negative == ON)?
                 SET_NEGATIVE : SET_NORMAL, COMMAND | i);
  }
}
#endif

/*
 * Okresla atrybuty kursora znakowego.
 * 
 * parametry:
 *    on    - wlaczony (ON) badz wylaczony (OFF)
 */
void Disp_set_cur(uint8_t on) {
  uint8_t temp_px = pix_x;
  uint8_t temp_py = pix_y;
    
  uint8_t new_px = cur_x * 6 + 1;
  uint8_t new_py = cur_y * 8 + 7;
  
  Disp_set_pxy(new_px, new_py);
  Disp_hlineto(new_px + 4, on);
  
  cursor = on;
  
  pix_x = temp_px;
  pix_y = temp_py;
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
      
    uint8_t chip_no = condition & 0x0F;
    
    if (!(condition & COMMAND)) {
      
      if (!(condition & RDMOWR)) {
        
        write_data(data, chip_no);
        
      } else {
        uint8_t temp;
        
        read_data(chip_no); // dummy read needed!
        temp = read_data(chip_no);
        send_cmd(SET_PTR_COL_MSB | (actual_x[chip_no] >> 4), chip_no);
        send_cmd(SET_PTR_COL_LSB | (actual_x[chip_no] & 0x0F), chip_no);
        
        if (condition & LOG_OR)
          temp |= data;
        else
          temp &= ~(data);
        
        write_data(temp, chip_no);
      }
      
      update_actual_x(chip_no);
          
    } else {
      send_cmd(data, chip_no);
      if ((data & 0xF0) == SET_PTR_COL_MSB) {
        actual_x[chip_no] &= 0x0F;
        actual_x[chip_no] |= data << 4;
      }
      if ((data & 0xF0) == SET_PTR_COL_LSB) {
        actual_x[chip_no] &= 0xF0;
        actual_x[chip_no] |= data & 0x0F;
      }
    }
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
  if ((cx >= DISP_C_COLS_NUM) || (cy >= DISP_C_ROWS_NUM))
    return;
  
  if ((cur_x != cx) || (cur_y != cy)) {
    uint8_t temp_cursor = cursor;
    if (temp_cursor == ON)
      Disp_set_cur(OFF);
    cur_x = cx;
    cur_y = cy;
    if (temp_cursor == ON)
      Disp_set_cur(ON);
  }
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
 * Przesuwa okno w gore lub w dol.
 * 
 * parametry:
 *    dir - w gore (UP) lub w dol (DOWN)
 */ 
void Disp_scroll_window(uint8_t dir) {

  if (dir == UP)
    shift++;
  else
    shift--;
    
  for (uint8_t i=0; i<DISP_CHIP_NUM; i++)
    write_buffer(SET_PTR_OFFSET | (shift & 0x3F), COMMAND | i);
}

/*
 * Czysci zawartość wyswietlacza.
 */
void Disp_clr(void) {
  shift = 0;
  
  for (uint8_t i=0; i<DISP_CHIP_NUM; i++) {
    for (uint8_t y=0; y<8; y++) {
      write_buffer(SET_PTR_COL_MSB | ((DISP_COL_OFFSET & 0xF0) >> 4), COMMAND | i);
      write_buffer(SET_PTR_COL_LSB | (DISP_COL_OFFSET & 0x0F), COMMAND | i);
      write_buffer(SET_PTR_PAGE | y, COMMAND | i);
      for (uint8_t x=DISP_COL_OFFSET; x<132; x++)
        write_buffer(0, i);
    }

    write_buffer(SET_PTR_PAGE, COMMAND | i);
    write_buffer(SET_PTR_COL_MSB | ((DISP_COL_OFFSET & 0xF0) >> 4), COMMAND | i);
    write_buffer(SET_PTR_COL_LSB | (DISP_COL_OFFSET & 0x0F), COMMAND | i);
    write_buffer(SET_PTR_OFFSET, COMMAND | i);
    
    virtual_x[i] = DISP_COL_OFFSET;
    virtual_y[i] = 0x00;
  }
  
  entry_mode = INC;
  Disp_set_cxy(0, 0);
  Disp_set_pxy(0, 0);
}

/*
 * Przywraca kursor i okno do pozycji poczatkowej.
 */
void Disp_ret_cur(void) {
  shift = 0;
  
  for (uint8_t i=0; i<DISP_CHIP_NUM; i++)
    write_buffer(SET_PTR_OFFSET, COMMAND | i);
  
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
  uint8_t temp_px = pix_x;
  uint8_t temp_py = pix_y;
  
  uint8_t new_px = cur_x * 6;
  uint8_t new_py = cur_y * 8;
  
  Disp_set_pxy(new_px, new_py);

  for (uint8_t i=0; i<6; i++) {
    uint8_t vert_line = 0;
    for (uint8_t j=0; j<8; j++) {
      vert_line >>= 1; 
      vert_line |= (FONTS(8*character+j) << (i + 2)) & 0x80;
    }
    write_buffer(vert_line, active_module);
    update_virtual_x(active_module);
    Disp_set_pxy(pix_x+1, pix_y);
  }
  
  if (entry_mode == INC) {
    if (++cur_x == DISP_C_COLS_NUM) {
      cur_x = 0;
      if (++cur_y == DISP_C_ROWS_NUM)
        cur_y = 0;
    }
  } else {
    if (cur_x-- == 0) {
      cur_x = DISP_C_COLS_NUM - 1;
      if (cur_y-- == 0)
        cur_y = DISP_C_ROWS_NUM - 1;
    }
  }
  
  if (cursor == ON)
    Disp_set_cur(ON);
  
  pix_x = temp_px;
  pix_y = temp_py;
}

/*
 * Wyswietla lancuch znakow poczawszy od aktualnej pozycji kursora.
 * 
 * parametry:
 *    string - wskaznik poczatku lancucha znakow
 */
void Disp_puts(char* string) {
  while (*string)
    Disp_putc(*(string++));
}

/*
 * Wyswietla staly lancuch znakow poczawszy od aktualnej pozycji kursora.
 * 
 * parametry:
 *    string - wskaznik poczatku lancucha znakow
 */
void Disp_puts_const(const char* string) {
  #if DISP_PLATFORM == AVR
  
    while (get_const((const uint8_t*)(string)))
      Disp_putc((char)get_const((const uint8_t*)(string++)));
      
  #else
  
    Disp_puts((char*)(string));
    
  #endif
}

/*
 * Wybiera tablice szablonow znakowych.
 * 
 * parametry:
 *    templates - wskaznik poczatku tablicy szablonow
 */
void Disp_set_ctempl(uint8_t* templates) {
  fonts = templates;
}

/*
 * Wybiera stala tablice szablonow znakowych.
 * 
 * parametry:
 *    templates - wskaznik poczatku tablicy szablonow
 */
void Disp_set_ctempl_const(const uint8_t* templates) {
  Disp_set_ctempl((uint8_t*)templates);  
}

/*
 * Zwraca tablice szablonow znakowych.
 * 
 * zwraca:
 *    wskaznik poczatku tablicy szablonow
 */
uint8_t* Disp_get_ctempl(void) { 
  return fonts;
}

/*
 * Zmienia pozycje kursora punktowego.
 * 
 * parametry:
 *    px - numer kolumny pikseli
 *    py - numer linii pikseli
 */
void Disp_set_pxy(uint8_t px, uint8_t py) {
  if ((px >= DISP_P_COLS_NUM) || (py >= DISP_P_ROWS_NUM))
    return;
  
  pix_x = px;
  pix_y = py;

  uint8_t new_virt_x = DISP_COL_OFFSET;
  
  #if DISP_SCR_X_NUM == 1
  
    active_module = DISP_SCR_X_NUM * (py / 64);
    new_virt_x += px;
    
  #elif DISP_SCR_X_NUM == 2
    
    if (px < DISP_G1_SEG_NUM) {
      active_module = DISP_SCR_X_NUM * (py / 64);
      new_virt_x += px;
    } else {
      active_module = DISP_SCR_X_NUM * (py / 64) + 1;
      new_virt_x += px - (DISP_G1_SEG_NUM);
    }
    
  #endif
  
  uint8_t new_virt_y = (py / 8) % 8;
  
  if (virtual_x[active_module] != new_virt_x) {
    write_buffer(SET_PTR_COL_MSB | (new_virt_x >> 4), COMMAND | active_module);
    write_buffer(SET_PTR_COL_LSB | (new_virt_x & 0x0F), COMMAND | active_module);
    virtual_x[active_module] = new_virt_x;
  }
  if (virtual_y[active_module] != new_virt_y) {
    write_buffer(SET_PTR_PAGE | new_virt_y, COMMAND | active_module);
    virtual_y[active_module] = new_virt_y;
  }
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
  Disp_set_pxy(pix_x, pix_y);
    
  write_buffer(1<<(pix_y % 8), RDMOWR | ((pixel==ON)?LOG_OR:0) | active_module);
  update_virtual_x(active_module);
}

/*
 * Pobiera piksel z aktualnej pozycji kursora.
 * 
 * zwraca:
 *    ON  - piksel aktywny
 *    OFF - piksel nieaktywny
 */
uint8_t Disp_getp(void) {
  Disp_set_pxy(pix_x, pix_y);
  
  #if DISP_UPDATE_ON_READ == 1
    Disp_update();
  #endif
  
  read_data(active_module);
  uint8_t temp = read_data(active_module);
  update_actual_x(active_module);
  update_virtual_x(active_module);
  
  temp &= (1 << (pix_y % 8));
  
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
  
  if (px >= pix_x) {
    dx = px - pix_x;
  } else {
    dx = pix_x - px;
    pix_x = px;
  }
  
  dx++;
  
  while (dx != 0) {
    Disp_putp(color);
    pix_x++;
    dx--;
  }

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
  uint8_t dy;
  
  if (py >= pix_y) {
    dy = py - pix_y;
  } else {
    dy = pix_y - py;
    pix_y = py;
  }
  
  dy++;
  
  uint8_t byte_pix = pix_y % 8;
  if (byte_pix != 0) {
    Disp_set_pxy(pix_x, pix_y);
    
    uint8_t line = 0xFF;
    if (dy < (8 - byte_pix))
      line >>= (8 - dy);
    line <<= byte_pix;
    
    if (dy > (8-byte_pix))
      dy -= (8-byte_pix);
    else
      dy = 0;
      
    write_buffer(line, RDMOWR | ((color==ON)?LOG_OR:0) | active_module);
    update_virtual_x(active_module);
    
    pix_y += 8 - byte_pix;
  }
  
  while (dy != 0) {
    Disp_set_pxy(pix_x, pix_y);
    
    uint8_t line;
    if (dy >= 8) {
      dy -= 8;
      pix_y += 8;
      write_buffer(((color==ON)?0xFF:0x00), active_module);
    } else {
      line = 0xFF >> (8 - dy);
      dy = 0;
      write_buffer(line, RDMOWR | ((color==ON)?LOG_OR:0) | active_module);
    }
    update_virtual_x(active_module);
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
    uint8_t px0;
    uint8_t width;
    
    if (px1 <= px2) {
      width = px2 - px1;
      px0 = px1;
    } else {
      width = px1 - px2;
      px0 = px2;
    }
    
    for (uint8_t i = 0; i <= width; i++) {
      Disp_set_pxy(px0 + i, py1);
      Disp_vlineto(py2, color);
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
  uint8_t top_offset = temp_py % 8;
  uint8_t top_length = 8 - top_offset;
  uint8_t bottom_offset = (uint8_t)(temp_py + length - 1) % 8;
  uint8_t width_bytes = (uint8_t)(width + 7) / 8;
  uint8_t length_bytes = (uint8_t)(length + top_offset + 7) / 8;
  
  if (length < top_length)
    top_length = length;
  
  //clear picture window
  if ((top_offset != 0) || (top_length == length))
    Disp_box(pix_x, pix_y,
      pix_x + width - 1, pix_y + top_length - 1, NO, YES);
  if ((bottom_offset != 7) && (top_length != length))    
    Disp_box(pix_x, pix_y + length - 1 - bottom_offset,
      pix_x + width - 1, pix_y + length - 1, NO, YES);
  
  Disp_set_pxy(temp_px, temp_py);
  
  for (uint8_t y=0; y<length_bytes; y++) {
    for (uint8_t x=0; x<width_bytes; x++) {
      uint8_t temp[8];
      
      uint16_t index = 0;
      if (y != 0)
        index = (8*y - top_offset)*width_bytes;
      index += x;
      for (uint8_t k=0; k<8; k++) {
        if ((y == length_bytes - 1)
           && (((length_bytes != 1) && (k > bottom_offset))
            || ((length_bytes == 1) && (k >= length))))
          temp[k] = 0;
        else {
          temp[k] = *(picture+index);
          index += width_bytes;
        }
      }
      
      for (uint8_t i=0; i<8; i++) {
        if (x*8 + i >= width)
          break;
        else {
          uint8_t vert_line = 0;
          for (uint8_t k=7; k!=0xFF; k--) {
            vert_line <<= 1;
            vert_line |= temp[k] & 0x01;
            temp[k] >>= 1;
          }
          if (y == 0)
            vert_line <<= top_offset;
          if (((y == 0) && (top_offset != 0)) || \
              ((y == length_bytes - 1) && (bottom_offset != 7)))
            write_buffer(vert_line, RDMOWR | LOG_OR | active_module);
          else
            write_buffer(vert_line, active_module);
          update_virtual_x(active_module);
          Disp_set_pxy(pix_x+1, pix_y);
        }
      }
    }
    Disp_set_pxy(temp_px, pix_y+8);
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
  uint8_t top_offset = temp_py % 8;
  uint8_t top_length = 8 - top_offset;
  uint8_t bottom_offset = (uint8_t)(temp_py + length - 1) % 8;
  uint8_t width_bytes = (uint8_t)(width + 7) / 8;
  uint8_t length_bytes = (uint8_t)(length + top_offset + 7) / 8;
  
  if (length < top_length)
    top_length = length;
  
  //clear picture window
  if ((top_offset != 0) || (top_length == length))
    Disp_box(pix_x, pix_y,
      pix_x + width - 1, pix_y + top_length - 1, NO, YES);
  if ((bottom_offset != 7) && (top_length != length))    
    Disp_box(pix_x, pix_y + length - 1 - bottom_offset,
      pix_x + width - 1, pix_y + length - 1, NO, YES);
  
  Disp_set_pxy(temp_px, temp_py);
  
  for (uint8_t y=0; y<length_bytes; y++) {
    for (uint8_t x=0; x<width_bytes; x++) {
      uint8_t temp[8];
      
      uint16_t index = 0;
      if (y != 0)
        index = (8*y - top_offset)*width_bytes;
      index += x;
      for (uint8_t k=0; k<8; k++) {
        if ((y == length_bytes - 1)
           && (((length_bytes != 1) && (k > bottom_offset))
            || ((length_bytes == 1) && (k >= length))))
          temp[k] = 0;
        else {
          temp[k] = get_const(picture+index);
          index += width_bytes;
        }
      }
      
      for (uint8_t i=0; i<8; i++) {
        if (x*8 + i >= width)
          break;
        else {
          uint8_t vert_line = 0;
          for (uint8_t k=7; k!=0xFF; k--) {
            vert_line <<= 1;
            vert_line |= temp[k] & 0x01;
            temp[k] >>= 1;
          }
          if (y == 0)
            vert_line <<= top_offset;
          if (((y == 0) && (top_offset != 0)) || \
              ((y == length_bytes - 1) && (bottom_offset != 7)))
            write_buffer(vert_line, RDMOWR | LOG_OR | active_module);
          else
            write_buffer(vert_line, active_module);
          update_virtual_x(active_module);
          Disp_set_pxy(pix_x+1, pix_y);
        }
      }
    }
    Disp_set_pxy(temp_px, pix_y+8);
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
  uint8_t top_offset = temp_py % 8;
  uint8_t width_bytes = (uint8_t)(width + 7) / 8;
  uint8_t length_bytes = (uint8_t)(length + top_offset + 7) / 8;
  
  Disp_set_pxy(temp_px, temp_py);
  Disp_update();
  
  for (uint16_t i=0; i<width_bytes*length; i++)
    *(picture+i) = 0;
  
  for (uint8_t y=0; y<length_bytes; y++) {
    read_data(active_module); // dummy read needed!
    for (uint8_t x=0; x<width; x++) {
      uint8_t vert_line = read_data(active_module);
      update_actual_x(active_module);
      update_virtual_x(active_module);
      
      uint16_t index = 0;
      uint8_t y_cnt = 0;
      if (y != 0) {
        y_cnt = 8*y - top_offset;
        index = y_cnt*width_bytes;
      }
      
      index += (x / 8);
      uint8_t or_bit = 0x01 << (x % 8);
      for (uint8_t k=(0x01 << (y?0:top_offset)); k!=0x00; k<<=1) {
        if (vert_line & k)
          *(picture+index) |= or_bit;
        index += width_bytes;
        if (++y_cnt == length)
          break;
      }
      
      uint8_t prev_act_mod = active_module;
      Disp_set_pxy(pix_x+1, pix_y);
      Disp_update();
      if (active_module != prev_act_mod)
        read_data(active_module); // dummy read needed!
    }
    Disp_set_pxy(temp_px, pix_y+8);
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
 * Uaktualnia wirtualna zmienna adresowania
 * aktywnego chipu w osi X.
 */
static void update_virtual_x(uint8_t no) {
  if (virtual_x[no] != 131)
    virtual_x[no]++;
}

/*
 * Uaktualnia rzeczywista zmienna adresowania
 * aktywnego chipu w osi X.
 */
static void update_actual_x(uint8_t no) {
  if (actual_x[no] != 131)
    actual_x[no]++;
}

/*
 * Ustawia badz zeruje odpowiednia linie CS. (8080)
 */
#if DISP_INTERF_TYPE == 0
static void toggle_cs(uint8_t chip_no, uint8_t on) {
  if (on == 0) {
    #if DISP_CHIP_NUM != 1
      if (chip_no == 0)
    #endif
        rst_cs1();
    #if DISP_CHIP_NUM >= 2
      else if (chip_no == 1)
        rst_cs2();
    #endif
    #if DISP_CHIP_NUM >= 3
      else if (chip_no == 2)
        rst_cs3();
    #endif
    #if DISP_CHIP_NUM >= 4
      else if (chip_no == 3)
        rst_cs4();
    #endif
  } else {
    #if DISP_CHIP_NUM != 1
      if (chip_no == 0)
    #endif
        set_cs1();
    #if DISP_CHIP_NUM >= 2
      else if (chip_no == 1)
        set_en2();
    #endif
    #if DISP_CHIP_NUM >= 3
      else if (chip_no == 2)
        set_cs3();
    #endif
    #if DISP_CHIP_NUM >= 4
      else if (chip_no == 3)
        set_cs4();
    #endif
  }
}
#endif

/*
 * Ustawia badz zeruje odpowiednia linie EN. (6800)
 */
#if DISP_INTERF_TYPE == 1
static void toggle_en(uint8_t chip_no, uint8_t on) {
  if (on == 0) {
    #if DISP_CHIP_NUM != 1
      if (chip_no == 0)
    #endif
        rst_en1();
    #if DISP_CHIP_NUM >= 2
      else if (chip_no == 1)
        rst_en2();
    #endif
    #if DISP_CHIP_NUM >= 3
      else if (chip_no == 2)
        rst_en3();
    #endif
    #if DISP_CHIP_NUM >= 4
      else if (chip_no == 3)
        rst_en4();
    #endif
  } else {
    #if DISP_CHIP_NUM != 1
      if (chip_no == 0)
    #endif
        set_en1();
    #if DISP_CHIP_NUM >= 2
      else if (chip_no == 1)
        set_en2();
    #endif
    #if DISP_CHIP_NUM >= 3
      else if (chip_no == 2)
        set_en3();
    #endif
    #if DISP_CHIP_NUM >= 4
      else if (chip_no == 3)
        set_en4();
    #endif
  }
}
#endif

/*
 * Przesyla 1 bajt danych do wyswietlacza. (8080)
 */
#if DISP_INTERF_TYPE == 0
static void write_data(uint8_t data, uint8_t chip_no) {
  
  toggle_cs(chip_no, 0);
  write_db(data);
  rst_wr();
  nop();
  set_wr();
  nop();
  nop();
  nop();
  toggle_cs(chip_no, 1);
}
#endif
 
/*
 * Przesyla 1 bajt danych do wyswietlacza. (6800)
 */
#if DISP_INTERF_TYPE == 1
static void write_data(uint8_t data, uint8_t chip_no) {
  
  write_db(data);
  toggle_en(chip_no, 1);
  nop();
  toggle_en(chip_no, 0);
  nop();
  nop();
  nop();
}
#endif

/*
 * Przesyla komende.
 */
__inline__ static void send_cmd(uint8_t cmd, uint8_t chip_no) {
  rst_rs();
  write_data(cmd, chip_no);
  set_rs();
}

/*
 * Odczytuje 1 bajt danych z wyswietlacza. (8080)
 */
#if DISP_INTERF_TYPE == 0
static uint8_t read_data(uint8_t chip_no) {
  uint8_t result;
  
  toggle_cs(chip_no, 0);
  
  chg_dir_db(IN);
  
  rst_rd();
  nop();
  nop();
  result = read_db();
  set_rd();
  
  nop();
  nop();
  
  chg_dir_db(OUT);
  
  toggle_cs(chip_no, 1);
  
  return result;
}
#endif

/*
 * Odczytuje 1 bajt danych z wyswietlacza. (6800)
 */
#if DISP_INTERF_TYPE == 1
static uint8_t read_data(uint8_t chip_no) {
  uint8_t result;
  
  chg_dir_db(IN);  
  set_rw();
  nop();
  
  toggle_en(chip_no, 1);
  nop();
  nop();
  result = read_db();
  toggle_en(chip_no, 0);
  
  rst_rw();
  
  nop();
  
  chg_dir_db(OUT);
  
  return result;
}
#endif

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
