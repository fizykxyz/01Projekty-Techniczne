/****************************************************************************
* Biblioteka obslugi wyswietlacza alfanumerycznego, opartego na sterowniku
* HD44780 lub odpowiedniku.
*
* Nazwa pliku: HD44780.c
* Data: 11.07.2007
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
#include <HD44780.h>
#include <disp_conf.h>
#include <platf_conf.h>

#define EMPTY   0
#define DATA    1
#define COMMAND 2
#define EN_2    4

/*
 * Definicje etykiet dla komend
 */
#define CLEAR   0  // czyszczenie wyswietlacza i powrot kursora
#define RETURN  1  // powrot kursora do pozycji poczatkowej

#define MODE    2  // zmiana trybu zapisu pamieci
#define M_INC   1  //   adres inkrementowany przy kazdym zapisie
#define M_MOVE  0  //   okno przesuwane przy kazdym zapisie

#define CTRL    3  // sterowanie wyswietlaczem
#define C_DI_ON 2  //   wyswietlanie wlaczone
#define C_CU_ON 1  //   kursor wlaczony
#define C_BL_ON 0  //   miganie kursora wlaczone

#define MOVE    4  // przesuwanie
#define M_WIN   3  //   przesuwanie okna
#define M_RIGHT 2  //   przesuwanie w prawo

#define SET     5  // ustawienia interfejsu i wyswietlacza
#define S_8BIT  4  //   interfejs 8-bitowy
#define S_2LIN  3  //   wyswietlacz 2-liniowy
#define S_5x10  2  //   znaki o rozmiarach 5x10 punktów

#define CGADR   6  // zmiana adresu CGRAM
#define DDADR   7  // zmiana adresu DDRAM

/*
 * Definicje makr.
 */
#if DISP_DOUBLE_SPEED == 0
#  define _nop() { nop(); nop(); }
#elif DISP_DOUBLE_SPEED == 1
#  define _nop() { nop(); }
#else
#  error "Nieznana predkosc pracy magistrali wyswietlacza (DISP_DOUBLE_SPEED)"
#endif

/*
 * Definicje zmiennych prywatnych
 */
static volatile uint8_t error = 1;
static volatile uint8_t overflow = 0;
static uint8_t update = DEFERRED;
static uint8_t cur_x;
static uint8_t cur_y;

static uint8_t active_module = 1;

static uint8_t settings_mode = (1<<MODE)|(1<<M_INC);
static uint8_t settings_ctrl = (1<<CTRL);

static uint8_t first_time = 1;

static uint8_t fifo[DISP_BUFFER_LEN * 2];
static volatile uint8_t* fifo_rd_ptr;
static volatile uint8_t* fifo_wr_ptr;


/*
 * Definicje funkcji prywatnych
 */
static void write_data(uint8_t data, uint8_t no);
__inline__ static void send_cmd(uint8_t cmd, uint8_t no);

#if DISP_USE_RW_LINE == 1
  static uint8_t wait_if_busy(uint8_t no);
  static uint8_t read_data(uint8_t no);
#endif

static uint8_t read_buffer(uint8_t* data);
static void write_buffer(uint8_t data, uint8_t type);

/*
 * Inicjalizuje wyswietlacz.
 * 
 * zwraca:
 *    0 - jesli inicjalizacja przebiegla poprawnie
 *    1 - jesli wystapil blad
 */
uint8_t Disp_init(void) {
  if (first_time) {
    delay_ms(40);
    first_time = 0;
  }
  
  init_ports();
  
  write_db((1<<SET)|(1<<S_8BIT));
  
  for (uint8_t i = 0; i<3; i++) {
    set_en1();
    #if DISP_EN_NUM == 2
      set_en2();
    #endif
    
    _nop();
    _nop();
    _nop();
    
    rst_en1();
    #if DISP_EN_NUM == 2
      rst_en2();
    #endif
    
    delay_ms(5);  
  }
  
  #if DISP_DB_WIDTH == 4
  
    write_db((1<<SET));
    
    set_en1();
    #if DISP_EN_NUM == 2
      set_en2();
    #endif
    
    _nop();
    _nop();
    _nop();
    
    rst_en1();
    #if DISP_EN_NUM == 2
      rst_en2();
    #endif
    
    delay_dus((uint8_t)(1 + 270 * 4.3 / DISP_OSC));
  #endif
  
  error = 0;
  Disp_flush_buffer();
  
  uint8_t params = (1<<SET);
  #if DISP_DB_WIDTH == 8
    params |= 1<<S_8BIT;
  #endif
  #if (DISP_C_ROWS_NUM > 1) || (DISP_SPECIAL != 0)
    params |= 1<<S_2LIN;
  #endif
  #if ISP_CHAR_LENGTH == 10
    params |= 1<<S_5x10;
  #endif
  
  settings_mode = (1<<MODE);
  settings_ctrl = (1<<CTRL);
  
  Disp_set_input_mode(INC);
  
  #if DISP_EN_NUM == 2
    write_buffer(params, COMMAND | EN_2);
  #endif
  
  active_module = 1;
  write_buffer(params, COMMAND);
  Disp_set_cur(OFF, NO_BLINK);
  Disp_clr_txt();
  Disp_set(ON);
  
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
    settings_ctrl |= (1<<C_DI_ON);
  else
    settings_ctrl &= ~(1<<C_DI_ON);
    
  #if DISP_EN_NUM == 2
    if (active_module == 1) {
      write_buffer(settings_ctrl, COMMAND);
      write_buffer(settings_ctrl & (~((1<<C_CU_ON) | (1<<C_BL_ON))), COMMAND | EN_2);
    } else {
      write_buffer(settings_ctrl & (~((1<<C_CU_ON) | (1<<C_BL_ON))), COMMAND);
      write_buffer(settings_ctrl, COMMAND | EN_2);
    }
  #else
    write_buffer(settings_ctrl, COMMAND);
  #endif
}

/*
 * Okresla atrybuty kursora znakowego.
 * 
 * parametry:
 *    on    - wlaczony (ON) badz wylaczony (OFF)
 *    blink - migajacy (BLINK) badz niemigajacy (NO_BLINK)
 */
void Disp_set_cur(uint8_t on, uint8_t blink) {
  if (on == ON)
    settings_ctrl |= (1<<C_CU_ON);
  else
    settings_ctrl &= ~(1<<C_CU_ON);
    
  if (blink == BLINK)
    settings_ctrl |= (1<<C_BL_ON);
  else
    settings_ctrl &= ~(1<<C_BL_ON);
  
  uint8_t temp = 0;
  #if DISP_EN_NUM == 2
    if (active_module == 2)
      temp = EN_2;
  #endif
  
  write_buffer(settings_ctrl, COMMAND | temp);
}

/*
 * Okresla tryb wprowadzania.
 * 
 * parametry:
 *    input_mode - autoinkrementacja adresu przy zapisie i odczycie (INC)
 *                 badz autodekrementacja (DEC)
 */
void Disp_set_input_mode(uint8_t input_mode) {
  if (input_mode == INC)
    settings_mode |= (1<<M_INC);
  else
    settings_mode &= ~(1<<M_INC);
    
  write_buffer(settings_mode, COMMAND);
  #if DISP_EN_NUM == 2
    write_buffer(settings_mode, COMMAND | EN_2);
  #endif
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
  uint8_t dtype;
  uint8_t data = 0;
  uint8_t no;
  
  do {
    dtype = read_buffer(&data);
  
    no = 1;
    #if DISP_EN_NUM == 2
      if (dtype & EN_2)
        no = 2;
    #endif
    
    if (dtype & DATA)
      write_data(data, no);
    else if (dtype & COMMAND)
      send_cmd(data, no);
      
  } while (dtype != EMPTY);
  
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
  cur_x = cx;
  cur_y = cy;
  
  uint8_t addr = DISP_C_ROW0_ADDR;
  
  #if DISP_C_ROWS_NUM > 1
    if (cy == 1)
      addr = DISP_C_ROW1_ADDR;
  #endif
  #if DISP_C_ROWS_NUM > 2
    else if (cy == 2)
      addr = DISP_C_ROW2_ADDR;
  #endif
  #if DISP_C_ROWS_NUM > 3
    else if (cy == 3)
      addr = DISP_C_ROW3_ADDR;
  #endif
  
  addr += cx;
  
  #if DISP_SPECIAL == 16
  
    if (cur_x >= 8)
      addr += 0x40;
      
  #elif DISP_SPECIAL == 20
  
    if (cur_x >= 10)
      addr += 0x40;
  
  #endif
  
  #if DISP_EN_NUM == 2
    if ((cy > 1) && (active_module == 1)) {
      active_module = 2;
      write_buffer(settings_ctrl & (~((1<<C_CU_ON) | (1<<C_BL_ON))), COMMAND);
      write_buffer(settings_ctrl, COMMAND | EN_2);
      write_buffer(addr | (1<<DDADR), COMMAND | EN_2);
    } else if ((cy < 2) && (active_module == 2)) {
      active_module = 1;
      write_buffer(settings_ctrl & (~((1<<C_CU_ON) | (1<<C_BL_ON))), COMMAND | EN_2);
      write_buffer(settings_ctrl, COMMAND);
      write_buffer(addr | (1<<DDADR), COMMAND);
    } else {
      uint8_t temp = 0;
      if (active_module == 2)
        temp = EN_2;
      write_buffer(addr | (1<<DDADR), COMMAND | temp);
    }
  #else
    write_buffer(addr | (1<<DDADR), COMMAND);
  #endif
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
  uint8_t temp = 0;
  #if DISP_EN_NUM == 2
    if (active_module == 2)
      temp = EN_2;
  #endif
  
  uint8_t command = (1<<MOVE);
  
  if (dir == RIGHT)
    command |= (1<<M_RIGHT);
  
  write_buffer(command, COMMAND | temp);
}

/*
 * Przesuwa okno o jedna pozycje.
 * 
 * parametry:
 *    dir - w prawo (RIGHT) lub w lewo (LEFT)
 */ 
void Disp_move_txt_window(uint8_t dir) {
  uint8_t temp = 0;
  #if DISP_EN_NUM == 2
    if (active_module == 2)
      temp = EN_2;
  #endif
  
  uint8_t command = (1<<MOVE)|(1<<M_WIN);
  
  if (dir == RIGHT)
    command |= (1<<M_RIGHT);
  
  write_buffer(command, COMMAND | temp);
}

/*
 * Czysci pole tekstowe wyswietlacza.
 */
void Disp_clr_txt(void) {
  Disp_set_cxy(0, 0);
  settings_mode |= (1<<M_INC);
  settings_mode &= ~(1<<M_MOVE);
  
  write_buffer(1<<CLEAR, COMMAND);
  #if DISP_EN_NUM == 2
    write_buffer(1<<CLEAR, COMMAND | EN_2);
  #endif
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
  settings_mode |= (1<<M_INC);
  settings_mode &= ~(1<<M_MOVE);
  
  write_buffer(1<<RETURN, COMMAND);
}

/*
 * Wyswietla znak na aktualnej pozycji kursora.
 * 
 * parametry:
 *    character - kod znaku do wyswietlenia
 */
void Disp_putc(char character) {
  uint8_t temp = 0;
  #if DISP_EN_NUM == 2
    if (active_module == 2)
      temp = EN_2;
  #endif
  
  if (settings_mode & (1<<M_INC))
    cur_x++;
  else
    cur_x--;
    
  write_buffer((uint8_t)character, DATA | temp);
    
  #if DISP_SPECIAL == 16
  
    if (((cur_x == 8) && (settings_mode & (1<<M_INC)))
     || ((cur_x == 7) && !(settings_mode & (1<<M_INC))))
      Disp_set_cxy(cur_x, cur_y);
      
  #elif DISP_SPECIAL == 20
  
    if (((cur_x == 10) && (settings_mode & (1<<M_INC)))
     || ((cur_x == 9) && !(settings_mode & (1<<M_INC))))
      Disp_set_cxy(cur_x, cur_y);
  
  #endif
}

/*
 * Pobiera znak z aktualnej pozycji kursora.
 * 
 * zwraca:
 *    kod znaku na aktualnej pozycji kursora
 */
#if DISP_USE_RW_LINE == 1
char Disp_getc(void) {
  #if DISP_UPDATE_ON_READ == 1
    Disp_update();
  #endif
  
  if (settings_mode & (1<<M_INC))
    cur_x++;
  else
    cur_x--;
    
  char temp = (char)read_data(active_module);
    
  #if DISP_SPECIAL == 16
  
    if (((cur_x == 8) && (settings_mode & (1<<M_INC)))
     || ((cur_x == 7) && !(settings_mode & (1<<M_INC))))
      Disp_set_cxy(cur_x, cur_y);
      
  #elif DISP_SPECIAL == 20
  
    if (((cur_x == 10) && (settings_mode & (1<<M_INC)))
     || ((cur_x == 9) && !(settings_mode & (1<<M_INC))))
      Disp_set_cxy(cur_x, cur_y);
  
  #endif
    
  return temp;
}
#endif

/*ing
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
 * Pobiera lancuch znakow poczawszy od aktualnej pozycji kursora.
 * 
 * parametry:
 *    string - wskaznik poczatku docelowego lancucha znakow
 *    count  - liczba znakow do odczytania
 */
#if DISP_USE_RW_LINE == 1
void Disp_gets(char* string, uint8_t count) {
  while (count--)
    *(string++) = Disp_getc();
    
  *string = 0;
}
#endif

/*
 * Wybiera tablice szablonow znakowych.
 * 
 * parametry:
 *    templates - wskaznik poczatku tablicy szablonow
 *    count     - dlugosc tablicy szablonow w bajtach
 *    addr      - numer pierwszego szablonu do zapisania
 */
void Disp_set_ctempl(uint8_t* templates, uint8_t count, uint8_t addr) {
  write_buffer((1<<CGADR)|(addr<<3), COMMAND);
  #if DISP_EN_NUM == 2
    write_buffer((1<<CGADR)|(addr<<3), COMMAND | EN_2);
  #endif
  
  while (count--) {
    write_buffer(*(templates++), DATA);
    #if DISP_EN_NUM == 2
      write_buffer(*(templates), DATA | EN_2);
    #endif
  }
    
  Disp_set_cxy(cur_x, cur_y);
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
  
    write_buffer((1<<CGADR)|(addr<<3), COMMAND);
    #if DISP_EN_NUM == 2
      write_buffer((1<<CGADR)|(addr<<3), COMMAND | EN_2);
    #endif
    
    while (count--) {
      write_buffer(get_const(templates++), DATA);
      #if DISP_EN_NUM == 2
        write_buffer(get_const(templates), DATA | EN_2);
      #endif
    }
     
    Disp_set_cxy(cur_x, cur_y);
      
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
 *    addr      - numer pierwszego szablonu do odczytu
 */
#if DISP_USE_RW_LINE == 1
void Disp_get_ctempl(uint8_t* templates, uint8_t count, uint8_t addr) {
  write_buffer((1<<CGADR)|(addr<<3), COMMAND);
  
  while (count--)
    *(templates++) = read_data(1);
  
  Disp_set_cxy(cur_x, cur_y);
}
#endif


/****************************************************************************
 * Deklaracje funkcji prywatnych
 ***************************************************************************/
 
/*
 * Oczekuje na gotowosc wyswietlacza.
 */
#if DISP_USE_RW_LINE == 1
static uint8_t wait_if_busy(uint8_t no) {
  uint8_t rs_old;
  uint8_t result = 0;
  uint16_t time_cnt = 0;
  
  rs_old = get_rs();

  chg_dir_db(IN);
  set_rw();
  rst_rs();
  
  while (!error) {
    #if DISP_EN_NUM == 2
      if (no == 1)
        set_en1();
      else
        set_en2();
    #else
      set_en1();
    #endif
    
    _nop();
    _nop();
      
    #if DISP_DB_WIDTH == 8
  
      result = read_db();
      _nop();
      #if DISP_EN_NUM == 2
        if (no == 1)
          rst_en1();
        else
          rst_en2();
      #else
        rst_en1();
      #endif
        
    #elif DISP_DB_WIDTH == 4
    
      result = read_db()<<4;
      _nop();
      #if DISP_EN_NUM == 2
        if (no == 1)
          rst_en1();
        else
          rst_en2();
      #else
        rst_en1();
      #endif
        
      _nop();
      _nop();
      _nop();
        
      #if DISP_EN_NUM == 2
        if (no == 1)
          set_en1();
        else
          set_en2();
      #else
        set_en1();
      #endif
      _nop();
      _nop();
      result |= read_db();
      _nop();
      #if DISP_EN_NUM == 2
        if (no == 1)
          rst_en1();
        else
          rst_en2();
      #else
        rst_en1();
      #endif
        
    #else
    #  error "Okreslono nieprawidlowa szerokosc magistrali wyswietlacza"
    #endif
    
    if (!(result & 0x80))
      break;
    else if (++time_cnt > DISP_MAX_TIMEOUT * 1000) {
      error = 1;
      break;
    }
    
    _nop();
    _nop();
    _nop();
  }

  rst_rw();
  chg_dir_db(OUT);
  
  if (rs_old)
    set_rs();
  else
    rst_rs();
    
  return result;
}
#endif
 
/*
 * Przesyla 1 bajt danych do wyswietlacza.
 */
static void write_data(uint8_t data, uint8_t no) {
  
  #if DISP_USE_RW_LINE == 1
    wait_if_busy(no);
  #endif  
  
  #if DISP_EN_NUM == 2
    if (no == 1)
      set_en1();
    else
      set_en2();
  #else
    set_en1();
  #endif
    
  #if DISP_DB_WIDTH == 8

    write_db(data);
    _nop();
    _nop();
    _nop();
    #if DISP_EN_NUM == 2
      if (no == 1)
        rst_en1();
      else
        rst_en2();
    #else
      rst_en1();
    #endif
      
  #elif DISP_DB_WIDTH == 4
  
    write_db(data>>4);
    _nop();
    _nop();
    _nop();
    #if DISP_EN_NUM == 2
      if (no == 1)
        rst_en1();
      else
        rst_en2();
    #else
      rst_en1();
    #endif
      
    _nop();
    _nop();
    _nop();
      
    #if DISP_EN_NUM == 2
      if (no == 1)
        set_en1();
      else
        set_en2();
    #else
      set_en1();
    #endif
    write_db(data);
    _nop();
    _nop();
    _nop();
    #if DISP_EN_NUM == 2
      if (no == 1)
        rst_en1();
      else
        rst_en2();
    #else
      rst_en1();
    #endif
      
  #else
  #  error "Okreslono nieprawidlowa szerokosc magistrali wyswietlacza"
  #endif
  
  delay_dus((uint8_t)(1 + 270 * 4.3 / DISP_OSC));

  #if DISP_USE_RW_LINE == 0
    if ((data == (1<<CLEAR) || data == (1<<RETURN)) \
        && !get_rs())
      delay_ms((uint8_t)(1 + 270 * 1.64 / DISP_OSC));
  #endif
}

/*
 * Przesyla komende.
 */
__inline__ static void send_cmd(uint8_t cmd, uint8_t no) {
  rst_rs();
  write_data(cmd, no);
  set_rs();
}

/*
 * Odczytuje 1 bajt danych z wyswietlacza.
 */
#if DISP_USE_RW_LINE == 1
static uint8_t read_data(uint8_t no) {
  uint8_t result;
  
  wait_if_busy(no);
  
  chg_dir_db(IN);
  set_rw();
  _nop();
  
  #if DISP_EN_NUM == 2
    if (no == 1)
      set_en1();
    else
      set_en2();
  #else
    set_en1();
  #endif
  
  _nop();
  _nop();
    
  #if DISP_DB_WIDTH == 8

    result = read_db();
    _nop();
    #if DISP_EN_NUM == 2
      if (no == 1)
        rst_en1();
      else
        rst_en2();
    #else
      rst_en1();
    #endif
      
  #elif DISP_DB_WIDTH == 4
  
    result = read_db()<<4;
    _nop();
    #if DISP_EN_NUM == 2
      if (no == 1)
        rst_en1();
      else
        rst_en2();
    #else
      rst_en1();
    #endif
      
    _nop();
    _nop();
    _nop();
      
    #if DISP_EN_NUM == 2
      if (no == 1)
        set_en1();
      else
        set_en2();
    #else
      set_en1();
    #endif
    _nop();
    _nop();
    result |= read_db();
    _nop();
    #if DISP_EN_NUM == 2
      if (no == 1)
        rst_en1();
      else
        rst_en2();
    #else
      rst_en1();
    #endif
      
  #else
  #  error "Okreslono nieprawidlowa szerokosc magistrali wyswietlacza"
  #endif
  
  rst_rw();
  chg_dir_db(OUT);
  
  return result;
}
#endif

/*
 * Zapisuje wartosc do bufora FIFO.
 */
static void write_buffer(uint8_t data, uint8_t type) {
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
  
  *(fifo_wr_ptr++) = type;
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
    uint8_t type;
    
    disable_interrupts();
    
    type = *(fifo_rd_ptr++);
    *data = *(fifo_rd_ptr++);
    
    if (fifo_rd_ptr == fifo + DISP_BUFFER_LEN*2)
      fifo_rd_ptr = fifo;
    
    revert_interrupts();
    
    return type;
  }
}
