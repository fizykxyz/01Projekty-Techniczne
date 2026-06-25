/****************************************************************************
* Biblioteka obslugi wyswietlacza alfanumerycznego, opartego na sterowniku
* M66004 (VFD) lub odpowiedniku.
*
* Nazwa pliku: M66004.c
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

#include <inttypes.h>
#include <M66004.h>
#include <disp_conf.h>
#include <platf_conf.h>

#define EMPTY      0xFF

#define CURSOR_ON  0x01
#define ENTRY_DEC  0x02

/*
 * Definicje etykiet dla komend
 */
#define SET_LENGTH    0x00

#define SET_DIMMER    0x08

#define SET_SLOW_SCAN 0xF7
#define SET_FAST_SCAN 0xF6

#define SET_POINTER   0xE0
#define SET_AUTO_INCR 0xF5
#define SET_NO_INCR   0xF4

#define SET_CUR_ON    0x10
#define SET_CUR_OFF   0x80

#define DISP_OFF      0xF0
#define DISP_ON       0xF1

#define SEND_CGRAM    0xFC


/*
 * Definicje zmiennych prywatnych
 */
static volatile uint8_t error = 1;
static volatile uint8_t overflow = 0;
static uint8_t update = DEFERRED;
static uint8_t cur_x;

static uint8_t settings;

static uint8_t fifo[DISP_BUFFER_LEN];
static volatile uint8_t* fifo_rd_ptr;
static volatile uint8_t* fifo_wr_ptr;


/*
 * Definicje funkcji prywatnych
 */
static void write_data(uint8_t data);

static uint8_t read_buffer(uint8_t* data);
static void write_buffer(uint8_t data);

/*
 * Inicjalizuje wyswietlacz.
 * 
 * zwraca:
 *    0 - jesli inicjalizacja przebiegla poprawnie
 *    1 - jesli wystapil blad
 */
uint8_t Disp_init(void) {
  init_ports();
  
  set_cs1();
  set_clk();
  
  error = 0;
  settings = 0;
  Disp_flush_buffer();
  
  delay_ms(50);
  
  write_buffer(SET_LENGTH | ((DISP_C_COLS_NUM - 9) & 0x07));
  write_buffer(SET_DIMMER | (DISP_DEF_CONTRAST & 0x07));
  write_buffer(DISP_SCAN_TIME? SET_FAST_SCAN : SET_SLOW_SCAN);
  write_buffer(SET_AUTO_INCR);
  #if DISP_CUR_PRESENT == 1
    for (uint8_t i=0; i<DISP_C_COLS_NUM; i++)
      write_buffer(SET_CUR_OFF | i);
  #endif      
  write_buffer(DISP_ON);
  
  Disp_clr_txt();
  
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
    write_buffer(DISP_ON);
  else
    write_buffer(DISP_OFF);
}

/*
 * Okresla kontrast wyswietlacza.
 * 
 * parametry:
 *    contrast - wartosc z przedzialu 0 do 7
 */
void Disp_set_contrast(uint8_t contrast) {
  write_buffer(SET_DIMMER | (contrast & 0x07));
}

/*
 * Okresla atrybuty kursora znakowego.
 * 
 * parametry:
 *    on    - wlaczony (ON) badz wylaczony (OFF)
 */
#if DISP_CUR_PRESENT == 1
void Disp_set_cur(uint8_t on) {
  if ((on == ON) && !(settings & CURSOR_ON)) {
    settings |= CURSOR_ON;
    write_buffer(SET_CUR_ON | cur_x);
  } else if ((on == OFF) && (settings & CURSOR_ON)) {
    settings &= ~CURSOR_ON;
    write_buffer(SET_CUR_OFF | cur_x);
  }
}
#endif

/*
 * Okresla tryb wprowadzania.
 * 
 * parametry:
 *    input_mode - autoinkrementacja adresu przy zapisie i odczycie (INC)
 *                 badz autodekrementacja (DEC)
 */
void Disp_set_input_mode(uint8_t input_mode) {
  if (input_mode == INC)
    settings &= ~ENTRY_DEC;
  else
    settings |= ENTRY_DEC;
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
  uint8_t result;
  uint8_t data = 0;
  
  result = read_buffer(&data);
  
  while (result != EMPTY) {
    write_data(data);
    result = read_buffer(&data);
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
  #if DISP_CUR_PRESENT == 1
    if (settings & CURSOR_ON) {
      write_buffer(SET_CUR_OFF | cur_x);
      write_buffer(SET_CUR_ON | (cx & 0x0F));
    }
  #endif
  cur_x = cx & 0x0F;
  write_buffer(SET_POINTER | cur_x);
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
  *cy = 0;
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
  return 0;
}

/*
 * Przesuwa kursor znakowy o jedna pozycje.
 * 
 * parametry:
 *    dir - w prawo (RIGHT) lub w lewo (LEFT)
 */
void Disp_move_txt_cur(uint8_t dir) {
  if (dir == RIGHT)
    Disp_set_cxy((cur_x + 1) & 0x0F, 0);
  else
    Disp_set_cxy((cur_x - 1) & 0x0F, 0);
}

/*
 * Czysci pole tekstowe wyswietlacza.
 */
void Disp_clr_txt(void) {
  for (uint8_t i=0; i<DISP_C_COLS_NUM; i++)
    write_buffer(' ');
  
  Disp_set_cxy(0, 0);
  settings &= ~(ENTRY_DEC);
}

/*
 * Czysci zawartość wyswietlacza.
 */
__inline__ void Disp_clr(void) {
  Disp_clr_txt();
}

/*
 * Przywraca kursor i okno do pozycji poczatkowej.
 */
void Disp_ret_cur(void) {
  Disp_set_cxy(0, 0);
  settings &= ~(ENTRY_DEC);
}

/*
 * Wyswietla znak na aktualnej pozycji kursora.
 * 
 * parametry:
 *    character - kod znaku do wyswietlenia
 */
void Disp_putc(char character) {
  
  write_buffer((uint8_t)character);
  
  if (settings & CURSOR_ON)
      write_buffer(SET_CUR_OFF | cur_x);

  if (settings & ENTRY_DEC) {
    
    if (cur_x == 0)
      cur_x = DISP_C_COLS_NUM - 1;
    else
      cur_x--;
      
    write_buffer(SET_POINTER | cur_x);
    
    #if DISP_CUR_PRESENT == 1
      if (settings & CURSOR_ON)
        write_buffer(SET_CUR_ON | cur_x);
    #endif        
      
  } else {
    
    if (cur_x == DISP_C_COLS_NUM - 1)
      cur_x = 0;
    else
      cur_x++;
    
    #if DISP_CUR_PRESENT == 1
      if (settings & CURSOR_ON)
        write_buffer(SET_CUR_ON | cur_x);
    #endif
  }
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
 *    count     - dlugosc tablicy szablonow w bajtach
 *    addr      - numer pierwszego szablonu do zapisania
 */
void Disp_set_ctempl(uint8_t* templates, uint8_t count, uint8_t addr) {
  while (count >= 5) {
    write_buffer(SEND_CGRAM);
    write_buffer(addr++);
    for (uint8_t i=0; i<5; i++)
      write_buffer(*(templates++));
    count -= 5;
  }
}

/*
 * Wybiera stala tablice szablonow znakowych.
 * 
 * parametry:
 *    templates - wskaznik poczatku tablicy szablonow
 *    count     - dlugosc tablicy szablonow w bajtach
 *    addr      - numer pierwszego szablonu do zapisania
 */
void Disp_set_ctempl_const(const uint8_t* templates, uint8_t count, uint8_t addr) {
  #if DISP_PLATFORM == AVR
  
    while (count >= 5) {
      write_buffer(SEND_CGRAM);
      write_buffer(addr++);
      for (uint8_t i=0; i<5; i++)
        write_buffer(get_const(templates++));
      count -= 5;
    }
      
  #else
  
    Disp_set_ctempl((uint8_t*)templates, count, addr);
    
  #endif
}


/****************************************************************************
 * Deklaracje funkcji prywatnych
 ***************************************************************************/
 
/*
 * Przesyla 1 bajt danych do wyswietlacza.
 */
static void write_data(uint8_t data) {
  
  rst_cs1();
  
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
    nop();
    nop();
  }
  
  set_cs1();
    
  delay_us((uint8_t)(6 * 1000.0 / DISP_OSC));
}

/*
 * Zapisuje wartosc do bufora FIFO.
 */
static void write_buffer(uint8_t data) {
  if ((fifo_wr_ptr == fifo_rd_ptr - 1)  // jesli wystapilo przepelnienie:
     || ((fifo_wr_ptr == fifo + (DISP_BUFFER_LEN-1))
         && (fifo_rd_ptr == fifo))) {
    
    #if DISP_ON_BUFFER_FULL == 1        //   przeslij zawartosc bufora
    
      Disp_update();
    
    #elif DISP_ON_BUFFER_FULL == 2      //   oczekuj na zwolnienie bufora
    
      while ((fifo_wr_ptr == fifo_rd_ptr - 1)
            || ((fifo_wr_ptr == fifo + (DISP_BUFFER_LEN-1))
                && (fifo_rd_ptr == fifo)));
    
    #else                               //   pomin zapisywane wartosci
      
      overflow = 1;
      return;
  
    #endif
  }
  
  disable_interrupts();
  
  *(fifo_wr_ptr++) = data;
  
  if (fifo_wr_ptr == fifo + DISP_BUFFER_LEN)
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
    disable_interrupts();
    
    *data = *(fifo_rd_ptr++);
    
    if (fifo_rd_ptr == fifo + DISP_BUFFER_LEN)
      fifo_rd_ptr = fifo;
    
    revert_interrupts();
    
    return 0;
  }
}
