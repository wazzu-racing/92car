#pragma once

#define DATALOG_MAX_HZ              50
#define DATALOG_SD_BUFFER           1
#define RADIO_MAX_HZ                5

#define JAN_1_2025                  1735718400

#define ANALOGX_0_CAN_ID            0xE4600
#define ANALOGX_1_CAN_ID            0xE4700
#define ANALOGX_2_CAN_ID            0xE4800
#define ANALOGX_3_CAN_ID            0xE4900

#define ECU_BASE_CAN_ID             1512
// #define SENSOR_CAN_ID_BREAKOUT      0x100
#define SENSOR_CAN_ID_BRAKE_1       0x100 + 1
#define SENSOR_CAN_ID_BRAKE_2       0x100 + 2
#define SENSOR_CAN_ID_SUSPOT_FL     0x100 + 3
#define SENSOR_CAN_ID_SUSPOT_FR     0x100 + 4
#define SENSOR_CAN_ID_RAD_IN        0x100 + 5
#define SENSOR_CAN_ID_RAD_OUT       0x100 + 6
#define SENSOR_CAN_ID_STEERING      0x100 + 7

#define THERMOCOUPLE_I2C_BUS        Wire2

#define TEENSY_PIN_GENERIC_0        0
#define TEENSY_PIN_GENERIC_1        1
#define TEENSY_PIN_MODE_BUTTON      2
#define TEENSY_PIN_GENERIC_3        3
#define TEENSY_PIN_GENERIC_4        4
#define TEENSY_PIN_GENERIC_5        5
//      EMPTY EMPTY EMPTY EMPTY     6
#define TEENSY_PIN_GENERIC_7        7
#define TEENSY_PIN_GENERIC_8        8
//      EMPTY EMPTY EMPTY EMPTY     9
#define TEENSY_PIN_SCREEN_DC        10
#define TEENSY_PIN_SCREEN_MOSI      11
#define TEENSY_PIN_SCREEN_RST       12
#define TEENSY_PIN_SCREEN_SCK       13
#define TEENSY_PIN_RAD_OUT          14
#define TEENSY_PIN_GENERIC_15       15
#define TEENSY_PIN_SEGMENT_SCL      16
#define TEENSY_PIN_SEGMENT_SDA      17

#define TEENSY_PIN_CAN_ECU_TX       22 // CAN 1
#define TEENSY_PIN_CAN_ECU_RX       23 // CAN 1
#define TEENSY_CAN_ECU              CAN1

#define TEENSY_PIN_GPS_SCL          24
#define TEENSY_PIN_GPS_SDA          25
#define TEENSY_GPS_WIRE             Wire2

#define TEENSY_IMU_WIRE             Wire
#define TEENSY_PIN_IMU_SDA          18
#define TEENSY_PIN_IMU_SCL          19
#define TEENSY_GYRO_REG             0x68
#define TEENSY_ACEL_REG             0x18
#define TEENSY_PIN_GYRO_INTERRUPT   20
#define TEENSY_PIN_ACEL_INTERRUPT   21

#define TEENSY_PIN_SUSPOT_RL        26
#define TEENSY_PIN_SUSPOT_RR        27
#define TEENSY_PIN_LED_0            28 // Status indicator for dashboardManager
#define TEENSY_PIN_LED_1            29 // Status indicator for dataloggerManager
#define TEENSY_PIN_CAN_SENSOR_RX    30 // CAN 3
#define TEENSY_PIN_CAN_SENSOR_TX    31 // CAN 3
#define TEENSY_CAN_SENSOR           CAN3

//      EMPTY EMPTY EMPTY EMPTY     32
//      EMPTY EMPTY EMPTY EMPTY     33
#define TEENSY_PIN_RADIO_TX         34
#define TEENSY_PIN_RADIO_RX         35
#define TEENSY_PIN_LED_3            36 // Status indicator for ethernetManager
#define TEENSY_PIN_LED_2            37 // Status indicator for sensorManager
#define TEENSY_PIN_LED_SHIFT        38
#define TEENSY_PIN_MARKER           39
#define TEENSY_PIN_RAD_IN           40
#define TEENSY_PIN_GPS_INTERRUPT    41





#define BREAKOUT_TEENSY_PIN_SUSP_POT_FL 38 // A14 on PCB
#define BREAKOUT_TEENSY_PIN_SUSP_POT_FR 16 // A2 on PCB
#define BREAKOUT_TEENSY_PIN_BRAKE_1     39 // A15 on PCB
#define BREAKOUT_TEENSY_PIN_BRAKE_2     41 // A17 on PCB
#define BREAKOUT_TEENSY_PIN_STEERING    40 // A16 on PCB
#define BREAKOUT_TEENSY_PIN_AIR_TEMP    22 // A8 on PCB

#define THERMISTOR_SERIES_RESISTOR      10000.0
#define THERMISTOR_NOMINAL_RESISTANCE   10000.0
#define THERMISTOR_NOMINAL_TEMPERATURE  25.0
#define THERMISTOR_B_COEFFICIENT        3950.0

#define BRAKE_PRESSURE_MIN_VOLTAGE      0.5
#define BRAKE_PRESSURE_MAX_VOLTAGE      4.5
#define BRAKE_PRESSURE_MIN_PSI          0.0
#define BRAKE_PRESSURE_MAX_PSI          2000.0
#define BRAKE_PRESSURE_VREF             3.3
#define BRAKE_PRESSURE_ADC_BITS         10
