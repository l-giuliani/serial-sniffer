/**
 * @file c_timer.h
 * @brief Custom Timer
 *
 * This file contains the declaration of classes used for custom Timer
 * 
 *
 * @author Lorenzo Giuliani
 */

#ifndef __CTIMER__
#define __CTIMER__

#include <chrono>
#include <future>
#include <mutex>

/**
 * @class CustomAction
 * @brief The interface of custom Action executed at the end of the timer
*/
class CustomAction {
public:
    virtual void action() = 0;
};

/**
 * @class CustomTimer
 * @brief The Custom Timer
 * 
 * It execute an action at the end of the period specified
 * It can be refreshed and stopped before action execute
*/
class CustomTimer {
private:
    CustomAction& customAction;
    int timeout_ms;
    bool execute;
    std::future<void> fut;
    std::mutex mx;
    std::chrono::time_point<std::chrono::system_clock> t;
    void timerFun();
public:
    CustomTimer(CustomAction& customAction, int timeout_ms);
    void start();
    void stop();
    void refresh();
    ~CustomTimer();
};

#endif