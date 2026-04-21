#pragma once

#define DATALOG_MAX_HZ              50
#define DATALOG_SD_BUFFER           1
#define RADIO_MAX_HZ                5

#define JAN_1_2025                  1735718400

#define ANALOGX_0_CAN_ID            0xE4600
#define ANALOGX_1_CAN_ID            0xE4700
#define ANALOGX_2_CAN_ID            0xE4800
#define ANALOGX_3_CAN_ID            0xE4900

#define ECU_BASE_CAN_ID             0x0
#define SENSOR_CAN_ID_BRAKE_1       0x100 + 1
#define SENSOR_CAN_ID_BRAKE_2       0x100 + 2
#define SENSOR_CAN_ID_SUSPOT_1      0x100 + 3
#define SENSOR_CAN_ID_SUSPOT_2      0x100 + 4
#define SENSOR_CAN_ID_SUSPOT_3      0x100 + 5
#define SENSOR_CAN_ID_SUSPOT_4      0x100 + 6
#define SENSOR_CAN_ID_RAD_IN        0x100 + 7
#define SENSOR_CAN_ID_RAD_OUT       0x100 + 8
#define SENSOR_CAN_ID_DASHBOARD     0x100 + 9

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
#define TEENSY_PIN_GENERIC_14       14
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

#define TEENSY_PIN_GENERIC_26       26
#define TEENSY_PIN_GENERIC_27       27
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
#define TEENSY_PIN_GENERIC_40       40
#define TEENSY_PIN_GPS_INTERRUPT    41
