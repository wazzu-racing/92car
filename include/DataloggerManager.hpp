#pragma once

#include "ThreadedManager.hpp"
#include "Row.hpp"
#include "constants.hpp"
#include <TeensyThreads.h>

#include <SdFat_Adafruit_Fork.h>

class DataloggerManager: public ThreadedManager {
public:
    DataloggerManager(Row& row, Threads::Mutex& lock) : row(row), rowLock(lock) {
        // ...
    }
    void loop();
    void setup();
private:
    SdFat SD;
    File32 file;
    bool hasBeenRenamed = false;

    Row rowBuffer[DATALOG_SD_BUFFER];
    int rowBuffer_i = 0;
    int last_sd = 0;
    int last_radio = 0;

    Row& row;
    Threads::Mutex& rowLock;
};
