#include "DataloggerManager.hpp"
#include "row.hpp"

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

    currentRowLock.lock();
    int lat = currentRow.lat;
    int lon = currentRow.lon;
    int elev = currentRow.elev;
    int gps_millis = currentRow.gps_millis;
    currentRowLock.unlock();

    Serial.print("lat: ");
    Serial.print(lat);
    Serial.print(", lon: ");
    Serial.print(lon);
    Serial.print(", elev: ");
    Serial.print(elev);
    Serial.print(", gps_millis: ");
    Serial.println(gps_millis);
}


DataloggerManager::DataloggerManager(Row& row, Threads::Mutex& lock) : currentRow(row), currentRowLock(lock) {
}
