#include "ThreadedManager.hpp"

void ThreadedManager::runner(void* arg) {
    Serial.println("runner...");
    ThreadedManager* self = static_cast<ThreadedManager*>(arg);
    while (self->KEEP_RUNNING) {
        self->loop();
        self->blink = !(self->blink);
    }
}

void ThreadedManager::start(std::string logname) {
    this->logname_ = logname;
    this->KEEP_RUNNING = true;
    this->setup();
    threads.addThread(runner, this);
}

void ThreadedManager::log(std::string data) {
    Serial.print("[");
    Serial.print(this->logname_.c_str());
    Serial.print("]\t");
    Serial.println(data.c_str());
}

void ThreadedManager::stop() {
    this->KEEP_RUNNING = false;
}

bool ThreadedManager::getBlink()
{
    return this->blink;
}
