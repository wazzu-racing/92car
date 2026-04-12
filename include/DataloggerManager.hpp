#pragma once

#include "ThreadedManager.hpp"
#include "row.hpp"
#include <TeensyThreads.h>
#include <FS.h>

#include <SdFat_Adafruit_Fork.h>

class DataloggerManager: public ThreadedManager {
public:
    DataloggerManager(Row& row, Threads::Mutex& lock);
    void loop();
    void setup();
private:
    SdFat SD;
    bool initialized = false;

    Row& currentRow;
    Threads::Mutex& currentRowLock;
    File32 file;
};
