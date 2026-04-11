#pragma once

// #include "Arduino.h"
#include <FlexCAN_T4.h>
#include <MegaCAN.h>

#define CAN_BASE_ID 1512
#define CAN_ID CAN_BASE_ID + 1

class MegaSquirt3 {
  MegaCAN megaCAN;

public:
  MegaCAN_broadcast_message_t data;

  MegaSquirt3() : megaCAN(CAN_BASE_ID) {}
  void initialize();
  void eventloop();
  bool decode(const CAN_message_t &msg);
};
