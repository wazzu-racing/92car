#include "SensorManager.hpp"

void SensorManager::setup() {
    Serial.println("Setup SensorManager");
}

void SensorManager::loop() {
    threads.delay(500);
    Serial.println("Sensor...");
}