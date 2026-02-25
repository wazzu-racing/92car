#pragma once
#include <TeensyThreads.h>
#include <Arduino.h>

ThreadWrap(Serial, SerialXtra);
#define Serial ThreadClone(SerialXtra)

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