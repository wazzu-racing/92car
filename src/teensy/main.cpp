#include <Arduino.h>
#include <TeensyThreads.h>
// #include <Watchdog_t4.h>

#include "DataloggerManager.hpp"
#include "DashboardManager.hpp"
#include "EthernetManager.hpp"
#include "SensorManager.hpp"
#include "Row.hpp"

#include "constants.hpp"

Threads::Mutex rowLock;
Row row;

// // WDT_T4<WDT1> wdt;

// // void wdtCallback() {
// //   Serial.println("FEED ME!");
// // }

DashboardManager dashboardManager;
SensorManager sensorManager(row, rowLock);
DataloggerManager dataloggerManager(row, rowLock);
EthernetManager ethernetManager;

void setup() {
  // rowLock.lock();
  // row = Row();
  // rowLock.unlock();

  // Serial.begin(9600);

  // // if (CrashReport) {
  // //   Serial.println("--- CRASH REPORT DETECTED ---");
  // //   Serial.print(CrashReport);
  // //   Serial.println("-----------------------------");
  // // }

  pinMode(TEENSY_PIN_LED_0, OUTPUT);
  pinMode(TEENSY_PIN_LED_1, OUTPUT);
  pinMode(TEENSY_PIN_LED_2, OUTPUT);
  pinMode(TEENSY_PIN_LED_3, OUTPUT);

  // WDT_timings_t config;
  // config.trigger = 5.0; // 5 seconds to trigger callback
  // config.timeout = 10.0; // 10 seconds to reset
  // config.callback = wdtCallback;
  // wdt.begin(config);

  // dashboardManager.start("dash_manager");
  dataloggerManager.start("datalog_manager");
  sensorManager.start("sensor_manager");
    // digitalWrite(TEENSY_PIN_LED_3, LOW);
  // ethernetManager.start("ethernet_manager");
}

void loop() {
  // digitalWrite(TEENSY_PIN_LED_0, HIGH);
  // delay(200);
  // digitalWrite(TEENSY_PIN_LED_0, LOW);
  // delay(200);
  // wdt.feed();
  digitalWrite(TEENSY_PIN_LED_0, !dashboardManager.getBlink() ? LOW : HIGH);
  digitalWrite(TEENSY_PIN_LED_1, !dataloggerManager.getBlink() ? LOW : HIGH);
  digitalWrite(TEENSY_PIN_LED_2, !sensorManager.getBlink() ? LOW : HIGH);
  // digitalWrite(TEENSY_PIN_LED_3, !ethernetManager.getBlink() ? LOW : HIGH);
}
