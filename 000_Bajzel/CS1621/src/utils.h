#pragma once

#define BIT_HIGH(REG,BIT) (REG)|=(1<<(BIT))
#define BIT_LOW(REG,BIT) (REG)&=~(1<<(BIT))
#define BIT_TOOGLE(REG,BIT) (REG)^=(1<<(BIT))
#define BIT_READ(REG,BIT) !!((REG)&(1<<(BIT)))

//#define _nop_() __asm nop __endasm
#define _nop_() 
//#define _dataBIG __code
#define _dataBIG 

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

static unsigned long int next = 1;

int rand(void)
{
    next = next * 1103515245UL + 12345;
    return (unsigned int)(next/65536) % (65536 + 1U);
}


uint8_t DEC2BCD(uint8_t val)
{
	return ((val/10*16)+(val%10));
	//return (((dec / 10) << 4) & 0xF0) | ((dec % 10) & 0x0F);
}

uint8_t BCD2DEC(uint8_t val)
{
	return ((val/16*10)+(val%16));
	// return (((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F));
}


void delay_ms(uint16_t millisec)
{
	uint16_t i,j;
	for(j=0;j<millisec;j++){
	 for(i=0;i<=84;i++); // f=110592 hz - 84 1ms
	 }
}

void delay_ms1(uint16_t millisec)
{
uint16_t m;
for(millisec;millisec>0;millisec--)
{
  for(m=121;m>0;m--);
    _nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}
}

void _delay_ms(uint16_t millisec)
{
	uint16_t i,j;
	for(j=0;j<millisec;j++){
	 for(i=0;i<=84;i++); // f=110592 hz - 84 1ms
	 }
}

void _delay_us(uint16_t millisec)
{
uint16_t m;
for(millisec;millisec>0;millisec--)
{
  for(m=1;m>0;m--);
    _nop_();
	_nop_();
	_nop_();
	
}
}


