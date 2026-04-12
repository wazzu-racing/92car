#pragma once

#include "ThreadedManager.hpp"
#include "Row.hpp"
#include <TeensyThreads.h>
#include <FS.h>

#include <SdFat_Adafruit_Fork.h>

class DataloggerManager: public ThreadedManager {
public:
    DataloggerManager(Row& row, Threads::Mutex& lock): row(row), rowLock(lock) {}
    void loop();
    void setup();
private:
    SdFat SD;
    bool initialized = false;

    Row& row;
    Threads::Mutex& rowLock;
    File32 file;
};
