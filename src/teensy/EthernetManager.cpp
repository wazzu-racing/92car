#include "EthernetManager.hpp"

void EthernetManager::setup() {
    Serial.println("Setup EthernetManager");
}

void EthernetManager::loop() {
    threads.delay(2000);
    Serial.println("Ethernet...");
}