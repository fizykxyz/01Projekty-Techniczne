#pragma once
#include <Arduino.h>
#include "vt100.h"

// Poziomy logowania z kolorami
#define L_INFO    F_B_GREEN  "INFO"  VT_RESET
#define L_WARN    F_B_YELLOW "WARN"  VT_RESET
#define L_ERROR   F_B_RED    "ERROR" VT_RESET
#define L_DEBUG   F_B_CYAN   "DEBUG" VT_RESET

#if DEBUG == 1
    #define LOG_INIT(baud) { Serial.begin(baud); while(!Serial); }
    #define LOG(level, msg) { \
        Serial.print("["); Serial.print(millis()); Serial.print("] "); \
        Serial.print("["); Serial.print(level); Serial.print("] "); \
        Serial.println(msg); \
    }
    // Dodatkowe makro do logowania bez nowej linii
    #define LOG_RAW(msg) { Serial.print(msg); }
#else
    #define LOG_INIT(baud) 
    #define LOG(level, msg) 
    #define LOG_RAW(msg)
#endif
