#pragma once

#include "ThreadedManager.hpp"

class EthernetManager: public ThreadedManager {
    void loop();
    void setup();
};