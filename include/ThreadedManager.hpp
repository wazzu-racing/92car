#pragma once
#include <TeensyThreads.h>
#include <Arduino.h>

// #ifndef WRAPPED_SERIAL_
// #define WRAPPED_SERIAL_
// ThreadWrap(Serial, SerialXtra);
// // #define Serial ThreadClone(SerialXtra)
// #endif

class ThreadedManager {
    volatile bool KEEP_RUNNING;
    volatile bool blink = false;

    virtual void loop() = 0;
    virtual void setup() = 0;

    static void runner(void* arg);
    
public:
    void start();
    void stop();

    bool getBlink();
};