#pragma once

#include "ThreadedManager.hpp"

#include <SD.h>

class DataloggerManager: public ThreadedManager {
    void loop();
    void setup();
};