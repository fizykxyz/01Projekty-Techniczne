/**
	Biblioteka sprzêtowej obs³ugi interfejsu TWI w mikrokontrolerach atmel.
*/
#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>

#define ACK 1
#define NOACK 0

#define SLA_W(address)  ((address) << 1)
#define SLA_R(address)  (((address) << 1) + 0x01)

void I2C_BITRATE(uint16_t KHZ)
{
	uint8_t _div;
	_div = ((F_CPU/1000l)/KHZ);
	if(_div >= 16)
		_div = (_div-16)/2;
	TWBR = _div;
}



/**    Inicjalizacja TWI / Podobnie jak I2C_BITRATE
*/
static void I2C_INIT(void)
{
#if defined(TWPS0)
  TWSR = 0;
#endif
  TWBR = (F_CPU / 100000UL - 16)/2;
}



/**
    Procedura transmisji sygna³u START
*/
static void I2C_START(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}
/**
    Procedura transmisji sygna³u STOP
*/
static void I2C_STOP(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
    while ((TWCR & (1<<TWSTO)));
}


/**
    Procedura transmisji bajtu danych
*/
static void I2C_WRITE(uint8_t OCTET)
{
    TWDR = OCTET;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}
/**
    Procedura odczytu bajtu danych
*/
static uint8_t I2C_READ(uint8_t _ACK)
{
//    TWCR = ACK ? ((1 << TWINT) | (1 << TWEN) | (1 << TWEA)) : ((1 << TWINT) | (1 << TWEN)) ;
    TWCR =   ((1<<TWINT)  | (_ACK<<TWEA) | (1<<TWEN));
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}

void I2C_WRITEBUF(uint8_t DEV, uint8_t ADR, uint8_t *BUF, uint8_t LEN)
{
	I2C_START();
	I2C_WRITE(DEV);
	I2C_WRITE(ADR);
	while (LEN--) I2C_WRITE(*BUF++);
	I2C_STOP();
}



void I2C_READBUF(uint8_t DEV, uint8_t ADR, uint8_t *BUF, uint8_t LEN)
{
	I2C_START();
	I2C_WRITE(DEV);
	I2C_WRITE(ADR);
	I2C_START();
	I2C_WRITE(DEV + 1);
	while (LEN--) *BUF++ = I2C_READ( LEN ? ACK : NOACK );
	I2C_STOP();
}


#endif // I2C_H_
