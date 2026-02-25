#pragma once

#include "ThreadedManager.hpp"

class DashboardManager: public ThreadedManager {
    void loop();
    void setup();
};