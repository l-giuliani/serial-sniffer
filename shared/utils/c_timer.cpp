/**
 * @file c_timer.cpp
 * @brief Custom Timer
 *
 * This file contains the implementation of classes used for custom Timer
 * The timer can be stopped or refreshed before a custom action
 *
 * @author Lorenzo Giuliani
 */

#include "c_timer.h"

#include <chrono>
#include <thread>

CustomTimer::CustomTimer(CustomAction& customAction, int timeout_ms) : customAction(customAction) {
    this->timeout_ms = timeout_ms; 
    this->execute = false;
}

/**
 * @brief The function for time check
*/
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

/**
 * @brief The start function
*/
void CustomTimer::start() {
    std::lock_guard<std::mutex> lock(this->mx);
    if(this->execute) {
        return;
    }
    this->t = std::chrono::system_clock::now() + std::chrono::milliseconds(timeout_ms);
    this->execute = true;
    this->fut = std::async(&CustomTimer::timerFun, this);
}

/**
 * @brief The stop function
*/
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

/**
 * @brief The refresh action
*/
void CustomTimer::refresh() {
    std::lock_guard<std::mutex> lock(this->mx);
    this->t = std::chrono::system_clock::now();
}

CustomTimer::~CustomTimer() {
    this->stop(); 
}
