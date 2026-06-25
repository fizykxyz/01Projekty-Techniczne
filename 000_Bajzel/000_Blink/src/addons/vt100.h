#pragma once

// --- RESET ---
#define VT_RESET           "\x1B[0m"

// --- STYLE ---
#define VT_BOLD            "\x1B[1m"
#define VT_DIM             "\x1B[2m"
#define VT_ITALIC          "\x1B[3m"
#define VT_UNDERLINE       "\x1B[4m"
#define VT_BLINK           "\x1B[5m"
#define VT_REVERSE         "\x1B[7m"
#define VT_HIDDEN          "\x1B[8m"

// --- KOLORY TEKSTU (Standard) ---
#define F_BLACK            "\x1B[30m"
#define F_RED              "\x1B[31m"
#define F_GREEN            "\x1B[32m"
#define F_YELLOW           "\x1B[33m"
#define F_BLUE             "\x1B[34m"
#define F_MAGENTA          "\x1B[35m"
#define F_CYAN             "\x1B[36m"
#define F_WHITE            "\x1B[37m"

// --- KOLORY TEKSTU (Jasne/Intensywne) ---
#define F_B_BLACK          "\x1B[90m"
#define F_B_RED            "\x1B[91m"
#define F_B_GREEN          "\x1B[92m"
#define F_B_YELLOW         "\x1B[93m"
#define F_B_BLUE           "\x1B[94m"
#define F_B_MAGENTA        "\x1B[95m"
#define F_B_CYAN           "\x1B[96m"
#define F_B_WHITE          "\x1B[97m"

// --- KOLORY TŁA ---
#define B_BLACK            "\x1B[40m"
#define B_RED              "\x1B[41m"
#define B_GREEN            "\x1B[42m"
#define B_YELLOW           "\x1B[43m"
#define B_BLUE             "\x1B[44m"
#define B_MAGENTA          "\x1B[45m"
#define B_CYAN             "\x1B[46m"
#define B_WHITE            "\x1B[47m"

// --- STEROWANIE KURSORAMI I EKRANEM ---
#define VT_CLR_SCREEN      "\x1B[2J"
#define VT_CLR_TO_EOS      "\x1B[J"   // Czyść od kursora do końca ekranu
#define VT_CLR_LINE        "\x1B[2K"  // Czyść całą linię
#define VT_CLR_TO_EOL      "\x1B[K"   // Czyść od kursora do końca linii
#define VT_CURSOR_HOME     "\x1B[H"
#define VT_SAVE_CURSOR     "\x1B[s"
#define VT_RESTORE_CURSOR  "\x1B[u"
#define VT_LINE_UP         "\x1B[A"
#define VT_LINE_DOWN       "\x1B[B"
#define VT_CHAR_RIGHT      "\x1B[C"
#define VT_CHAR_LEFT       "\x1B[D"

// Makro do ustawiania kursora w konkretnym miejscu (Y, X)
#define VT_GOTO(y,x)       Serial.print("\x1B["); Serial.print(y); Serial.print(";"); Serial.print(x); Serial.print("H")
