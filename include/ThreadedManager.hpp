#pragma once
#include <TeensyThreads.h>
#include <Arduino.h>
#include <string>

class ThreadedManager {
    std::string logname_;

    volatile bool KEEP_RUNNING;
    volatile bool blink = false;


    virtual void loop() = 0;
    virtual void setup() = 0;

    static void runner(void* arg);

protected:
    void log(std::string data);

public:
    void start(std::string logname);
    void stop();

    bool getBlink();
};
