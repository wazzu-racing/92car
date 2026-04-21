#include <Arduino.h>
#include <FlexCAN_T4.h>
#include "constants.hpp"

#include <GBALib_Potentiometer.h>


FlexCAN_T4<TEENSY_CAN_SENSOR, RX_SIZE_256, TX_SIZE_16> can_sensor;


GBALib_Potentiometer susp_pot_fl(BREAKOUT_TEENSY_PIN_SUSP_POT_FL);
GBALib_Potentiometer susp_pot_fr(BREAKOUT_TEENSY_PIN_SUSP_POT_FR);


void setup(){
    Serial.begin(9600);

    can_sensor.begin();
    can_sensor.setBaudRate(500000);
    can_sensor.setMaxMB(16);
    can_sensor.enableFIFO();
    can_sensor.setFIFOFilter(ACCEPT_ALL);

    susp_pot_fl.range(0, 255);
    susp_pot_fr.range(0, 255);
}

void loop(){
    int val_fl = susp_pot_fl.value();
    int val_fr = susp_pot_fr.value();


    can_sensor.events();
    CAN_message_t msg;
    msg.id = SENSOR_CAN_ID_DASHBOARD;
    msg.len = 2;
    msg.buf[0] = val_fl;
    msg.buf[1] = val_fr;

    bool result = can_sensor.write(msg);
    if(result){
        Serial.println("Sent dashboard message");
    } else {
        Serial.println("Failed to send dashboard message");
    }
    delay(100);
}
