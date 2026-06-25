#include "../ONEWIRE/onewire.h"
#include "DS18B20.h"

void OW_Start_Conversion(uint8_t block)
{
 if(OW_WAITFORPRESENCEPULSE()==0) return;
 OW_WRITE(OW_SkipROM);
 OW_WRITE(OW_CONVERT);
 if(block) while(!OW_READBIT());
}

int16_t OW_GetTemperature(uint8_t *aID)
{
 OW_SELECTDEVICE(aID);
 if(Error!=OW_OK) return 0xFFFF;

 OW_WRITE(OW_READ_SCRATCHPAD);
 uint16_t temp=OW_READ();
 temp=temp+(OW_READ()<<8);
 if(temp & 0x8000) temp=1-(temp ^ 0xFFFF); //Konwersja kodów
 return temp;
}
