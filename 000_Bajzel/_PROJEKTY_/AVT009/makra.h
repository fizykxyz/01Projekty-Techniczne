/*
 * AVT009 makra.h
 *
 *	    Autor: D¹browski Tomasz 
 *  Stworzono: 2020-04-21   
 *     Edycja: 09:47:33
 */

#ifndef MAKRA_H_INCLUDED
#define MAKRA_H_INCLUDED

#define PORT(x) XPORT(x)
#define XPORT(x) (PORT##x)

#define PIN(x) XPIN(x)
#define XPIN(x) (PIN##x)

#define DDR(x) XDDR(x)
#define XDDR(x) (DDR##x)

#define NOP() {asm volatile("nop"::);}

#define ELEMS(p) (sizeof(p)/sizeof(p[0]))

#endif /* MAKRA_H_INCLUDED */
