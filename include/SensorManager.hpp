#pragma once

#include "ThreadedManager.hpp"
#include "constants.hpp"
#include <Wire.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include "BMI088.h"
#include "row.hpp"
#include <TeensyThreads.h>
#include <FlexCAN_T4.h>
#include <MegaCAN.h>
#include "MegaSquirt3.hpp"


class SensorManager: public ThreadedManager {
public:
    SensorManager();
    void loop();
    void setup();
    static void handleManager(const CAN_message_t &msg);
    void handle(const CAN_message_t &msg);
private:
    static SensorManager* instance;
    SFE_UBLOX_GNSS myGNSS;
    Bmi088 imu;

    FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can;
    MegaSquirt3 ecu;
public:

    Threads::Mutex currentRowLock;
    Row currentRow;
};
