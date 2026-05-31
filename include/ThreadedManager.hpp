#pragma once
#include <TeensyThreads.h>
#include <Arduino.h>
#include <string>

class ThreadedManager {
    std::string logname_;

    volatile bool KEEP_RUNNING;
    volatile bool blink = false;
    uint8_t managerId = 0;

    virtual void loop() = 0;
    virtual void setup() = 0;

    static void runner(void* arg);

protected:
    void log(std::string data);
    void warning(std::string data);

    // Hook for log line forwarding to SD (set by DataloggerManager at runtime)
    static void (*logToSdHook)(const std::string&);


public:
    void start(std::string logname);
    void stop();

    bool getBlink();
};
