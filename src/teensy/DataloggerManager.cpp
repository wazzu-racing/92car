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
    int gps_millis = currentRow.gps_millis;
    // int lon = currentRow.lon;
    // int elev = currentRow.elev;
    float ax = (float)currentRow.ax / (float)1000;
    int imu_millis = currentRow.imu_millis;

    int rpm = currentRow.rpm;
    int ecu_millis = currentRow.ecu_millis;

    currentRowLock.unlock();

    Serial.print("lat: ");
    Serial.print(lat);
    Serial.print(", ax: ");
    Serial.print(ax);
    // Serial.print(", elev: ");
    // Serial.print(elev);
    Serial.print(", gps_millis: ");
    Serial.print(gps_millis);
    Serial.print(", imu_millis: ");
    Serial.print(imu_millis);
    Serial.print(", rpm: ");
    Serial.print(rpm);
    Serial.print(", ecu: ");
    Serial.println(ecu_millis);
}


DataloggerManager::DataloggerManager(Row& row, Threads::Mutex& lock) : currentRow(row), currentRowLock(lock) {
}
