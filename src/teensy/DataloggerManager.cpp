#include "DataloggerManager.hpp"

void DataloggerManager::setup() {
    Serial.println("Setup DataloggerManager");

    if(!SD.begin(SdioConfig(FIFO_SDIO))){
        Serial.println("SD card failed or not present!");
        return;
    }

    // year-month-day-hour:minute:second
}

void DataloggerManager::loop() {
    threads.delay(100);
    Serial.println("Datalogger...");
}
