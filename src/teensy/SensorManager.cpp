#include "SensorManager.hpp"

volatile bool gyro_updated = false;
volatile bool accel_updated = false;
volatile bool gps_updated = false;

void gyro_ISR() {gyro_updated = true;}
void accel_ISR() {accel_updated = true;}
void gps_ISR() {gps_updated = true;}

void SensorManager::setup() {
    log("Setup SensorManager");

    TEENSY_GPS_WIRE.begin();
    TEENSY_IMU_WIRE.begin();

    if (gps.begin(TEENSY_GPS_WIRE) == false) { //Connect to the u-blox module using Wire port {
        log("error with GPS... cannot connect...");
        while (1);
    }
    log("gps connected...");
    gps.checkUblox();

    if(accel.begin() < 0 || gyro.begin() < 0) {
        log("error with IMU... cannot connect...");
    }

    // set IMU interrupts
    pinMode(TEENSY_PIN_ACEL_INTERRUPT, INPUT_PULLUP);
    pinMode(TEENSY_PIN_GYRO_INTERRUPT, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(TEENSY_PIN_ACEL_INTERRUPT), accel_ISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(TEENSY_PIN_GYRO_INTERRUPT), gyro_ISR, FALLING);

    // set GPS interrupt
    pinMode(TEENSY_PIN_GPS_INTERRUPT, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(TEENSY_PIN_GPS_INTERRUPT), gps_ISR, FALLING);
}

void SensorManager::loop() {
    // accelerometer of IMU =======================================================================
    if (accel_updated) {
        accel_updated = false;

        accel.readSensor();
        
        rowLock.lock();
        row.ax           = accel.getAccelX_mss() * 1000;
        row.ay           = accel.getAccelY_mss() * 1000;
        row.az           = accel.getAccelZ_mss() * 1000;
        row.accel_millis = millis();
        rowLock.unlock();
    }

    // gyroscope of IMU ===========================================================================
    if (gyro_updated) {
        gyro_updated = false;

        gyro.readSensor();

        rowLock.lock();
        row.imu_x      = gyro.getGyroX_rads() * 1000;
        row.imu_y      = gyro.getGyroY_rads() * 1000;
        row.imu_z      = gyro.getGyroZ_rads() * 1000;
        row.imu_millis = millis();
        rowLock.unlock();
    }

    // GPS ========================================================================================
    if (gps_updated) {

    }

    // if (gps.getPVT(10)) {
    //     currentRowLock.lock();
    //     currentRow.lat = gps.getLatitude();
    //     currentRow.lon = gps.getLongitude();
    //     currentRow.elev = gps.getAltitude();
    //     currentRow.ground_speed = gps.getGroundSpeed();
    //     currentRow.gps_millis = millis();
    //     currentRowLock.unlock();

    //     // Serial.print("unixtime\t"); Serial.println(myGNSS.getUnixEpoch());
    //     // Serial.print("lat\t"); Serial.println(myGNSS.getLatitude());
    //     // Serial.print("lon\t"); Serial.println(myGNSS.getLongitude());
    //     // Serial.print("elev\t"); Serial.println(myGNSS.getAltitude());
    //     // Serial.print("ground_speed\t"); Serial.println(myGNSS.getGroundSpeed());
    //     // Serial.print("gps_millis\t"); Serial.println(millis());
    //     // Serial.println("\n\n\n");
    // }


    // // Sensors:

    // // FROM THE IMU:
    // // - ax * 1000
    // // - ay * 1000
    // // - az * 1000
    // // - pitch * 1000
    // // - yaw * 1000
    // // - roll * 1000
    // // - imu_millis

    // // FROM THE ECU:
    // // - rpm
    // // - time (seconds)
    // // - afr * 1000
    // // - fuelload * 1000
    // // - spark_advance * 1000
    // // - baro * 1000
    // // - map * 1000
    // // - mat * 1000
    // // - clnt_temp * 1000
    // // - tps * 1000
    // // - batt * 1000
    // // - oil_press * 1000
    // // - syncloss_count
    // // - syncloss_code
    // // - itcl_timing * 1000
    // // - ve1 * 1000
    // // - ve2 * 1000
    // // - egt * 1000
    // // - maf * 1000
    // // - in_temp * 1000
    // // - ecu_millis

    // // OTHER SENSORS

    // // CAN ID 1
    // // - brake1
    // // - brake2
    // // - susp_pot_1
    // // - susp_pot_2
    // // - analogx1_millis

    // // CAN ID 2
    // // - susp_pot_4
    // // - rad_in
    // // - rad_out
    // // - susp_pot_3

    // // CAN ID 3
    // // - analogx3_millis
}
