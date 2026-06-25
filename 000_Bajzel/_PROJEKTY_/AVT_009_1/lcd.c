/////////////////////////////////////////////////////////////////////////////////////////////
// lcd.c - plik Ÿród³owy do obs³ugi alfanumerycznego wyœwietlacza LCD.
//         ! Dynamiczne przydzielanie znaków specjalnych !
//
// Autor: Rados³aw Koppel   Kompilator: WinAVR 20060125
/////////////////////////////////////////////////////////////////////////////////////////////
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include <string.h>
#include <avr/pgmspace.h>

#include "makra.h"
#include "harddef.h"
#include "lcd.h"
#include "langsys.h"

//__________________________________________________________________________________________
// Definicje sta³ych
// Komendy steruj¹ce wyœwietlaczem
#define LCDC_CLS      0x01
#define LCDC_HOME      0x02
#define LCDC_MODE      0x04
   #define LCDC_MODER      0x02
   #define LCDC_MODEL      0
   #define LCDC_MODEMOVE   0x01
#define LCDC_ON       0x08
   #define LCDC_ONDISPLAY   0x04
   #define LCDC_ONCURSOR   0x02
   #define LCDC_ONBLINK   0x01
#define LCDC_SHIFT      0x10
   #define LCDC_SHIFTDISP   0x08
   #define LCDC_SHIFTR      0x04
   #define LCDC_SHIFTL      0
#define LCDC_FUNC      0x20
   #define LCDC_FUNC8b      0x10
   #define LCDC_FUNC4b      0
   #define LCDC_FUNC2L      0x08
   #define LCDC_FUNC1L      0
   #define LCDC_FUNC5x10   0x4
   #define LCDC_FUNC5x7   0
#define LCDC_CGA      0x40
#define LCDC_DDA      0x80

//__________________________________________________________________________________________
// Deklaracje funkcji które musz¹ byæ tutaj zdeklarowane
static int lcd_put(char c, FILE* f);

//__________________________________________________________________________________________
// Zmienne
// Korzystamy z nowej mo¿liwoœci avrlibc 1.4
static FILE lcd_fLCD_temp = FDEV_SETUP_STREAM(lcd_put, NULL, _FDEV_SETUP_WRITE);
#define lcd_fLCD (&lcd_fLCD_temp)

// Uwaga - wyœwietlacz niewykorzystuje kodów 0x80-0x9f. To 32 kody które zostan¹ wykorzystane dla znaków specjalnych
static char lcd_buffer[LCD_SX*LCD_SY];
// Aktualne pozycja kursora (dotyczy bufora i jest aktualizwana przy odœwierzaniu napisu)
static uint8_t lcd_curpos;
// Informacja o tym jakie znaki specjalne zosta³y zapisane do wyœwietlacza
static uint8_t lcd_spec[8];
// Zmienna przechowuj¹ca flagi na temat w³¹czenia wyœwietlacza (wyœwietlacz, kursor, migaj¹cy kursor)
static uint8_t lcd_status;

//__________________________________________________________________________________________
// Obs³uga LCD - funkcje niskiego poziomu

// Impuls ENABLE - przede wszystkim dla zapisu
// Odczyt wymaga trochê bardziej skomplikowanej sekwencji
#define lcd_epulse() \
   {PORT(LCD_CTRLPORT) |= 1<<LCD_E; \
   _delay_us(0.25); \
   PORT(LCD_CTRLPORT) &= ~(1<<LCD_E);}

// Odbiór danej - funkcja wewnêtrzna
static inline uint8_t lcd_Get(void)
{
   uint8_t dana;
   // Ustawienie portu lcd jako wejœcia
   DDR(LCD_DATAPORT) = 0x00;
   // Aktywacja odczytu
   PORT(LCD_CTRLPORT) |= 1<<LCD_E;
   // \/ Uwaga - sprawdzi³em eksperymentalnie,
   // \/ ¿e 0.5us to czasami za ma³o
   // \/ przy pod³¹czonym programatorze wyœwietlacz
   // \/ wci¹¿ gubi³ dane
   _delay_us(1);
   // Skopiowanie danych z wyjœcia modu³u
   dana = PIN(LCD_DATAPORT);
   // Deaktywacja wyjœcia ENABLE
   PORT(LCD_CTRLPORT) &= ~(1<<LCD_E);
   return dana;
}

uint8_t lcd_GetBF(void)
{
   // Wysterowanie wyprowadzeñ do odczytu statusu
   PORT(LCD_CTRLPORT) |= 1<<LCD_RW;
   PORT(LCD_CTRLPORT) &= ~(1<<LCD_RS);
   // Odczekanie wymaganego przez sterownik czasu
   _delay_us(0.25);
   // Odczyt danej
   return lcd_Get();
}

// Sprawdza flagê zajêtoœci i czeka na jej wyzerowanie
void lcd_WaitBF(void)
{
   // Bit zajêtoœci to bit najstarszy
   while(0 != (0x80 & lcd_GetBF())) {};
}

// Wys³anie danej - funkcja wewnêtrzna
static void lcd_Send(uint8_t dana)
{
   // Ustawienie portu lcd jako wyjœcia
   DDR(LCD_DATAPORT) = 0xFF;
   // Pres³anie na port danej
   PORT(LCD_DATAPORT) = dana;
   // Przes³anie do lcd
   lcd_epulse();
}

// Wys³anie danej do pamiêci DDRAM lub CGRAM
// Zale¿nie od ustawionego adresu
void lcd_SendData(uint8_t dana)
{
   lcd_WaitBF();
   // Wysterowanie wyprowadzeñ dla zapisu danej
   PORT(LCD_CTRLPORT) &= ~(1<<LCD_RW);
   PORT(LCD_CTRLPORT) |= 1<<LCD_RS;
   // Odczekanie oraz wys³anie
   _delay_us(0.25);
   lcd_Send(dana);
}

// Wys³anie instrukcji steruj¹cej
void lcd_SendInstr(uint8_t dana)
{
   lcd_WaitBF();
   // Wysterowanie wyprowadzeñ dla zapisu instrukcji
   PORT(LCD_CTRLPORT) &= ~(1<<LCD_RW | 1<<LCD_RS);
   // Odczekanie oraz wys³anie
   _delay_us(0.25);
   lcd_Send(dana);
}

//__________________________________________________________________________________________
// Funkcje obs³uguj¹ce wejœcie wyjœcie strumienia
static int lcd_put(char c, FILE* f)
{
   // Zabezpieczenie przed przepe³nieniem
   if(lcd_curpos >= ELEMS(lcd_buffer))
      lcd_curpos = 0;
   // Zapis do bufora
   lcd_buffer[lcd_curpos++] = c;
   return 0;
}

//__________________________________________________________________________________________
// Funkcje interfejsu

// inicjacja LCD (uwaga - nie w³¹czam wyœwietlacza ani kursora)
void lcd_Init(void)
{
   lcd_Cls();
   PORT(LCD_CTRLPORT) &= ~(1<<LCD_RW | 1<<LCD_RS);
   _delay_us(0.25);
   lcd_Send(LCDC_FUNC | LCDC_FUNC8b);
   _delay_ms(4.3);
   lcd_Send(LCDC_FUNC | LCDC_FUNC8b);
   _delay_us(200);
   lcd_Send(LCDC_FUNC | LCDC_FUNC8b);
   // Ju¿ mo¿na sprawdzaæ BF
#if LCD_SY == 1
   lcd_SendInstr(LCDC_FUNC | LCDC_FUNC8b | LCDC_FUNC1L);
#else
   lcd_SendInstr(LCDC_FUNC | LCDC_FUNC8b | LCDC_FUNC2L);
#endif
   lcd_SendInstr(LCDC_ON);
   lcd_SendInstr(LCDC_CLS);
   lcd_SendInstr(LCDC_MODE | LCDC_MODER);
   lcd_WaitBF();
   // Koniec inicjacji. Wyj¹tkowo czekam na zakoñczenie. Normalnie BF jest sprawdzana przed wykonaniem indtrukcji
   // - daje to wiêksz¹ oszczêdnoœæ czasu. Podczas inicjacji nie ma to wielkiego znaczenia.
}

// Zerowanie bufora wyœwietlacza
void lcd_Cls(void)
{
   lcd_curpos = 0;
   memset(lcd_buffer, ' ', sizeof(lcd_buffer));
}

// kontrola w³¹czenia wyœwietlacza, kursora, migaj¹cego kursora
void lcd_SetStatus(uint8_t status)
{
   lcd_status = status;
}

// Odswierzenie statusu
inline void lcd_UpdateStatus(void)
{
   lcd_SendInstr(LCDC_ON | lcd_status);
}

// Funkcja wewnêtrzna podaj¹ca pocz¹tek danej linii
static uint8_t lcd_LineStart(uint8_t line)
{
#if LCD_SY == 1
   return LCD_LINE1;
#elif LCD_SY == 2
   if(line == 0)
      return LCD_LINE1;
   else
      return LCD_LINE2;
#elif LCD_SY == 4
   if(line == 0)
      return LCD_LINE1;
   else if(linenum == 1)
      return LCD_LINE2;
   else if(linenum == 2)
      return LCD_LINE3;
   else
      return LCD_LINE4;
#else
#error Unknown display type!
#endif
}

// Ustawienie kursora
inline void lcd_GoToAdr(uint8_t adr)
{
   lcd_curpos = adr;
}

// Odswierzenie pozycji kursora
void lcd_UpdateCurPos(void)
{
   // Obliczenie adresu w wyœwietlaczu
   uint8_t adres =
      lcd_LineStart(lcd_curpos / LCD_SX) + (lcd_curpos % LCD_SX);
   // Wys³anie instrkucji ustawiaj¹cej kursor
   lcd_SendInstr(LCDC_DDA | adres);
}

// Wewnêtrzna pomocnicza funkcja sprawdzaj¹ca czy dany znak jest znakiem specjalnym
static uint8_t lcd_IsSpec(char c)
{
   // Przydzia³ znaków specjalnych na obszar którego LCD nie wykorzystuje
   return (c >= 0x80) && (c <= 0x9f);
}

// Zamiana znaku specjalnego na odpowiedni numer w tablicy symboli
static inline uint8_t lcd_Spec2Index(char c)
{
   return c-0x80;
}

// Funkcja sprawdzajaca czy w tablicy jest podany znak
// Zwraca: indeks w tablicy gdzie znajduje sie dany znak
//  lub kod znaku alternatywnego (zawsze >= 0x20)
static uint8_t lcd_GetSpec(uint8_t s_index)
{
   // Zabezpieczenie
   if(langsys_GetSpec() == NULL)
      return 0x20;
   uint8_t a;
   for(a=0; a<ELEMS(lcd_spec); a++)
   {
      // 0xff oznacza, ¿e nie ma ju¿ dalej wpisów
      if(lcd_spec[a] == 0xff)
         break;
      // Jeœli znaleziono...
      else if(lcd_spec[a] == s_index)
         return a;
   }
   // Nic nie znaleziono
   return pgm_read_byte(&(langsys_GetSpec()[s_index].cAlt));
}

// Makro pomocnicze
#define LCD_SPECNF(spec) (spec > 7)

// Funkcja przydzielaj¹ca znaki specjalne
// Zwraca iloœæ ró¿nych znaków jak¹ znaleziono. Jeœli > 8 czêœci znaków nie uda³o siê przydzieliæ
uint8_t lcd_PrepareSpec(void)
{
   // Analizujê ca³y ³añcuch danych i zapisujê kolejno znalezione znaki
   char* pbuffer = lcd_buffer;
   uint8_t n;
   uint8_t cnt=0;
   char znak;
   // Na pocz¹tku "zerowanie" tablicy znaków specjalnych
   memset(lcd_spec, 0xff, sizeof(lcd_spec));
   // dodawanie wpisów
   for(n=0; n<ELEMS(lcd_buffer); n++)
   {
      znak = *pbuffer++;
      // Jeœli znaleziony znak jest specjalny
      if(lcd_IsSpec(znak))
      {
         // Przeszukanie tablicy
         znak = lcd_Spec2Index(znak);
         // Jeœli nie znaleziono - dodawanie
         if(LCD_SPECNF(lcd_GetSpec(znak)))
         {
            // dodaj do tablicy tylko jeœli
            // nie wyst¹pi³o przepe³nienie
            if(cnt < ELEMS(lcd_spec))
               lcd_spec[cnt] = znak;
            // Licznik zwiêkszany zawsze
            // dla statystyki
            ++cnt;
         }
      }
   }
   return cnt;
}

// Funkcja zapisuj¹ca definicje znaków do pamiêci CGRAM wyœwietlacza
void lcd_UpdateCGRAM(void)
{
   // Zabezpieczenie
   if(langsys_GetSpec() == NULL)
      return;
   // Ustawiam adres w module na pocz¹tek definicji znaków:
   lcd_SendInstr(LCDC_CGA);
   // Przesy³anie bloków - tylko do czasu a¿ mam 0xff w tablicy - koniec danych
   uint8_t a;
   for(a=0; a<ELEMS(lcd_spec); a++)
   {
      // 0xff oznacza koniec danych
      if(lcd_spec[a] == 0xff) break;
      // WskaŸnik na pocz¹tek danych wygl¹du znaku
      uint8_t* pdata = langsys_GetSpec()[lcd_spec[a]].matrix;
      // Wysy³am kolejny indeks
      uint8_t bait;
      for(bait=0; bait<8; bait++)
      {
         lcd_SendData(pgm_read_byte(pdata++));
      }
   }
}

// Funckcja odœwierzaj¹ca dane w pamiêci DDRAM wyœwietlacza
// Aby wyœwietlanie przebieg³o prawid³owo pamiêæ CGRAM musi byæ ju¿ ustawiona
void lcd_UpdateText(void)
{
   uint8_t x, y;

   char *pbuffer = lcd_buffer;
   // Przetwarzanie ka¿dej linii oddzielnie
   for(y=0; y<LCD_SY; y++)
   {
      lcd_SendInstr(LCDC_DDA | lcd_LineStart(y));
      // Wys³anie wszystkich znaków z danej linii
      for(x=0; x<LCD_SX; x++)
      {
         uint8_t znak = *pbuffer++;
         if(lcd_IsSpec(znak))
            znak = lcd_GetSpec(lcd_Spec2Index(znak));
         // Znak gotowy do wys³ania
         lcd_SendData(znak);
      }
   }
}

// Odœwierzenie wszystkich danych wyœwietlacza
void lcd_Update(void)
{
   lcd_PrepareSpec();
   lcd_UpdateCGRAM();
   // Wy³¹czenie kursora na czas odœwierzania
   lcd_SendInstr(LCDC_ON | (lcd_status & LCD_STATUS_DISP));
   lcd_UpdateText();
   lcd_UpdateCurPos();
   // Ponowne w³¹czenie kursora (jeœli by³ w³¹czony)
   lcd_UpdateStatus();
}

//__________________________________________________________________________________________
// Funkcja podaj¹ca uchwyt do strumienia
inline FILE* lcd_GetFile(void)
{
   return lcd_fLCD;
}
