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
    SensorManager(Row& row, Threads::Mutex& lock) :
        row(row), rowLock(lock), accel(TEENSY_IMU_WIRE, TEENSY_ACEL_REG), gyro(TEENSY_IMU_WIRE, TEENSY_GYRO_REG)  {
        // ...
    }
    void loop();
    void setup();

private:
    SFE_UBLOX_GNSS gps;
    Bmi088Accel accel;
    Bmi088Gyro gyro;

    Row& row;
    Threads::Mutex& rowLock;
};
