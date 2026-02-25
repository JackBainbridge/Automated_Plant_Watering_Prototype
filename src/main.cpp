#include <Arduino.h>

// Constants
const int DRY_THRESHOLD = 300;        // Point where we turn the pump ON
const int WET_THRESHOLD = 700;        // Point where we turn the pump OFF
const unsigned long TICK_RATE = 1000; // Update every 1 second

// Global Variables
int moistureLevel = 500;              // Start at a healthy middle point
bool pumpActive = false;
unsigned long lastTick = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);       // LED will represent our PUMP
  Serial.begin(115200);
  Serial.println("--- Virtual Plant Watering System Online ---");
}

void loop() {
  unsigned long currentMillis = millis();

  // Run our simulation every 1 second
  if (currentMillis - lastTick >= TICK_RATE) {
    lastTick = currentMillis;

    // 1. Simulate Physics
    if (pumpActive) {
      moistureLevel += 50;         // Adding water
    } else {
      moistureLevel -= 10;         // Evaporation
    }

    // 2. Watering Logic (The "Brain")
    if (moistureLevel <= DRY_THRESHOLD) {
      pumpActive = true;
    } else if (moistureLevel >= WET_THRESHOLD) {
      pumpActive = false;
    }

    // 3. Output to Hardware & Serial
    digitalWrite(LED_BUILTIN, pumpActive ? HIGH : LOW);
    
    // Format for Serial Plotter (Value1, Value2)
    Serial.print("Moisture:");
    Serial.print(moistureLevel);
    Serial.print(",");
    Serial.print("Pump_Status:");
    Serial.println(pumpActive ? 1 : 0); // Offset status for easy viewing
  }
}