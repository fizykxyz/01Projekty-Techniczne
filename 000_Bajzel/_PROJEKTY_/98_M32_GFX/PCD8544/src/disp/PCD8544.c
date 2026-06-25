/****************************************************************************
* Biblioteka obslugi wyswietlacza graficznego, opartego na sterowniku
* PCD8544 lub odpowiedniku.
*
* Nazwa pliku: PCD8544.c
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

#include <inttypes.h>
#include <PCD8544.h>
#include <disp_conf.h>
#include <platf_conf.h>

#define EMPTY   0x80
#define COMMAND 0x40
#define RDMOWR  0x20
#define LOG_OR  0x10
#define DATA    0x00

/*
 * Definicje etykiet dla komend
 */
#define SET_FUNCTION    0x20
#define F_DISP_PD       0x04
#define F_BIT_V         0x02
#define F_BIT_H         0x01

/*
 * Definicje etykiet dla komend ze zbioru standardowego (H=0)
 */
#define SET_DISP_OFF    0x08
#define SET_DISP_ALL_ON 0x09
#define SET_DISP_POS    0x0C
#define SET_DISP_NEG    0x0D

#define SET_PTR_PAGE    0x40
#define SET_PTR_COL     0x80

/*
 * Definicje etykiet dla komend ze zbioru rozszerzonego (H=1)
 */
#define SET_T_COEF      0x04
#define SET_BIASES      0x10
#define SET_CONTRAST    0x80

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

static uint8_t fifo[DISP_BUFFER_LEN * 2];
static volatile uint8_t* fifo_rd_ptr;
static volatile uint8_t* fifo_wr_ptr;

static volatile uint8_t virtual_x;
static volatile uint8_t virtual_y;

static uint8_t* fonts;

#if DISP_MIRROR_MEM == 1
  static volatile uint8_t mem_mirror[DISP_P_COLS_NUM][DISP_P_ROWS_NUM/8];
#endif

/*
 * Definicje funkcji prywatnych
 */
static void write_data(uint8_t data);
__inline__ static void send_cmd(uint8_t cmd);

static uint8_t read_data(void);

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

  set_cd();
  
  delay_dus(100);
  set_res();
  
  error = 0;
  overflow = 0;

  cursor = OFF;
  entry_mode = INC;
  
  Disp_flush_buffer();
  
  write_buffer(SET_FUNCTION | F_BIT_H, COMMAND);
  write_buffer(SET_CONTRAST | (DISP_CONTRAST & 0x7F), COMMAND);
  write_buffer(SET_BIASES | (DISP_BIASES & 0x07), COMMAND);
  write_buffer(SET_T_COEF | (DISP_T_COEF & 0x03), COMMAND);
  write_buffer(SET_FUNCTION, COMMAND);
  
  Disp_clr();
  
  write_buffer(DISP_NEGATIVE? SET_DISP_NEG : SET_DISP_POS, COMMAND);
  
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
 *    on       - wlaczony (ON) badz wylaczony (OFF)
 *    negative - tryb normalny (OFF) lub negatywowy (ON)
 */
void Disp_set(uint8_t on, uint8_t negative) {
  
  uint8_t settings = SET_DISP_OFF;
  
  if (on == ON) {
    if (negative == OFF)
      settings = SET_DISP_POS;
    else
      settings = SET_DISP_NEG;
  }
  
  write_buffer(settings, COMMAND);
}

/*
 * Steruje zasilaniem wyswietlacza.
 * 
 * parametry:
 *    on - wlaczony (ON) badz wylaczony (OFF)
 */
void Disp_set_power(uint8_t on) {
    
  if (on == OFF)
    write_buffer(SET_FUNCTION | F_DISP_PD, COMMAND);
  else
    write_buffer(SET_FUNCTION, COMMAND);
}

/*
 * Okresla kontrast wyswietlacza.
 * 
 * parametry:
 *    contrast - kontrast (0...127)
 */
void Disp_set_contrast(uint8_t contrast) {
   write_buffer(SET_FUNCTION | F_BIT_H, COMMAND);
   write_buffer(SET_CONTRAST | (contrast & 0x7F), COMMAND);
   write_buffer(SET_FUNCTION, COMMAND);
}

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
    
    if (!(condition & COMMAND))
      write_data(data);
    else
      send_cmd(data);
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
 * Czysci zawartość wyswietlacza.
 */
void Disp_clr(void) {
  
  for (uint8_t i=0; i<DISP_P_ROWS_NUM/8; i++) {
    write_buffer(SET_PTR_PAGE | i, COMMAND);
    write_buffer(SET_PTR_COL, COMMAND);
    
    pix_y = i*8;
    
    for (uint8_t j=0; j<DISP_P_COLS_NUM; j++) {
      pix_x = j;
      write_buffer(0x00, DATA);
    }
  }
  
  entry_mode = INC;
  
  cur_x = 0;
  cur_y = 0;
  pix_x = 0;
  pix_y = 0;
  virtual_x = 0;
  virtual_y = 0;
  
  write_buffer(SET_PTR_PAGE, COMMAND);
  write_buffer(SET_PTR_COL, COMMAND);
  
  if (cursor == ON)
    Disp_set_cur(ON);
}

/*
 * Przywraca kursor i okno do pozycji poczatkowej.
 */
void Disp_ret_cur(void) {
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
    write_buffer(vert_line, DATA);
    virtual_x++;
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
  
  if (virtual_x != px) {
    write_buffer(SET_PTR_COL | px, COMMAND);
    virtual_x = px;
  }
  
  uint8_t new_virt_y = py / 8;
  
  if (virtual_y != new_virt_y) {
    write_buffer(SET_PTR_PAGE | new_virt_y, COMMAND);
    virtual_y = new_virt_y;
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
    
  write_buffer(1<<(pix_y % 8), RDMOWR | ((pixel==ON)?LOG_OR:0));
  virtual_x++;
}

/*
 * Pobiera piksel z aktualnej pozycji kursora.
 * 
 * zwraca:
 *    ON  - piksel aktywny
 *    OFF - piksel nieaktywny
 */
#if DISP_MIRROR_MEM == 1
uint8_t Disp_getp(void) {
  Disp_set_pxy(pix_x, pix_y);
  
  #if DISP_UPDATE_ON_READ == 1
    Disp_update();
  #endif
  
  return (read_data() & (1 << (pix_y % 8)))? ON : OFF;
}
#endif

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
      
    write_buffer(line, RDMOWR | ((color==ON)?LOG_OR:0));
    virtual_x++;
    
    pix_y += 8 - byte_pix;
  }
  
  while (dy != 0) {
    Disp_set_pxy(pix_x, pix_y);
    
    uint8_t line;
    if (dy >= 8) {
      dy -= 8;
      write_buffer(((color==ON)?0xFF:0x00), DATA);
      pix_y += 8;
    } else {
      line = 0xFF >> (8 - dy);
      dy = 0;
      write_buffer(line, RDMOWR | ((color==ON)?LOG_OR:0));
    }
    
    virtual_x++;
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
            write_buffer(vert_line, RDMOWR | LOG_OR);
          else
            write_buffer(vert_line, DATA);
            
          virtual_x++;
            
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
            write_buffer(vert_line, RDMOWR | LOG_OR);
          else
            write_buffer(vert_line, DATA);
            
          virtual_x++;
            
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
#if (DISP_NOC_GET_PIC == 0) && (DISP_MIRROR_MEM == 1)
uint16_t Disp_get_picture(uint8_t* picture, \
                      uint8_t width, uint8_t length) {
  
  uint8_t temp_px = pix_x;
  uint8_t temp_py = pix_y;
  uint8_t top_offset = temp_py % 8;
  uint8_t width_bytes = (uint8_t)(width + 7) / 8;
  uint8_t length_bytes = (uint8_t)(length + top_offset + 7) / 8;
    
  #if DISP_UPDATE_ON_READ == 1
    Disp_update();
  #endif
  
  for (uint16_t i=0; i<width_bytes*length; i++)
    *(picture+i) = 0;
  
  for (uint8_t y=0; y<length_bytes; y++) {
    for (uint8_t x=0; x<width; x++) {
      uint8_t vert_line = read_data();
      
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
      
      pix_x++;
    }
    pix_x = temp_px;
    pix_y += 8;
  }
  
  pix_x = temp_px;
  pix_y = temp_py;
  
  return width_bytes*length;
}
#endif

/****************************************************************************
 * Deklaracje funkcji prywatnych
 ***************************************************************************/

/*
 * Przesyla 1 bajt danych do wyswietlacza.
 */
static void write_data(uint8_t data) {
  
  rst_ce();
  
  for (uint8_t i=0x80; i!=0; i>>=1) {
    rst_clk();
    if (data & i)
      set_mosi();
    else
      rst_mosi();
    nop();
    nop();
    set_clk();
    nop();
  }
  
  set_ce();
}

/*
 * Przesyla komende.
 */
__inline__ static void send_cmd(uint8_t cmd) {
  rst_cd();
  write_data(cmd);
  set_cd();
}

/*
 * Odczytuje 1 bajt danych z wyswietlacza.
 */
uint8_t read_data(void) {
 
  #if DISP_MIRROR_MEM == 1
    return mem_mirror[pix_x][pix_y/8];
  #else
    return 0;
  #endif
}

/*
 * Zapisuje wartosc do bufora FIFO.
 */
static void write_buffer(uint8_t data, uint8_t condition) {
  
  #if DISP_MIRROR_MEM == 1
    if (!(condition & COMMAND)) {
      
      if (condition & RDMOWR) {

        if (condition & LOG_OR)
          data |= read_data();
        else
          data = read_data() & (~data);
      }

      mem_mirror[pix_x][pix_y/8] = data;
    }
  #endif
  
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
