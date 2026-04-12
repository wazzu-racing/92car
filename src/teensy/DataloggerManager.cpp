#include "DataloggerManager.hpp"
#include "Row.hpp"

void DataloggerManager::setup() {
    this->log("Setup DataloggerManager");

    // Power-up delay
    threads.delay(200);

    // Standard high-speed SDIO initialization
    if(!SD.begin(SdioConfig(FIFO_SDIO))){
        this->log("SD card failed or not present!");
        return;
    }

    // Use O_RDWR | O_CREAT | O_APPEND (or similar) to ensure the file is created if it doesn't exist
    // FILE_WRITE in SdFat 2.x often requires the file to already exist if not using specific flags
    if (!file.open("test.bin", O_RDWR | O_CREAT)) {
        this->log("Failed to open 'test.bin' for writing!");
        return;
    }

    initialized = true;
    this->log("Datalogger initialized. Logging to 'test.bin'");
}

void DataloggerManager::loop() {
    threads.delay(100);

    if (!initialized) {
        return;
    }

    rowLock.lock();
    Row localRow = row;
    rowLock.unlock();

    this->log(
        "RPM: " + std::to_string(localRow.rpm) +
        " | Lat: " + std::to_string(localRow.lat)
    );

    // Update timestamp and write binary data
    rowLock.lock();
    row.write_millis = millis();
    size_t written = file.write((const uint8_t *)&row, sizeof(Row));

    // Periodically sync file
    static uint32_t lastSync = 0;
    if (millis() - lastSync > 1000) {
        file.sync();
        lastSync = millis();
    }
    rowLock.unlock();

    if (written != sizeof(Row)) {
        this->log("Write failed! bytes: " + std::to_string(written));
    }
}
