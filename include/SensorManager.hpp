#pragma once

#include "ThreadedManager.hpp"
#include "constants.hpp"
#include <Wire.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include "BMI088.h"
#include "row.hpp"
#include <TeensyThreads.h>


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

    Threads::Mutex currentRowLock;
    Row currentRow;
};
