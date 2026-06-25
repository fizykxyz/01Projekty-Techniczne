#include <Arduino.h>

#define DEBUG 0
#include "addons/debug.h"

// --- PARAMETRY KONFIGURACYJNE ---
#define SERIAL_BAUD_RATE    9600
#define PIN_STATUS_LED      LED_BUILTIN

// --- CZASY CYKLU PRACY ---
#define MS_BLINK_ACTIVE     1000
#define MS_BLINK_INACTIVE   1000
#define MS_BOOT_DELAY       500

void setup() {
    // Używamy makra z debug.h, ale parametry płyną z naszych define
    LOG_INIT(SERIAL_BAUD_RATE);
    
    // Krótka zwłoka na ustabilizowanie portu po stronie PC
    delay(MS_BOOT_DELAY);
    
    pinMode(PIN_STATUS_LED, OUTPUT);
    
    LOG(L_INFO, "System wystartowal. Wszystkie parametry sparametryzowane.");
    LOG(L_DEBUG, "Baudrate ustawiony na: " + String(SERIAL_BAUD_RATE));
}

void loop() {
    // Faza: WŁĄCZONY
    digitalWrite(PIN_STATUS_LED, HIGH);
    LOG(L_INFO, "LED STATUS: HIGH");
    delay(MS_BLINK_ACTIVE);

    // Faza: WYŁĄCZONY
    digitalWrite(PIN_STATUS_LED, LOW);
    LOG(L_INFO, "LED STATUS: LOW");
    LOG_RAW("Wyłączam LED, czekam na ponowne włączenie...\n");
    delay(MS_BLINK_INACTIVE);
}
