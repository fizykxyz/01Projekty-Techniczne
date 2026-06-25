#include <Arduino.h>

// Definicja pinu dla wbudowanej diody
// Dla Arduino Uno to zazwyczaj pin 13
// Dla ESP32 często jest to pin 2
#ifdef LED_BUILTIN
#define LED_PIN LED_BUILTIN
#else
#define LED_PIN 13 // Domyślnie dla ESP32
#endif

void setup() {
  // Inicjalizacja pinu jako wyjście
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH); // Włącz diodę
  delay(1000);                 // Czekaj 1 sekundę
  digitalWrite(LED_PIN, LOW);  // Wyłącz diodę
  delay(1000);                 // Czekaj 1 sekundę
}