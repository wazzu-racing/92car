#include "DataloggerManager.hpp"

void DataloggerManager::setup() {
    Serial.println("Setup DataloggerManager");
}

void DataloggerManager::loop() {
    threads.delay(100);
    Serial.println("Datalogger...");
}