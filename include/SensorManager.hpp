#pragma once

#include "ThreadedManager.hpp"
#include "constants.hpp"
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>

class SensorManager: public ThreadedManager {
    void loop();
    void setup();
private:
    SFE_UBLOX_GNSS myGNSS;
};