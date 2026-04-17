#pragma once

#include "ThreadedManager.hpp"
#include "constants.hpp"
#include <Wire.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include "BMI088.h"
#include "Row.hpp"
#include <FlexCAN_T4.h>
#include <MegaCAN.h>
#include <TeensyThreads.h>


class SensorManager: public ThreadedManager {
public:
    SensorManager(Row& row, Threads::Mutex& lock) :
        row(row), rowLock(lock), accel(TEENSY_IMU_WIRE, TEENSY_ACEL_REG), gyro(TEENSY_IMU_WIRE, TEENSY_GYRO_REG),
        megaCAN(ECU_BASE_CAN_ID)  {
        // ...
    }
    void loop();
    void setup();

private:
    SFE_UBLOX_GNSS gps;
    Bmi088Accel accel;
    Bmi088Gyro gyro;

    FlexCAN_T4<TEENSY_CAN_SENSOR, RX_SIZE_256, TX_SIZE_16> can_sensor;
    FlexCAN_T4<TEENSY_CAN_ECU, RX_SIZE_256, TX_SIZE_16> can_ecu;
    MegaCAN megaCAN;

    Row& row;
    Threads::Mutex& rowLock;
};
