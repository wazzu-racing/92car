#include "DashboardManager.hpp"

void DashboardManager::setup() {
    Serial.println("Setup DashboardManager");
}

void DashboardManager::loop() {
    threads.delay(1000);
    Serial.println("Dashboard...");
}