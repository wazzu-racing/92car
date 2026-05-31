#include "ThreadedManager.hpp"

void ThreadedManager::runner(void* arg) {
    Serial.println("runner...");
    ThreadedManager* self = static_cast<ThreadedManager*>(arg);
    while (self->KEEP_RUNNING) {
        // CrashReport.breadcrumb(self->managerId, millis());
        self->loop();
        self->blink = !(self->blink);
        threads.yield(); // IDK if this is really needed, but it works with it
    }
}

void ThreadedManager::start(std::string logname) {
    // static uint8_t nextId = 1;
    // this->managerId = nextId++;
    this->logname_ = logname;
    this->KEEP_RUNNING = true;
    this->setup();
    threads.addThread(runner, this);
}

// void (*ThreadedManager::logToSdHook)(const std::string&) = nullptr;

void ThreadedManager::log(std::string data) {
    Serial.print("[");
    Serial.print(this->logname_.c_str());
    Serial.print("]\t");
    Serial.println(data.c_str());
    // Forward log to SD file, if a hook was set (by DataloggerManager)
    // if (ThreadedManager::logToSdHook) {
    //     ThreadedManager::logToSdHook("[" + this->logname_ + "]\t" + data);
    // }
}

void ThreadedManager::warning(std::string data) {
    Serial.print("[");
    Serial.print(this->logname_.c_str());
    Serial.print("]\t");
    Serial.print("\x1b[33m"); // ANSI yellow
    Serial.print(data.c_str());
    Serial.print("\x1b[0m\n"); // reset color with newline
}

void ThreadedManager::stop() {
    this->KEEP_RUNNING = false;
}

bool ThreadedManager::getBlink()
{
    return this->blink;
}
