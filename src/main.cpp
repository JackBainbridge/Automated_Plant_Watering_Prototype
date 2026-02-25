#include <Arduino.h>

void setup() {
  // Initialize the onboard LED pin as an output
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Initialize Serial communication at 115200 bits per second
  Serial.begin(115200);
  Serial.println("--- System Initialized ---");
}

void loop() {
  // Turn the LED on
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Beat... (ON)");
  delay(1000); // Wait for 1 second

  // Turn the LED off
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Beat... (OFF)");
  delay(1000); // Wait for 1 second
}