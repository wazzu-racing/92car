#include "DataloggerManager.hpp"
#include "Row.hpp"
#include <time.h>

void DataloggerManager::setup() {
    log("Setup DataloggerManager");

    if(!SD.begin(SdioConfig(FIFO_SDIO))){
        log("SD card failed or not present!");
        return;
    }

    Serial8.begin(9600);
    // year-month-day-hour:minute:second

    int i = 0;
    while (SD.exists((String(i) + ".wr").c_str())) {
      i++;
    }
    snprintf(currentFileName, sizeof(currentFileName), "%d.wr", i);
    file = SD.open(currentFileName, FILE_WRITE);
}

void DataloggerManager::loop() {
    int dt = millis() - last_sd;
    threads.delay((1000 / DATALOG_MAX_HZ) - dt); // wait remaining time

    if (rowBuffer_i == DATALOG_SD_BUFFER) {
        for (int i=0; i<DATALOG_SD_BUFFER; i++) {
            rowBuffer[i].write_millis = millis();
            file.write((byte*) &rowBuffer[i], sizeof(Row));
        }
        file.flush();
        rowBuffer_i = 0;
    }

    rowLock.lock();
    memcpy(&rowBuffer[rowBuffer_i], &row, sizeof(Row));
    rowLock.unlock();

    if (millis() - last_radio >= (1000 / RADIO_MAX_HZ)) {
        rowBuffer[rowBuffer_i].write_millis = millis();
        Serial8.write((byte*) &rowBuffer[rowBuffer_i], sizeof(Row));
        Serial8.print("\n\n\n");
        last_radio = millis();
    }

    if (!hasBeenRenamed) {
        if (rowBuffer[rowBuffer_i].unixtime >= JAN_1_2025) {
            struct tm *t = gmtime((time_t*)&rowBuffer[rowBuffer_i].unixtime);
            char buf[32];
            strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S.wr", t);
            
            file.close();
            if (SD.rename(currentFileName, buf)) {
                log("Renamed log file to " + std::string(buf));
                hasBeenRenamed = true;
                // Reopen in append mode
                file = SD.open(buf, FILE_WRITE);
            } else {
                log("Failed to rename log file!");
                // Reopen original so we don't lose data
                file = SD.open(currentFileName, FILE_WRITE);
            }
        }
    }

    rowBuffer_i++;

    last_sd = millis();
}
