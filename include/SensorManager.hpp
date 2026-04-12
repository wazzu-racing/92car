#pragma once

#include "ThreadedManager.hpp"
#include "constants.hpp"
#include <Wire.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include "BMI088.h"
#include "Row.hpp"
#include <TeensyThreads.h>
#include <FlexCAN_T4.h>
#include <MegaCAN.h>
#include "MegaSquirt3.hpp"


class SensorManager: public ThreadedManager {
public:
    SensorManager(Row& row, Threads::Mutex& lock):
        row(row),
        rowLock(lock), 
        imu(TEENSY_IMU_WIRE, TEENSY_ACEL_REG, TEENSY_GYRO_REG) { }

    void loop();
    void setup();
    static void handleManager(const CAN_message_t &msg);
    void handle(const CAN_message_t &msg);
private:
    static SensorManager* instance;
    SFE_UBLOX_GNSS gps;

    FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can;
    MegaSquirt3 ecu;

public:

    Threads::Mutex rowLock;
    Row row;
    Bmi088 imu;
};
