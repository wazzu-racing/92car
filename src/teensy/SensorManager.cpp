#include "SensorManager.hpp"

volatile bool gps_updated = false;
volatile bool accel_updated = false;
volatile bool gyro_updated = false;

static CAN_message_t msg;
static MegaCAN_broadcast_message_t ecu;

void gyro_ISR() {gyro_updated = true;}
void accel_ISR() {accel_updated = true;}
void gps_ISR() {gps_updated = true;}
void accel_ISR() {accel_updated = true;}
void gyro_ISR() {gyro_updated = true;}


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

    if (accel.begin() < 0 || gyro.begin() < 0) {
        log("error with IMU... cannot connect...");
    }
    accel.mapDrdyInt1(true);
    // accel.setOdr(accel.ODR_200HZ_BW_38HZ);
    gyro.mapDrdyInt3(true);
    // gyro.setOdr(gyro.ODR_200HZ_BW_64HZ);


    pinMode(TEENSY_PIN_SUSPOT_RL, INPUT);
    pinMode(TEENSY_PIN_SUSPOT_RR, INPUT);
    pinMode(TEENSY_PIN_RAD_IN, INPUT);
    pinMode(TEENSY_PIN_RAD_OUT, INPUT);

    // set IMU interrupts
    pinMode(TEENSY_PIN_ACEL_INTERRUPT, INPUT);
    pinMode(TEENSY_PIN_GYRO_INTERRUPT, INPUT_PULLDOWN);
    pinMode(TEENSY_PIN_GPS_INTERRUPT, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(TEENSY_PIN_ACEL_INTERRUPT), accel_ISR, RISING);
    attachInterrupt(digitalPinToInterrupt(TEENSY_PIN_GYRO_INTERRUPT), gyro_ISR, RISING);
    attachInterrupt(digitalPinToInterrupt(TEENSY_PIN_GPS_INTERRUPT), gps_ISR, RISING);

    log("setup IMU interrupts");

    // setup ECU CAN interrupt
    can_ecu.begin();
    can_ecu.setBaudRate(500000); //set to 500000 for normal Megasquirt usage - need to change Megasquirt firmware to change MS CAN baud rate
    can_ecu.setMaxMB(16); //sets maximum number of mailboxes for FlexCAN_T4 usage
    can_ecu.enableFIFO();
    can_ecu.setFIFOFilter(ACCEPT_ALL); // or specific IDs
    can_ecu.mailboxStatus();

    log("set up can ecu interrupts");

    // setup sensor CAN interrupt
    can_sensor.begin();
    can_sensor.setBaudRate(500000); //set to 500000 for normal Megasquirt usage - need to change Megasquirt firmware to change MS CAN baud rate
    can_sensor.setMaxMB(16); //sets maximum number of mailboxes for FlexCAN_T4 usage
    can_sensor.enableFIFO();
    can_sensor.setFIFOFilter(ACCEPT_ALL); // or specific IDs
    can_sensor.mailboxStatus();

    log("set up can sensor interrupts");


    THERMOCOUPLE_I2C_BUS.begin();
}

void SensorManager::loop() {
    // accelerometer of IMU =======================================================================
    if (accel_updated) {
        log("accel!!");
        accel_updated = false;

        accel.readSensor();

        rowLock.lock();
        row.ax           = accel.getAccelX_mss() * 1000;
        row.ay           = accel.getAccelY_mss() * 1000;
        row.az           = accel.getAccelZ_mss() * 1000;
        // Serial.println(row.az);
        row.accel_millis = millis();
        rowLock.unlock();
    }

    // gyroscope of IMU ===========================================================================
    if (gyro_updated) {
        log("imu!");
        gyro_updated = false;

        gyro.readSensor();

        rowLock.lock();
        row.imu_x      = gyro.getGyroX_rads() * 1000;
        row.imu_y      = gyro.getGyroY_rads() * 1000;
        row.imu_z      = gyro.getGyroZ_rads() * 1000;
        row.imu_millis = millis();
        rowLock.unlock();
    }

    if ((gps.getPVT(1) && (gps.getInvalidLlh() == false))) {
        // log("gps!");

        rowLock.lock();
        row.unixtime      =  gps.getUnixEpoch();
        row.lat           =  gps.getLatitude();
        row.lon           =  gps.getLongitude();
        row.elev          =  gps.getAltitude();
        row.ground_speed  =  gps.getGroundSpeed();
        row.gps_millis    = millis();
        rowLock.unlock();
    }
    // else {
    //     log("no gps");
    // }

    // ECU CAN ====================================================================================
    while (can_ecu.read(msg)) {
        // log("message from can!!");
        // Serial.println(msg.id);
        // if (msg.flags.extended) continue;

        megaCAN.getBCastData(msg.id, msg.buf, ecu);

        // Serial.print("RPM:\t\t\t");Serial.print(ecu.rpm);Serial.println("\tRPM");
        // Serial.print("Time:\t\t\t");Serial.print(ecu.seconds);Serial.println("\tseconds");
        // Serial.print("AFR1:\t\t\t");Serial.print(ecu.AFR1);Serial.println("\tAFR");
        // Serial.print("spark advance:\t\t");Serial.print(ecu.adv_deg);Serial.println("\tdeg BTDC");
        // Serial.print("barometric pressure:\t");Serial.print(ecu.baro);Serial.println("\tkPa");
        // Serial.print("manifold pressure:\t");Serial.print(ecu.map);Serial.println("\tkPa");
        // Serial.print("manifold temp:\t\t");Serial.print(ecu.mat);Serial.println("\tdeg F");
        // Serial.print("coolant temp:\t\t");Serial.print(ecu.clt);Serial.println("\tdeg F");
        // Serial.print("throttle position:\t");Serial.print(ecu.tps);Serial.println("\t%");
        // Serial.print("battery voltage:\t");Serial.print(ecu.batt);Serial.println("\tV");
        // Serial.print("oil pressure (egov2):\t");Serial.print(ecu.sensors1);Serial.println("\t");
        // Serial.print("sync loss counter:\t");Serial.print(ecu.synccnt);Serial.println("\tcount");
        // Serial.print("sync reason:\t\t");Serial.print(ecu.syncreason);Serial.println("\t(code)");
        // Serial.print("launch control timing:\t");Serial.print(ecu.launch_timing);Serial.println("\tdeg");
        // Serial.print("VE1:\t\t\t");Serial.print(ecu.ve1);Serial.println("\t%");
        // Serial.print("VE2:\t\t\t");Serial.print(ecu.ve2);Serial.println("\t%");
        // Serial.print("EGT:\t\t\t");Serial.print(ecu.egt1);Serial.println("\tdeg F");
        // Serial.print("Mass air flow:\t\t");Serial.print(ecu.MAF);Serial.println("\tg/s");
        // Serial.print("Intake air temp:\t");Serial.print(ecu.airtemp);Serial.println("\tdeg F");
        // Serial.print("Gear:\t\t\t");Serial.print(ecu.gear);Serial.println("\t");
        // Serial.print("\n\n");Serial.println("\t()");
        // Serial.println(millis());


        rowLock.lock();
        row.rpm             = ecu.rpm;
        row.time            = ecu.seconds;
        // row.quickshift      = ecu.
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
                row.susp_pot_1 = (int16_t)(msg.buf[0] | (msg.buf[1] << 8));
                row.susp_pot_2 = (int16_t)(msg.buf[2] | (msg.buf[3] << 8));
                row.brake1     = (int16_t)(msg.buf[4] | (msg.buf[5] << 8));
                row.brake2     = (int16_t)(msg.buf[6] | (msg.buf[7] << 8));
                break;
            case SENSOR_CAN_ID_DASHBOARD + 1:
                row.steering = (int16_t)(msg.buf[0] | (msg.buf[1] << 8));
                row.amb_air_temp = (int16_t)(msg.buf[2] | (msg.buf[3] << 8));
                row.steering_millis = millis();
                break;
            case SENSOR_CAN_ID_BRAKE_1:
                row.brake1 = (int16_t)(msg.buf[0] | (msg.buf[1] << 8));
                break;
            case SENSOR_CAN_ID_BRAKE_2:
                row.brake2 = (int16_t)(msg.buf[0] | (msg.buf[1] << 8));
                break;
            case SENSOR_CAN_ID_SUSPOT_FL:
                row.susp_pot_FL = (int)*msg.buf;
                break;
            case SENSOR_CAN_ID_SUSPOT_FR:
                row.susp_pot_FR = (int)*msg.buf;
                break;
            case SENSOR_CAN_ID_RAD_IN:
                row.rad_in = (int16_t)(msg.buf[0] | (msg.buf[1] << 8));
                break;
            case SENSOR_CAN_ID_RAD_OUT:
                row.rad_out = (int16_t)(msg.buf[0] | (msg.buf[1] << 8));
                break;
        }
        row.breakout_millis = millis();
        rowLock.unlock();
    }

    // ONBOARD ANALOG =============================================================================
    rowLock.unlock();
    row.susp_pot_RL = analogRead(TEENSY_PIN_SUSPOT_RL);
    row.susp_pot_RR = analogRead(TEENSY_PIN_SUSPOT_RR);
    row.rad_in = analogRead(TEENSY_PIN_RAD_IN);
    row.rad_out = analogRead(TEENSY_PIN_RAD_OUT);
    row.analog_millis = millis();
    rowLock.lock();

}
