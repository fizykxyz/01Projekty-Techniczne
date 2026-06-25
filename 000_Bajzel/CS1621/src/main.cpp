#include <Arduino.h>
#include <HT1621Display.h>
#include "utils.h"
#include "7segment.h"



#define PIN_CS 13
#define PIN_WR 12
#define PIN_DATA 8
uint8_t rotation7LED=upside;
typedef HT1621::Driver<PIN_CS, PIN_WR, PIN_DATA> DisplayDriver;
typedef LCD::Display<DisplayDriver> Display;

DisplayDriver htDriver;
Display htDisplay;

char places[][10] = {
  "DOM 23*C",
  "PIEC 57*C",
  "DWOREK -2",
  "SALON 19*",
  "HOL 17*C",
  "SYPIALNIA",
  "SRACZ 13*",
  "PIWNICA 7",
  "KIBEL 12*",
  "JADZIEM*",
  "KORYTARZ",
  "KLATKA 8*",
  };



void setChar(uint8_t pos, uint8_t ascCode)
{

  uint8_t databyte=reorder(glyph7[ascCode-32],pivotSeg[rotation7LED]);
  htDriver.sendData(pos*2,databyte & 0x0F);
  htDriver.sendData(pos*2+1,(databyte >> 4) & 0x0F);
}
void setString(uint8_t pos, char *text)
{
  uint8_t i=0;
  while (*text)
  {
    setChar((pos+i)%12,*text++);
    i++;
  }
}


void setup()
{
    htDisplay.begin();
    htDisplay.configure();
   htDisplay.clearBuffer();
    //htDisplay.setDigit(11, 1);
    htDriver.sendData(23,15);
    htDisplay.setDigit(3, 1);
    for (uint8_t i=0;i<12;i++)
    {
htDisplay.setDigit(i,i%10);
    }
    
    htDisplay.sync();
    delay(2000);
}

void loop()
{
  for (uint8_t i=0;i<sizeof(places)/sizeof(places[0]);i++)
  {
  htDisplay.clearBuffer();
 setChar(2,'0'+i%10);
 setChar(1,'0'+i/10);
setString(3,places[i]);
htDisplay.sync();
    delay(3000);
    }

    
    
}
