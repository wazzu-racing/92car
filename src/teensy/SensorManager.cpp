#include "SensorManager.hpp"

SensorManager::SensorManager() : accel(TEENSY_IMU_WIRE, 0x18), gyro(TEENSY_IMU_WIRE, 0x68) {}

void SensorManager::setup() {
    log("Setup SensorManager");
    TEENSY_GPS_WIRE.begin();
    TEENSY_IMU_WIRE.begin();
    if (myGNSS.begin(TEENSY_GPS_WIRE) == false) { //Connect to the u-blox module using Wire port {
        log("error with GPS... cannot connect...");
        while (1);
    }
    log("gps connected...");
        myGNSS.checkUblox();

    if(accel.begin() < 0 || gyro.begin() < 0) {
        log("error with IMU... cannot connect...");
    }
}

void SensorManager::loop() {
    if (myGNSS.getPVT(10)) {
        Serial.print("unixtime\t"); Serial.println(myGNSS.getUnixEpoch());
        Serial.print("lat\t"); Serial.println(myGNSS.getLatitude());
        Serial.print("lon\t"); Serial.println(myGNSS.getLongitude());
        Serial.print("elev\t"); Serial.println(myGNSS.getAltitude());
        Serial.print("ground_speed\t"); Serial.println(myGNSS.getGroundSpeed());
        Serial.print("gps_millis\t"); Serial.println(millis());
        Serial.println("\n\n\n");
    }


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
}
