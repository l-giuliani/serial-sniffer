#ifndef __CTIMER__
#define __CTIMER__

#include <chrono>
#include <future>
#include <mutex>

class CustomAction {
public:
    virtual void action() = 0;
};

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