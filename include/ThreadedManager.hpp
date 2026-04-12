#pragma once
#include <TeensyThreads.h>
#include <Arduino.h>
#include <string>

// #ifndef WRAPPED_SERIAL_
// #define WRAPPED_SERIAL_
// ThreadWrap(Serial, SerialXtra);
// // #define Serial ThreadClone(SerialXtra)
// #endif

class ThreadedManager {
    std::string logname_;

    volatile bool KEEP_RUNNING;
    volatile bool blink = false;


    virtual void loop() = 0;
    virtual void setup() = 0;

    static void runner(void* arg);

protected:
    virtual void log(std::string data);

public:
    void start(std::string logname);
    void stop();

    bool getBlink();
};
