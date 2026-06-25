/*
 * ATMEGA32L_LCD serwo.h
 *
 *  Stworzono: 2014 16-10-2014
 *      Autor: D¹browski Tomasz
 *     Edycja: 13:41:25
 */

#ifndef SERWO_H_
#define SERWO_H_

#define SERWO1_MIN 500
#define SERWO1_MAX 2500
#define SERWO1_MID 1500
#define SERWO1_ANGLE 180
#define SERWO1_A2PWM (SERWO1_MAX-SERWO1_MIN)/SERWO1_ANGLE
#define SERWO1_ALFA(x) (x)*SERWO1_A2PWM+SERWO1_MIN

void SERWO1_INIT(void);
void SERWO1(uint8_t ALFA);

#endif /* SERWO_H_ */
