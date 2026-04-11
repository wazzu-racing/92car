#include <Arduino.h>
#include <TeensyThreads.h>

#include "DataloggerManager.hpp"
#include "DashboardManager.hpp"
#include "EthernetManager.hpp"
#include "SensorManager.hpp"

#include "constants.hpp"

DashboardManager dashboardManager;
SensorManager sensorManager;
DataloggerManager dataloggerManager(sensorManager.currentRow, sensorManager.currentRowLock);
EthernetManager ethernetManager;

void setup() {
  Serial.begin(9600);

  pinMode(TEENSY_PIN_LED_0, OUTPUT);
  pinMode(TEENSY_PIN_LED_1, OUTPUT);
  pinMode(TEENSY_PIN_LED_2, OUTPUT);
  pinMode(TEENSY_PIN_LED_3, OUTPUT);

  // dashboardManager.start("dash_manager");
  dataloggerManager.start("datalog_manager");
  sensorManager.start("sensor_manager");
  // ethernetManager.start("ethernet_manager");
}

void loop() {
  digitalWrite(TEENSY_PIN_LED_0, dashboardManager.getBlink() ? LOW : HIGH);
  digitalWrite(TEENSY_PIN_LED_1, dataloggerManager.getBlink() ? LOW : HIGH);
  digitalWrite(TEENSY_PIN_LED_2, sensorManager.getBlink() ? LOW : HIGH);
  digitalWrite(TEENSY_PIN_LED_3, ethernetManager.getBlink() ? LOW : HIGH);
}
