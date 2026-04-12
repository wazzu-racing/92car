#include "SensorManager.hpp"

SensorManager* SensorManager::instance = nullptr;
volatile bool gyro_updated = false;
volatile bool accel_updated = false;
volatile bool gps_updated = false;

void gyro_ISR() {gyro_updated = true;}
void accel_ISR() {accel_updated = true;}
void gps_ISR() {gps_updated = true;}

void SensorManager::setup() {
    instance = this;
    log("Setup SensorManager");


    TEENSY_GPS_WIRE.setSDA(TEENSY_PIN_GPS_SDA);
    TEENSY_GPS_WIRE.setSCL(TEENSY_PIN_GPS_SCL);
    TEENSY_GPS_WIRE.begin();

    TEENSY_IMU_WIRE.setSDA(TEENSY_PIN_IMU_SDA);
    TEENSY_IMU_WIRE.setSCL(TEENSY_PIN_IMU_SCL);
    TEENSY_IMU_WIRE.begin();

    if (gps.begin(TEENSY_GPS_WIRE) == false) { //Connect to the u-blox module using Wire port {
        log("error with GPS... cannot connect...");
    } else {
        log("gps connected...");
        gps.checkUblox();
    }

    if(imu.begin() < 0) {
        log("error with IMU... cannot connect...");
    } else {
        log("imu connected...");
    }

    // Set IMU interrupts
    pinMode(TEENSY_PIN_ACEL_INTERRUPT, INPUT_PULLUP);
    pinMode(TEENSY_PIN_GYRO_INTERRUPT, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(TEENSY_PIN_ACEL_INTERRUPT), accel_ISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(TEENSY_PIN_GYRO_INTERRUPT), gyro_ISR, FALLING);

    // Set GPS interrupt
    pinMode(TEENSY_PIN_GPS_INTERRUPT, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(TEENSY_PIN_GPS_INTERRUPT), gps_ISR, FALLING);


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
    // accelerometer of IMU =======================================================================
    if (accel_updated) {
        accel_updated = false;

        imu.readSensor();

        rowLock.lock();
        row.ax           = imu.getAccelX_mss() * 1000;
        row.ay           = imu.getAccelY_mss() * 1000;
        row.az           = imu.getAccelZ_mss() * 1000;
        row.accel_millis = millis();
        rowLock.unlock();
    }


    // gyroscope of IMU ===========================================================================
    if (gyro_updated) {
        gyro_updated = false;

        imu.readSensor();

        rowLock.lock();
            row.imu_x      = imu.getGyroX_rads() * 1000;
            row.imu_y      = imu.getGyroY_rads() * 1000;
            row.imu_z      = imu.getGyroZ_rads() * 1000;
            row.imu_millis = millis();
            rowLock.unlock();
    }

    if (gps_updated){

    }


    threads.delay(10);
}

void SensorManager::handleManager(const CAN_message_t &msg){
    if(instance){
        instance->handle(msg);
    }
}

void SensorManager::handle(const CAN_message_t &msg){
    if(ecu.decode(msg)){
        rowLock.lock();
        row.rpm = ecu.data.rpm;
        row.time = ecu.data.seconds;
        row.afr = ecu.data.AFR1 * 1000;
        row.fuelload = ecu.data.fuelload * 1000;
        row.spark_advance = ecu.data.adv_deg * 1000;
        row.baro = ecu.data.baro * 1000;
        row.map = ecu.data.map * 1000;
        row.mat = ecu.data.mat * 1000;
        row.clnt_temp = ecu.data.clt * 1000;
        row.tps = ecu.data.tps * 1000;
        row.batt = ecu.data.batt * 1000;
        row.oil_press = ecu.data.sensors1 * 1000;
        row.syncloss_count = ecu.data.synccnt;
        row.syncloss_code = ecu.data.syncreason;
        row.ltcl_timing = ecu.data.launch_timing * 1000;
        row.ve1 = ecu.data.ve1 * 1000;
        row.ve2 = ecu.data.ve2 * 1000;
        row.egt = ecu.data.egt1 * 1000;
        row.maf = ecu.data.MAF * 1000;
        row.in_temp = ecu.data.airtemp * 1000;
        row.ecu_millis = millis();
        rowLock.unlock();
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
