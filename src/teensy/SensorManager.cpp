#include "SensorManager.hpp"

// volatile bool gps_updated = false;
volatile bool accel_updated = false;
volatile bool gyro_updated = false;

static MegaCAN_broadcast_message_t ecu;

void gyro_ISR() {gyro_updated = true;}
void accel_ISR() {accel_updated = true;}
// void gps_ISR() {gps_updated = true;}

void SensorManager::setup() {
    log("Setup SensorManager");

    TEENSY_GPS_WIRE.begin();
    TEENSY_IMU_WIRE.begin();

    if (gps.begin(TEENSY_GPS_WIRE) == false) { //Connect to the u-blox module using Wire port {
        log("error with GPS... cannot connect...");
        gps_enabled = false;
    } else {
        log("gps connected...");
        gps.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
        gps.setNavigationFrequency(1);
        gps.setAutoPVT(true);
        gps_enabled = true;
    }

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
    pinMode(TEENSY_PIN_OIL_TEMP, INPUT);

    log("start thermocouple");
    THERMOCOUPLE_I2C_BUS.begin();
    mcp[0]->begin(THERMOCOUPLE_1_ID, &THERMOCOUPLE_I2C_BUS);
    mcp[1]->begin(THERMOCOUPLE_2_ID, &THERMOCOUPLE_I2C_BUS);
    mcp[2]->begin(THERMOCOUPLE_3_ID, &THERMOCOUPLE_I2C_BUS);
    mcp[3]->begin(THERMOCOUPLE_4_ID, &THERMOCOUPLE_I2C_BUS);

    log("end thermo");

    for (int i=0; i<4; i++) {
        // // mcp[i] =
        // mcp[i]->setAmbientResolution(ambientRes);
        // mcp[i]->setADCresolution(MCP9600_ADCRESOLUTION_18);
        // mcp[i]->setThermocoupleType(MCP9600_TYPE_K);
        // mcp[i]->setFilterCoefficient(3);
        // mcp[i]->setAlertTemperature(1, 30);
        // mcp[i]->configureAlert(1, true, true);
        // mcp[i]->enable(true);
    }

    log("finish mcp thermo setup");
    // set IMU interrupts
    pinMode(TEENSY_PIN_ACEL_INTERRUPT, INPUT);
    pinMode(TEENSY_PIN_GYRO_INTERRUPT, INPUT_PULLDOWN);
    pinMode(TEENSY_PIN_GPS_INTERRUPT, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(TEENSY_PIN_ACEL_INTERRUPT), accel_ISR, RISING);
    attachInterrupt(digitalPinToInterrupt(TEENSY_PIN_GYRO_INTERRUPT), gyro_ISR, RISING);
    // attachInterrupt(digitalPinToInterrupt(TEENSY_PIN_GPS_INTERRUPT), gps_ISR, RISING);

    log("setup IMU interrupts");

    // setup ECU CAN interrupt
    can_ecu.begin();
    can_ecu.setBaudRate(500000); //set to 500000 for normal Megasquirt usage - need to change Megasquirt firmware to change MS CAN baud rate
    can_ecu.setMaxMB(16); //sets maximum number of mailboxes for FlexCAN_T4 usage
    can_ecu.enableFIFO();
    // can_ecu.setFIFOFilter(ACCEPT_ALL); // or specific IDs
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
}

void SensorManager::loop() {
      digitalWrite(TEENSY_PIN_LED_2, ((millis()/1000) % 2 == 0) ? LOW : HIGH);

    // accelerometer of IMU =======================================================================
    if (accel_updated) {
        // log("accel!!");
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
        // log("imu!");
        gyro_updated = false;

        gyro.readSensor();

        rowLock.lock();
        row.imu_x      = gyro.getGyroX_rads() * 1000;
        row.imu_y      = gyro.getGyroY_rads() * 1000;
        row.imu_z      = gyro.getGyroZ_rads() * 1000;
        row.imu_millis = millis();
        rowLock.unlock();
    }

    if (gps_enabled && (gps.getPVT(1) && (gps.getInvalidLlh() == false))) {
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
    else {
        log("no gps");
    }

    // ECU CAN ====================================================================================
    // digitalWrite(TEENSY_PIN_LED_0, HIGH);
    CAN_message_t msg;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // CAN_error_t err = can_ecu.error();

    // if (err & BUS_OFF) {
    //     warning("CAN BUS OFF");

    //     can_ecu.reset();

    //     can_ecu.begin();
    //     can_ecu.setBaudRate(500000);
    //     can_ecu.setMaxMB(16);
    //     can_ecu.enableFIFO();
    //     can_ecu.setFIFOFilter(ACCEPT_ALL);
    // }

    // int count = 0;
    // can_ecu.events();
    // while (can_ecu.read(msg)) {
    //     if (count++ > 20) {
    //         digitalWrite(TEENSY_PIN_LED_3, HIGH);
    //     }
        

    //     if (msg.flags.extended) continue;
    //     log("Reading CAN data");

    //     // Defensive: only handle known ECU IDs (optional, expand this if you expect more)
    //     uint32_t relid = msg.id - ECU_BASE_CAN_ID;
    //     if (relid > 7) { log("ECU CAN RX: unknown ID, ignored"); continue; }

    //     // Basic validation: drop frames that are not standard length, or are too short
    //     if (msg.len < 8) {
    //         warning("ECU CAN RX: frame too short, ignoring");
    //         continue;
    //     }


    //     megaCAN.getBCastData(msg.id, msg.buf, ecu);

    //     rowLock.lock();
    //     row.rpm             = ecu.rpm;
    //     row.time            = ecu.seconds;
    //     // row.quickshift      = ecu.
    //     row.afr             = ecu.AFR1          * 1000;
    //     row.fuelload        = ecu.fuelload      * 1000;
    //     row.spark_advance   = ecu.adv_deg       * 1000;
    //     row.baro            = ecu.baro          * 1000;
    //     row.map             = ecu.map           * 1000;
    //     row.mat             = ecu.mat           * 1000;
    //     row.clnt_temp       = ecu.clt           * 1000;
    //     row.tps             = ecu.tps           * 1000;
    //     row.batt            = ecu.batt          * 1000;
    //     row.oil_press       = ecu.sensors1      * 1000;
    //     row.syncloss_count  = ecu.synccnt;
    //     row.syncloss_code   = ecu.syncreason;
    //     row.ltcl_timing     = ecu.launch_timing * 1000;
    //     row.ve1             = ecu.ve1           * 1000;
    //     row.ve2             = ecu.ve2           * 1000;
    //     row.maf             = ecu.MAF           * 1000;
    //     row.in_temp         = ecu.airtemp       * 1000;
    //     row.ecu_millis      = millis();
    //     rowLock.unlock();
    // }
    // digitalWrite(TEENSY_PIN_LED_0, LOW);

    // Sensor CAN =================================================================================
    can_sensor.events();
    while (can_sensor.read(msg)) {
        rowLock.lock();
        switch (msg.id) {
            case SENSOR_CAN_ID_AMBAIR:
                memcpy(&row.amb_air_temp, msg.buf, sizeof(int));
                break;
            case SENSOR_CAN_ID_BRAKE_1:
                memcpy(&row.brake1, msg.buf, sizeof(int));
                break;
            case SENSOR_CAN_ID_BRAKE_2:
                memcpy(&row.brake2, msg.buf, sizeof(int));
                break;
            case SENSOR_CAN_ID_SUSPOT_FL:
                memcpy(&row.susp_pot_FL, msg.buf, sizeof(int));
                break;
            case SENSOR_CAN_ID_SUSPOT_FR:
                memcpy(&row.susp_pot_FR, msg.buf, sizeof(int));
                break;
            case SENSOR_CAN_ID_STEERING:
                memcpy(&row.steering, msg.buf, sizeof(int));
                break;
        }
        row.breakout_millis = millis();
        rowLock.unlock();
    }

    // ONBOARD ANALOG =============================================================================
    rowLock.lock();
    // log("analog...");
    row.susp_pot_RL = analogRead(TEENSY_PIN_SUSPOT_RL);
    row.susp_pot_RR = analogRead(TEENSY_PIN_SUSPOT_RR);
    row.rad_in = analogRead(TEENSY_PIN_RAD_IN);
    row.rad_out = analogRead(TEENSY_PIN_RAD_OUT);
    row.oil_temp = analogRead(TEENSY_PIN_OIL_TEMP);
    row.analog_millis = millis();
    rowLock.unlock();

    // THERMOCOUPLE ===============================================================================
    // rowLock.lock();
    // row.thermo_1 = mcp[0]->readThermocouple() * 1000;
    // row.thermo_2 = mcp[1]->readThermocouple() * 1000;
    // row.thermo_3 = mcp[2]->readThermocouple() * 1000;
    // row.thermo_4 = mcp[3]->readThermocouple() * 1000;
    // for (int i=0; i<4; i++) Serial.println(mcp[i]->readThermocouple());
    // row.thermo_millis = millis();
    // rowLock.unlock();
}
