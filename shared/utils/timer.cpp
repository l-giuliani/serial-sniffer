#include "timer.h"

#include <chrono>
#include <thread>

CustomTimer::CustomTimer(CustomAction& customAction, int timeout_ms) {
    this->customAction = customAction;
    this->timeout_ms = timeout_ms; 
    this->execute = false;
}

void CustomTimer::timerFun() {
    while(true) {
        {
            std::lock_guard<std::mutex> lock(this->mx);
            if(!this->execute) {
                return;
            }
            auto now = std::chrono::system_clock::now();  
            if(now > this->t) {
                this->execute = false;
                this->customAction.action();
            }   
        }   
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void CustomTimer::start() {
    std::lock_guard<std::mutex> lock(this->mx);
    if(this->execute) {
        return;
    }
    this->t = std::chrono::system_clock::now();
    this->execute = true;
    this->fut = std::async(CustomTimer::timerFun, this);
}

void CustomTimer::stop() {
    {
        std::lock_guard<std::mutex> lock(this->mx);
        if(!this->execute) {
            return;
        }
        this->execute = false;
    }
    this->fut.get();
}

void CustomTimer::refresh() {
    std::lock_guard<std::mutex> lock(this->mx);
    this->t = std::chrono::system_clock::now();
}
