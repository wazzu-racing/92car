#include "SensorManager.hpp"

volatile bool gyro_updated = false;
volatile bool accel_updated = false;
volatile bool gps_updated = false;

static CAN_message_t msg;
static MegaCAN_broadcast_message_t ecu;

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
    gps.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
    gps.setNavigationFrequency(1);
    gps.setAutoPVT(true);

    if(accel.begin() < 0 || gyro.begin() < 0) {
        log("error with IMU... cannot connect...");
    }

    // set IMU interrupts
    pinMode(TEENSY_PIN_ACEL_INTERRUPT, INPUT_PULLDOWN);
    pinMode(TEENSY_PIN_GYRO_INTERRUPT, INPUT_PULLDOWN);
    pinMode(TEENSY_PIN_GPS_INTERRUPT, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(TEENSY_PIN_ACEL_INTERRUPT), accel_ISR, RISING);
    attachInterrupt(digitalPinToInterrupt(TEENSY_PIN_GYRO_INTERRUPT), gyro_ISR, RISING);
    attachInterrupt(digitalPinToInterrupt(TEENSY_PIN_GPS_INTERRUPT), gps_ISR, RISING);

    log("setup IMU interrupts");

    // setup ECU CAN interrupt
    can_ecu.begin();
    can_ecu.setBaudRate(500000); //set to 500000 for normal Megasquirt usage - need to change Megasquirt firmware to change MS CAN baud rate
    // can_ecu.setMaxMB(16); //sets maximum number of mailboxes for FlexCAN_T4 usage
    // can_ecu.enableFIFO();
    // can_ecu.setFIFOFilter(ACCEPT_ALL); // or specific IDs
    // can_ecu.mailboxStatus();

    log("set up can ecu interrupts");

    // setup sensor CAN interrupt
    can_sensor.begin();
    can_sensor.setBaudRate(500000); //set to 500000 for normal Megasquirt usage - need to change Megasquirt firmware to change MS CAN baud rate
    can_sensor.setMaxMB(16); //sets maximum number of mailboxes for FlexCAN_T4 usage
    can_sensor.enableFIFO();
    can_sensor.setFIFOFilter(ACCEPT_ALL); // or specific IDs
    // can_sensor.mailboxStatus();

    log("set up can sensor interrupts");
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

    // ECU CAN ====================================================================================
    can_ecu.events();
    while (can_ecu.read(msg)) {
        log("message from can!!");
        if (msg.flags.extended) continue;

        megaCAN.getBCastData(msg.id, msg.buf, ecu);

        log(std::to_string(ecu.tps));

        rowLock.lock();
        row.rpm             = ecu.rpm;
        row.time            = ecu.seconds;
        row.afr             = ecu.AFR1          * 1000;
        row.fuelload        = ecu.fuelload      * 1000;
        row.spark_advance   = ecu.adv_deg       * 1000;
        row.baro            = ecu.baro          * 1000;
        row.map             = ecu.map           * 1000;
        row.mat             = ecu.mat           * 1000;
        row.clnt_temp       = ecu.clt           * 1000;
        row.tps             = ecu.tps           * 1000;
        row.batt            = ecu.batt          * 1000;
        row.oil_press       = ecu.sensors1      * 1000;
        row.syncloss_count  = ecu.synccnt;
        row.syncloss_code   = ecu.syncreason;
        row.ltcl_timing     = ecu.launch_timing * 1000;
        row.ve1             = ecu.ve1           * 1000;
        row.ve2             = ecu.ve2           * 1000;
        row.egt             = ecu.egt1          * 1000;
        row.maf             = ecu.MAF           * 1000;
        row.in_temp         = ecu.airtemp       * 1000;
        row.ecu_millis      = millis();
        rowLock.unlock();
    }

    // Sensor CAN =================================================================================
    can_sensor.events();
    while (can_sensor.read(msg)) {
        rowLock.lock();
        switch (msg.id) {
            case SENSOR_CAN_ID_DASHBOARD:
                // row.rpm = (int)msg.buf[0];
                break;
            case SENSOR_CAN_ID_BRAKE_1:
                row.brake1 = (int)*msg.buf;
                break;
            case SENSOR_CAN_ID_BRAKE_2:
                row.brake2 = (int)*msg.buf;
                break;
            case SENSOR_CAN_ID_SUSPOT_1:
                row.susp_pot_1 = (int)*msg.buf;
                break;
            case SENSOR_CAN_ID_SUSPOT_2:
                row.susp_pot_2 = (int)*msg.buf;
                break;
            case SENSOR_CAN_ID_SUSPOT_3:
                row.susp_pot_3 = (int)*msg.buf;
                break;
            case SENSOR_CAN_ID_SUSPOT_4:
                row.susp_pot_4 = (int)*msg.buf;
                break;
            case SENSOR_CAN_ID_RAD_IN:
                row.rad_in = (int)*msg.buf;
                break;
            case SENSOR_CAN_ID_RAD_OUT:
                row.rad_out = (int)*msg.buf;
                break;
        }
        rowLock.unlock();
    }
}
