#pragma once

#include "ThreadedManager.hpp"
#include "constants.hpp"
#include <Wire.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include "BMI088.h"

class SensorManager: public ThreadedManager {
public:
    SensorManager();
    void loop();
    void setup();
private:
    SFE_UBLOX_GNSS myGNSS;
    Bmi088Accel accel;
    Bmi088Gyro gyro;
public:
    // Variables that the datalogger module will read. It is safe to read them
    // without a mutex because the read is atomic (they're ints)

    int32_t unixtime;
    int32_t lat;
    int32_t lon;
    int32_t alt;

};
