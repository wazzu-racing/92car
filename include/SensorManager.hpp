#pragma once

#include "ThreadedManager.hpp"

class SensorManager: public ThreadedManager {
    void loop();
    void setup();
};