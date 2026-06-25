/****************************************************************************
* Biblioteka obslugi wyswietlacza.
* 
* Plik konfiguracyjny platformy.
*
* Nazwa pliku: platf_conf.h
* Data: 13.07.2007
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

#ifndef __PLATF_CONF_H__
#define __PLATF_CONF_H__ 1

#define AVR 1
#define LPC21 2
#define STR91 3

#if DISP_PLATFORM == AVR
#  include <AVR_platf_conf.h>
#  include <AVR_platf_spec.h>
#elif DISP_PLATFORM == LPC21
#  include <LPC21_platf_conf.h>
#  include <LPC21_platf_spec.h>
#elif DISP_PLATFORM == STR91
#  include <STR91_platf_conf.h>
#  include <STR91_platf_spec.h>
#else
#  error Nieznana platforma (zle zadeklarowano DISP_PLATFORM).
#endif

#endif // ! __PLATF_CONF_H__
