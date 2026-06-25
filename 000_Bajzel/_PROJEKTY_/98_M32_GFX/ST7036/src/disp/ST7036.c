/****************************************************************************
* Biblioteka obslugi wyswietlacza alfanumerycznego, opartego na sterowniku
* ST7036 lub odpowiedniku.
*
* Nazwa pliku: ST7036.c
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

#include <inttypes.h>
#include <ST7036.h>
#include <disp_conf.h>
#include <platf_conf.h>

#define EMPTY   0
#define DATA    1
#define COMMAND 2

/*
 * Definicje etykiet dla komend w trybie normalnym i rozszerzonym
 */
#define CLEAR   0x01  // czyszczenie wyswietlacza i powrot kursora

#define RETURN  0x02  // powrot kursora do pozycji poczatkowej

#define MODE    0x04  // zmiana trybu zapisu pamieci
#define M_INC   0x02  //   adres inkrementowany przy kazdym zapisie
#define M_MOVE  0x01  //   okno przesuwane przy kazdym zapisie

#define CTRL    0x08  // sterowanie wyswietlaczem
#define C_DI_ON 0x04  //   wyswietlanie wlaczone
#define C_CU_ON 0x02  //   kursor wlaczony
#define C_BL_ON 0x01  //   miganie kursora wlaczone

#define SET     0x20  // ustawienia interfejsu i wyswietlacza
#define S_8BIT  0x10  //   interfejs 8-bitowy
#define S_2LIN  0x08  //   wyswietlacz 2-liniowy

#define DDADR   0x80  // zmiana adresu DDRAM

/*
 * Definicje etykiet dla komend w trybie rozszerzonym
 */
#define ES_IS0    0x00  //   zestaw instrukcji nr 0 
#define ES_IS1    0x01  //   zestaw instrukcji nr 1
#define ES_IS2    0x02  //   zestaw instrukcji nr 2
#define ES_DOUBLE 0x04  //   font o podwojnej wysokosci

/*
 * Definicje etykiet dla komend w trybie normalnym i w
 * trybie rozszerzonym przy IS=0
 */
#define N_MOVE    0x10  // przesuwanie
#define NM_WIN    0x08  //   przesuwanie okna
#define NM_RIGHT  0x04  //   przesuwanie w prawo

#define N_CGADR   0x40  // zmiana adresu CGRAM

/*
 * Definicje etykiet dla komend w trybie rozszerzonym (IS=1)
 */
#define E1_BIAS   0x14  // sposob zasilania LCD
#define E1B_1_4   0x08  //   stopien podzialu napiecia zasilania rowny 1/4 
#define E1B_3LIN  0x01  //   wyswietlacz 3-liniowy

#define E1_ICADR  0x40  // zmiana adresu ICRAM

#define E1_POWER  0x50  // nastawa zasilania
#define E1P_ICON  0x08  //   wyswietlanie ikon wlaczone
#define E1P_BOOST 0x04  //   konwerter napiecia wlaczony

#define E1_FOLLOW 0x60  // nastawa wtornikow napiecia
#define E1F_ON    0x08  //   wtornik napiecia wlaczony

#define E1_CONTR  0x70  // nastawa kontrastu

/*
 * Definicje etykiet dla komend w trybie rozszerzonym (IS=2)
 */
#define E2_DPOS_H 0x18  // font o podwojnej wysokosci w liniach gornych
#define E2_DPOS_L 0x10  // font o podwojnej wysokosci w liniach dolnych

/*
 * Definicje makr
 */


/*
 * Definicje zmiennych prywatnych
 */
static volatile uint8_t error = 1;
static volatile uint8_t overflow = 0;
static uint8_t update = DEFERRED;
static uint8_t cur_x;
static uint8_t cur_y;

static uint8_t settings_mode = MODE | M_INC;
static uint8_t settings_ctrl = CTRL;

static uint8_t first_time = 1;

static uint8_t fifo[DISP_BUFFER_LEN * 2];
static volatile uint8_t* fifo_rd_ptr;
static volatile uint8_t* fifo_wr_ptr;


/*
 * Definicje funkcji prywatnych
 */
static void write_data(uint8_t data);
__inline__ static void send_cmd(uint8_t cmd);

#if DISP_USE_RW_LINE == 1 && DISP_DB_WIDTH != 1
  static uint8_t wait_if_busy(void);
  static uint8_t read_data(void);
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

  init_ports();
  
  #if DISP_DB_WIDTH == 1
    set_cs1();
  #endif
  
  if (first_time) {
    delay_ms(40);
    first_time = 0;
  }
  
  #if (DISP_DB_WIDTH == 4) || (DISP_DB_WIDTH == 8)
  
    write_db(SET | S_8BIT);
    
    for (uint8_t i = 0; i<3; i++) {
      set_en1();
      nop();
      nop();
      rst_en1();
      
      delay_dus((uint8_t)(1 + 540 * 1.85 / DISP_OSC)); 
    }
  #endif
  
  #if DISP_DB_WIDTH == 4
  
    write_db(SET);
    set_en1();
    nop();
    nop();
    rst_en1();
    
    delay_dus((uint8_t)(1 + 540 * 1.85 / DISP_OSC));
    
  #endif
  
  error = 0;
  Disp_flush_buffer();
  
  write_buffer(SET \
           | ((DISP_DB_WIDTH == 8 || DISP_DB_WIDTH == 1)? S_8BIT : 0) \
           | ((DISP_C_ROWS_NUM >= 2)? S_2LIN : 0) \
           | ES_IS0, \
         COMMAND);
  
  #if DISP_EXT_MODE == 1
    write_buffer(SET \
           | ((DISP_DB_WIDTH == 8 || DISP_DB_WIDTH == 1)? S_8BIT : 0) \
           | ((DISP_C_ROWS_NUM >= 2)? S_2LIN : 0) \
           | ES_IS1, \
         COMMAND);
         
    write_buffer(E1_BIAS \
           | ((DISP_POW_SUPPLY == 1)? E1B_1_4 : 0) \
           | ((DISP_C_ROWS_NUM == 3)? E1B_3LIN : 0),
         COMMAND);
           
   
    write_buffer(E1_POWER \
           | ((DISP_POW_SUPPLY == 0)? E1P_BOOST : 0) \
           | (((DISP_CONTR_FINE & 0x3F) >> 4)), \
         COMMAND);
         
    write_buffer(E1_FOLLOW | E1F_ON | (DISP_CONTR_COAR & 0x07), COMMAND);
    
    write_buffer(E1_CONTR | (DISP_CONTR_FINE & 0x0F), COMMAND);
          
  #endif
  
  write_buffer(SET \
         | ((DISP_DB_WIDTH == 8 || DISP_DB_WIDTH == 1)? S_8BIT : 0 )\
         | ((DISP_C_ROWS_NUM >= 2)? S_2LIN : 0) \
         | ES_IS0, \
       COMMAND);
  
  settings_mode = MODE;
  settings_ctrl = CTRL;
  
  Disp_set_input_mode(INC);
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
    settings_ctrl |= C_DI_ON;
  else
    settings_ctrl &= ~(C_DI_ON);
    
  write_buffer(settings_ctrl, COMMAND);
}

/*
 * Okresla kontrast wyswietlacza.
 * 
 * parametry:
 *    coarse - kontrast zgrubny (0...7)
 *    fine   - kontrast dokladny (0...63)
 */
#if DISP_EXT_MODE == 1
void Disp_set_contrast(uint8_t coarse, uint8_t fine) {
  write_buffer(SET \
           | ((DISP_DB_WIDTH == 8 || DISP_DB_WIDTH == 1)? S_8BIT : 0) \
           | ((DISP_C_ROWS_NUM >= 2)? S_2LIN : 0) \
           | ES_IS1, \
         COMMAND);
    
  write_buffer(E1_POWER \
           | ((DISP_POW_SUPPLY == 0)? E1P_BOOST : 0) \
           | (((fine & 0x3F) >> 4)), \
         COMMAND);
         
  write_buffer(E1_FOLLOW | E1F_ON | (coarse & 0x07), COMMAND);
    
  write_buffer(E1_CONTR | (fine & 0x0F), COMMAND);
  
  write_buffer(SET \
         | ((DISP_DB_WIDTH == 8 || DISP_DB_WIDTH == 1)? S_8BIT : 0 )\
         | ((DISP_C_ROWS_NUM >= 2)? S_2LIN : 0) \
         | ES_IS0, \
       COMMAND);
}
#endif

/*
 * Okresla wielkosc znakow (font).
 * 
 * parametry:
 *    font - wysokosc w pikselach (8 lub 16)
 *    position - nastawa dla linii gornych (0) lub dolnych (1)
 */
#if DISP_EXT_MODE == 1
void Disp_set_font(uint8_t font, uint8_t position) {
  write_buffer(SET \
           | ((DISP_DB_WIDTH == 8 || DISP_DB_WIDTH == 1)? S_8BIT : 0) \
           | ((DISP_C_ROWS_NUM >= 2)? S_2LIN : 0) \
           | ((font == 16)? ES_DOUBLE : 0) \
           | ES_IS2, \
         COMMAND);
    
  write_buffer(((position == 0)? E2_DPOS_H : E2_DPOS_L), COMMAND);
    
  write_buffer(SET \
           | ((DISP_DB_WIDTH == 8 || DISP_DB_WIDTH == 1)? S_8BIT : 0) \
           | ((DISP_C_ROWS_NUM >= 2)? S_2LIN : 0) \
           | ((font == 16)? ES_DOUBLE : 0) \
           | ES_IS0, \
         COMMAND);
}
#endif

/*
 * Okresla atrybuty kursora znakowego.
 * 
 * parametry:
 *    on    - wlaczony (ON) badz wylaczony (OFF)
 *    blink - migajacy (BLINK) badz niemigajacy (NO_BLINK)
 */
void Disp_set_cur(uint8_t on, uint8_t blink) {
  if (on == ON)
    settings_ctrl |= (C_CU_ON);
  else
    settings_ctrl &= ~(C_CU_ON);
    
  if (blink == BLINK)
    settings_ctrl |= (C_BL_ON);
  else
    settings_ctrl &= ~(C_BL_ON);
  
  write_buffer(settings_ctrl, COMMAND);
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
    settings_mode |= (M_INC);
  else
    settings_mode &= ~(M_INC);
    
  write_buffer(settings_mode, COMMAND);
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
  
  do {
    dtype = read_buffer(&data);
    
    if (dtype & DATA)
      write_data(data);
    else if (dtype & COMMAND)
      send_cmd(data);
      
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
  
  addr += cx;
  
  write_buffer(addr | DDADR, COMMAND);
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
  
  uint8_t command = N_MOVE;
  
  if (dir == RIGHT)
    command |= NM_RIGHT;
  
  write_buffer(command, COMMAND);
}

/*
 * Przesuwa okno o jedna pozycje.
 * 
 * parametry:
 *    dir - w prawo (RIGHT) lub w lewo (LEFT)
 */ 
void Disp_move_txt_window(uint8_t dir) {
  
  uint8_t command = N_MOVE | NM_WIN;
  
  if (dir == RIGHT)
    command |= NM_RIGHT;
  
  write_buffer(command, COMMAND);
}

/*
 * Czysci pole tekstowe wyswietlacza.
 */
void Disp_clr_txt(void) {
  Disp_set_cxy(0, 0);
  settings_mode |= M_INC;
  settings_mode &= ~(M_MOVE);
  
  write_buffer(CLEAR, COMMAND);
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
  settings_mode |= M_INC;
  settings_mode &= ~(M_MOVE);
  
  write_buffer(RETURN, COMMAND);
}

/*
 * Wyswietla znak na aktualnej pozycji kursora.
 * 
 * parametry:
 *    character - kod znaku do wyswietlenia
 */
void Disp_putc(char character) {
  
  if (settings_mode & M_INC)
    cur_x++;
  else
    cur_x--;
    
  write_buffer((uint8_t)character, DATA);
}

/*
 * Pobiera znak z aktualnej pozycji kursora.
 * 
 * zwraca:
 *    kod znaku na aktualnej pozycji kursora
 */
#if DISP_USE_RW_LINE == 1 && DISP_DB_WIDTH != 1
char Disp_getc(void) {
  #if DISP_UPDATE_ON_READ == 1
    Disp_update();
  #endif
  
  if (settings_mode & M_INC)
    cur_x++;
  else
    cur_x--;
    
  char temp = (char)read_data();
    
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
#if DISP_USE_RW_LINE == 1 && DISP_DB_WIDTH != 1
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
  write_buffer(N_CGADR|(addr<<3), COMMAND);
  
  while (count--)
    write_buffer(*(templates++), DATA);
    
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
  
    write_buffer(N_CGADR|(addr<<3), COMMAND);
    
    while (count--)
      write_buffer(get_const(templates++), DATA);
     
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
#if DISP_USE_RW_LINE == 1 && DISP_DB_WIDTH != 1
void Disp_get_ctempl(uint8_t* templates, uint8_t count, uint8_t addr) {
  write_buffer(N_CGADR|(addr<<3), COMMAND);
  
  while (count--)
    *(templates++) = read_data();
  
  Disp_set_cxy(cur_x, cur_y);
}
#endif


/****************************************************************************
 * Deklaracje funkcji prywatnych
 ***************************************************************************/
 
/*
 * Oczekuje na gotowosc wyswietlacza.
 */
#if DISP_USE_RW_LINE == 1 && DISP_DB_WIDTH != 1
static uint8_t wait_if_busy(void) {
  uint8_t rs_old;
  uint8_t result = 0;
  uint16_t time_cnt = 0;
  
  rs_old = get_rs();

  chg_dir_db(IN);
  set_rw();
  rst_rs();
  nop();
  
  while (!error) {
    set_en1();
    
      nop();
      nop();
      nop();
      nop();
      nop();
      
    #if DISP_DB_WIDTH == 8
  
      result = read_db();
      rst_en1();
        
    #elif DISP_DB_WIDTH == 4
    
      result = read_db()<<4;
      rst_en1();
        
      nop();
      nop();
        
      set_en1();
      nop();
      nop();
      nop();
      nop();
      nop();
      result |= read_db();
      rst_en1();
        
    #else
    #  error "Okreslono nieprawidlowa szerokosc magistrali wyswietlacza"
    #endif
    
    if (!(result & 0x80))
      break;
    else if (++time_cnt > DISP_MAX_TIMEOUT * 1000) {
      error = 1;
      break;
    }
    
    nop();
    nop();
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
static void write_data(uint8_t data) {
  
  #if DISP_USE_RW_LINE == 1 && DISP_DB_WIDTH != 1
    wait_if_busy();
  #endif  
    
  #if DISP_DB_WIDTH == 8
  
    set_en1();
    write_db(data);
    nop();
    nop();
    rst_en1();
      
  #elif DISP_DB_WIDTH == 4
  
    set_en1();
    write_db(data>>4);
    nop();
    nop();
    rst_en1();
      
    nop();
    nop();
     
    set_en1();
    write_db(data);
    nop();
    nop();
    rst_en1();
    
  #elif DISP_DB_WIDTH == 1
  
    rst_cs1();
    nop();
  
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
    
    nop();
    nop();
    nop();
    
    set_cs1();
      
  #else
  #  error "Okreslono nieprawidlowa szerokosc magistrali wyswietlacza"
  #endif
  
  delay_dus((uint8_t)(1 + 540 * 1.85 / DISP_OSC));

  #if DISP_USE_RW_LINE == 0 || DISP_DB_WIDTH == 1
    if ((data == CLEAR || data == RETURN) \
        && !get_rs())
      delay_ms((uint8_t)(1 + 540 * 0.76 / DISP_OSC));
  #endif
}

/*
 * Przesyla komende.
 */
__inline__ static void send_cmd(uint8_t cmd) {
  rst_rs();
  write_data(cmd);
  set_rs();
}

/*
 * Odczytuje 1 bajt danych z wyswietlacza.
 */
#if DISP_USE_RW_LINE == 1 && DISP_DB_WIDTH != 1
static uint8_t read_data(void) {
  uint8_t result;
  
  wait_if_busy();
  
  chg_dir_db(IN);
  set_rw();
  nop();
  
  set_en1();
  
  nop();
  nop();
  nop();
  nop();
  nop();
    
  #if DISP_DB_WIDTH == 8

    result = read_db();
    rst_en1();
      
  #elif DISP_DB_WIDTH == 4
  
    result = read_db()<<4;
    rst_en1();
      
    nop();
    nop();
      
    set_en1();
    nop();
    nop();
    nop();
    nop();
    nop();
    result |= read_db();
    rst_en1();
      
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
