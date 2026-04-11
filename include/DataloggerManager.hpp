#pragma once

#include "ThreadedManager.hpp"

#include <SdFat_Adafruit_Fork.h>

class DataloggerManager: public ThreadedManager {
    void loop();
    void setup();
    SdFat SD;
};
