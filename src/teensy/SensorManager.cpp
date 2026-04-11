#include "SensorManager.hpp"

SensorManager* SensorManager::instance = nullptr;

SensorManager::SensorManager() : imu(TEENSY_IMU_WIRE, 0x18, 0x68) {
    instance = this;
}

void SensorManager::setup() {
    log("Setup SensorManager");

    currentRowLock.lock();
    currentRow = Row();
    currentRowLock.unlock();


    TEENSY_GPS_WIRE.setSDA(TEENSY_PIN_GPS_SDA);
    TEENSY_GPS_WIRE.setSCL(TEENSY_PIN_GPS_SCL);
    TEENSY_GPS_WIRE.begin();

    TEENSY_IMU_WIRE.setSDA(TEENSY_PIN_IMU_SDA);
    TEENSY_IMU_WIRE.setSCL(TEENSY_PIN_IMU_SCL);
    TEENSY_IMU_WIRE.begin();

    if (myGNSS.begin(TEENSY_GPS_WIRE) == false) { //Connect to the u-blox module using Wire port {
        log("error with GPS... cannot connect...");
    } else {
        log("gps connected...");
        myGNSS.checkUblox();
    }

    if(imu.begin() < 0) {
        log("error with IMU... cannot connect...");
    } else {
        log("imu connected...");
    }


    // CAN STUFF
    Can.begin();
    Can.setBaudRate(
        500000); // set to 500000 for normal Megasquirt usage - need to change
                 // Megasquirt firmware to change MS CAN baud rate
    Can.setMaxMB(16); // sets maximum number of mailboxes for FlexCAN_T4 usage
    Can.enableFIFO();
    Can.enableFIFOInterrupt();
    Can.onReceive(handleManager); // when a CAN message is received, runs the
}

void SensorManager::loop() {
    if (myGNSS.getPVT(10)) {
        currentRowLock.lock();
        currentRow.lat = myGNSS.getLatitude();
        currentRow.lon = myGNSS.getLongitude();
        currentRow.elev = myGNSS.getAltitude();
        currentRow.ground_speed = myGNSS.getGroundSpeed();
        currentRow.gps_millis = millis();
        currentRowLock.unlock();
    }

    imu.readSensor();

    currentRowLock.lock();
    currentRow.ax = imu.getAccelX_mss() * 1000;
    currentRow.ay = imu.getAccelY_mss() * 1000;
    currentRow.az = imu.getAccelZ_mss() * 1000;
    currentRow.accel_millis = millis();

    currentRow.imu_x = imu.getGyroX_rads() * 1000;
    currentRow.imu_y = imu.getGyroY_rads() * 1000;
    currentRow.imu_z = imu.getGyroZ_rads() * 1000;
    currentRow.imu_millis = millis();
    currentRowLock.unlock();

    threads.delay(10);
}

void SensorManager::handleManager(const CAN_message_t &msg){
    if(instance){
        instance->handle(msg);
    }
}

void SensorManager::handle(const CAN_message_t &msg){
    if(ecu.decode(msg)){
        currentRowLock.lock();
        currentRow.rpm = ecu.data.rpm;
        currentRow.time = ecu.data.seconds;
        currentRow.afr = ecu.data.AFR1 * 1000;
        currentRow.fuelload = ecu.data.fuelload * 1000;
        currentRow.spark_advance = ecu.data.adv_deg * 1000;
        currentRow.baro = ecu.data.baro * 1000;
        currentRow.map = ecu.data.map * 1000;
        currentRow.mat = ecu.data.mat * 1000;
        currentRow.clnt_temp = ecu.data.clt * 1000;
        currentRow.tps = ecu.data.tps * 1000;
        currentRow.batt = ecu.data.batt * 1000;
        currentRow.oil_press = ecu.data.sensors1 * 1000;
        currentRow.syncloss_count = ecu.data.synccnt;
        currentRow.syncloss_code = ecu.data.syncreason;
        currentRow.ltcl_timing = ecu.data.launch_timing * 1000;
        currentRow.ve1 = ecu.data.ve1 * 1000;
        currentRow.ve2 = ecu.data.ve2 * 1000;
        currentRow.egt = ecu.data.egt1 * 1000;
        currentRow.maf = ecu.data.MAF * 1000;
        currentRow.in_temp = ecu.data.airtemp * 1000;
        currentRow.ecu_millis = millis();
        currentRowLock.unlock();
    }
}

/*
    // Sensors:

    // FROM THE IMU:
    // - ax * 1000
    // - ay * 1000
    // - az * 1000
    // - pitch * 1000
    // - yaw * 1000
    // - roll * 1000
    // - imu_millis

    // FROM THE ECU:
    // - rpm
    // - time (seconds)
    // - afr * 1000
    // - fuelload * 1000
    // - spark_advance * 1000
    // - baro * 1000
    // - map * 1000
    // - mat * 1000
    // - clnt_temp * 1000
    // - tps * 1000
    // - batt * 1000
    // - oil_press * 1000
    // - syncloss_count
    // - syncloss_code
    // - itcl_timing * 1000
    // - ve1 * 1000
    // - ve2 * 1000
    // - egt * 1000
    // - maf * 1000
    // - in_temp * 1000
    // - ecu_millis

    // OTHER SENSORS

    // CAN ID 1
    // - brake1
    // - brake2
    // - susp_pot_1
    // - susp_pot_2
    // - analogx1_millis

    // CAN ID 2
    // - susp_pot_4
    // - rad_in
    // - rad_out
    // - susp_pot_3

    // CAN ID 3
    // - analogx3_millis
*/
