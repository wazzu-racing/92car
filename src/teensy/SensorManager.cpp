#include "SensorManager.hpp"

void SensorManager::setup() {
    log("Setup SensorManager");
    TEENSY_GPS_WIRE.begin();
    if (myGNSS.begin(TEENSY_GPS_WIRE) == false) { //Connect to the u-blox module using Wire port {
        log("error with GPS... cannot connect...");
        while (1);
    }
    log("gps connected...");
        myGNSS.checkUblox();

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
}