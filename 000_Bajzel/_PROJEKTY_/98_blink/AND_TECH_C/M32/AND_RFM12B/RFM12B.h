/*
 * RFM12B.h
 *
 *  Created on: 10-06-2013
 *      Author: Piotr Rzeszut
 *  
 * Description: Funkcje do obs³ugi uk³adu RFM12B z poziomu procesora AVR
 */
#ifndef RFM12B_H
#define RFM12B_H

#include <avr/io.h>
#include <util/delay.h>
#include "RFM12B_reg.h"

void Rfm_spi_init(void);

uint16_t Rfm_xmit(uint16_t data);

void Rfm_ready_wait(void);

void Rfm_write_str(char* s, uint8_t address);

void Rfm_read_str(char* c, uint8_t max_size);

#endif //RFM12B_H
