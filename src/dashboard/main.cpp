#include <Arduino.h>
#include <FlexCAN_T4.h>
#include "constants.hpp"


FlexCAN_T4<TEENSY_CAN_SENSOR, RX_SIZE_256, TX_SIZE_16> can_sensor;

void setup(){
    Serial.begin(9600);
    Serial.println("Hello, world");

    can_sensor.begin();
    can_sensor.setBaudRate(500000);
    can_sensor.setMaxMB(16);
    can_sensor.enableFIFO();
    can_sensor.setFIFOFilter(ACCEPT_ALL);
}

void loop(){
    can_sensor.events();
    CAN_message_t msg;
    msg.id = SENSOR_CAN_ID_DASHBOARD;
    msg.len = 1;
    msg.buf[0] = 67;

    bool result = can_sensor.write(msg);
    if(result){
        Serial.println("Sent dashboard message");
    } else {
        Serial.println("Failed to send dashboard message");
    }
    delay(1000);
}
