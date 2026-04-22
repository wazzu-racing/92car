#include "constants.hpp"
#include <Arduino.h>
#include <FlexCAN_T4.h>

#include <GBALib_Potentiometer.h>
#include <PressureTransducer.h>

FlexCAN_T4<TEENSY_CAN_SENSOR, RX_SIZE_256, TX_SIZE_16> can_sensor;

GBALib_Potentiometer susp_pot_fl(BREAKOUT_TEENSY_PIN_SUSP_POT_FL);
GBALib_Potentiometer susp_pot_fr(BREAKOUT_TEENSY_PIN_SUSP_POT_FR);
GBALib_Potentiometer steering(BREAKOUT_TEENSY_PIN_STEERING);

// Brake Pressure: Assuming 0.5V - 4.5V for 0 - 2000 PSI (common automotive
// spec) Parameters: pin, minPressure, maxPressure, minVoltage, maxVoltage,
// vRef, adcBits
// PressureTransducer brake_1(BREAKOUT_TEENSY_PIN_BRAKE_1, BRAKE_PRESSURE_MIN_PSI,
//                            BRAKE_PRESSURE_MAX_PSI, BRAKE_PRESSURE_MIN_VOLTAGE,
//                            BRAKE_PRESSURE_MAX_VOLTAGE, BRAKE_PRESSURE_VREF,
//                            BRAKE_PRESSURE_ADC_BITS);
// PressureTransducer brake_2(BREAKOUT_TEENSY_PIN_BRAKE_2, BRAKE_PRESSURE_MIN_PSI,
//                            BRAKE_PRESSURE_MAX_PSI, BRAKE_PRESSURE_MIN_VOLTAGE,
//                            BRAKE_PRESSURE_MAX_VOLTAGE, BRAKE_PRESSURE_VREF,
//                            BRAKE_PRESSURE_ADC_BITS);

// Air Temp: NTC Thermistor
// Simplified Steinhart-Hart implementation to avoid library issues
// float readThermistor(int pin) {
//   int reading = analogRead(pin);
//   if (reading == 0)
//     return -273.15; // Avoid division by zero

//   float resistance =
//       THERMISTOR_SERIES_RESISTOR *
//       ((float)((1 << BRAKE_PRESSURE_ADC_BITS) - 1) / reading - 1);
//   float steinhart;
//   steinhart = resistance / THERMISTOR_NOMINAL_RESISTANCE;       // (R/Ro)
//   steinhart = log(steinhart);                                   // ln(R/Ro)
//   steinhart /= THERMISTOR_B_COEFFICIENT;                        // 1/B * ln(R/Ro)
//   steinhart += 1.0 / (THERMISTOR_NOMINAL_TEMPERATURE + 273.15); // + (1/To)
//   steinhart = 1.0 / steinhart;                                  // Invert
//   steinhart -= 273.15;                                          // convert to C

//   return steinhart;
// }

void setup() {
  Serial.begin(9600);

  can_sensor.begin();
  can_sensor.setBaudRate(500000);
  can_sensor.setMaxMB(16);
  can_sensor.enableFIFO();
  can_sensor.setFIFOFilter(ACCEPT_ALL);

  susp_pot_fl.range(0, 1000); // 0-1000 scale for better precision over CAN
  susp_pot_fr.range(0, 1000);
  steering.range(0, 1000);

  // brake_1.begin();
  // brake_2.begin();
}

void loop() {
  // Read and scale to integers for CAN transmission
  int val_fl = susp_pot_fl.value();
  int val_fr = susp_pot_fr.value();
  int val_steer = steering.value();

  // Pressure in PSI * 10 (e.g. 1250 = 125.0 PSI)
  // int val_b1 = (int)(brake_1.readPressure() * 10);
  // int val_b2 = (int)(brake_2.readPressure() * 10);

  int val_b1 = 0;
  int val_b2 = 0;
  int val_air = 0;
  // // Temp in Celsius * 10 (e.g. 255 = 25.5 C)
  // int val_air = (int)(readThermistor(BREAKOUT_TEENSY_PIN_AIR_TEMP) * 10);

  can_sensor.events();

  CAN_message_t msg_fl;
  msg_fl.id = SENSOR_CAN_ID_SUSPOT_FL;
  msg_fl.len = 8;
  memcpy(msg_fl.buf, val_fl , 8);
  can_sensor.write(msg_fl);


  CAN_message_t msg_fr;
  msg_fr.id = SENSOR_CAN_ID_SUSPOT_FR;
  msg_fr.len = 8;
  memcpy(msg_fr.buf, val_fr , 8);
  can_sensor.write(msg_fr);

  // msg.id = SENSOR_CAN_ID_DASHBOARD;
  // msg.len = 8; // Use full buffer

  // // Pack into 16-bit values for transmission (Little Endian)
  // msg.buf[0] = val_fl & 0xFF;
  // msg.buf[1] = (val_fl >> 8) & 0xFF;
  // msg.buf[2] = val_fr & 0xFF;
  // msg.buf[3] = (val_fr >> 8) & 0xFF;
  // msg.buf[4] = val_b1 & 0xFF;
  // msg.buf[5] = (val_b1 >> 8) & 0xFF;
  // msg.buf[6] = val_b2 & 0xFF;
  // msg.buf[7] = (val_b2 >> 8) & 0xFF;

  // can_sensor.write(msg);

  // // Send second message for remaining data
  // msg.id = SENSOR_CAN_ID_DASHBOARD + 1;
  // msg.len = 4;
  // msg.buf[0] = val_steer & 0xFF;
  // msg.buf[1] = (val_steer >> 8) & 0xFF;
  // msg.buf[2] = val_air & 0xFF;
  // msg.buf[3] = (val_air >> 8) & 0xFF;

  // can_sensor.write(msg);

  delay(20); // 50Hz update rate
}
