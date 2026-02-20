#include <Arduino.h>
#include "MegaSquirt3.h"

#include "pins.hpp"


FlexCAN_T4<TEENSY_CAN_SENSOR, RX_SIZE_256, TX_SIZE_16> Can;

MegaSquirt3 ecu;

ROW tosend;

void handler(const CAN_message_t &msg) {
if (ecu.decode(msg)) {
     tosend.rpm = ecu.data.rpm;
     tosend.time = ecu.data.seconds;
     tosend.afr = ecu.data.AFR1 * 1000;
     tosend.fuelload = ecu.data.fuelload * 1000;
     tosend.spark_advance = ecu.data.adv_deg * 1000;
     tosend.baro = ecu.data.baro * 1000;
     tosend.map = ecu.data.map * 1000;
     tosend.mat = ecu.data.mat * 1000;
     tosend.clnt_temp = ecu.data.clt * 1000;
     tosend.tps = ecu.data.tps * 1000;
     tosend.batt = ecu.data.batt * 1000;
     tosend.oil_press = ecu.data.sensors1 * 1000;
     tosend.syncloss_count = ecu.data.synccnt;
    //  TEST += ecu.data.synccnt;
    //  Serial.print("Sync loss total:\t");
    //  Serial.println(TEST);
     tosend.syncloss_code = ecu.data.syncreason;
     tosend.ltcl_timing = ecu.data.launch_timing * 1000;
     tosend.ve1 = ecu.data.ve1 * 1000;
     tosend.ve2 = ecu.data.ve2 * 1000;
     tosend.egt = ecu.data.egt1 * 1000;
     tosend.maf = ecu.data.MAF * 1000;
     tosend.in_temp = ecu.data.airtemp * 1000;
     tosend.ecu_millis = millis();
    // check engine
    bool evenodd = ((int)floor(millis()/150)) % 2;
    bool engine_bad = (ecu.data.clt > 215) || (ecu.data.sensors1 < 5) || (ecu.data.sensors1 > 125) || (ecu.data.batt > 15) || (ecu.data.batt < 7);
    bool engine_really_bad = (ecu.data.clt > 230) || (ecu.data.sensors1 > 150) || (ecu.data.sensors1 < 1);

    // show displays
    // Serial.print(ecu.data.syncreason);
    // Serial.print(", ");
    // Serial.println(ecu.data.synccnt);
 
  }}

void setup() {
  pinMode(TEENSY_PIN_LED_0, OUTPUT);
  pinMode(TEENSY_PIN_LED_1, OUTPUT);
  pinMode(TEENSY_PIN_LED_2, OUTPUT);
  pinMode(TEENSY_PIN_LED_3, OUTPUT);
  digitalWrite(TEENSY_PIN_LED_0, LOW);
  digitalWrite(TEENSY_PIN_LED_1, LOW);
  digitalWrite(TEENSY_PIN_LED_2, LOW);
  digitalWrite(TEENSY_PIN_LED_3, LOW);

  Serial.begin(9600);
  Serial8.begin(9600);


    Can.begin();
  Can.setBaudRate(500000); //set to 500000 for normal Megasquirt usage - need to change Megasquirt firmware to change MS CAN baud rate
  Can.setMaxMB(16); //sets maximum number of mailboxes for FlexCAN_T4 usage
  Can.enableFIFO();
  Can.enableFIFOInterrupt();
  Can.mailboxStatus();
  Can.onReceive(handler);

}

uint32_t lastwritelora = 0;

void loop() {
 if (millis() > lastwritelora + 200) {
      tosend.write_millis = millis();
      Serial8.write((byte*) &tosend, sizeof(tosend));
      Serial8.print("\n\n\n");
      Serial.printf("sent bytes to LoRa...\n");
      lastwritelora = millis();
    }

  // Serial.println(millis());
  // digitalWrite(TEENSY_PIN_LED_0, LOW);
  // delay(500);
  // digitalWrite(TEENSY_PIN_LED_1, LOW);
  // delay(500);
  // digitalWrite(TEENSY_PIN_LED_2, LOW);
  // delay(500);
  // digitalWrite(TEENSY_PIN_LED_3, LOW);
  // delay(500);
  // digitalWrite(TEENSY_PIN_LED_0, HIGH);
  // delay(500);
  // digitalWrite(TEENSY_PIN_LED_1, HIGH);
  // delay(500);
  // digitalWrite(TEENSY_PIN_LED_2, HIGH);
  // delay(500);
  // digitalWrite(TEENSY_PIN_LED_3, HIGH);
  // delay(500);
}