#ifndef __LOGMANAGER_H__
#define __LOGMANAGER_H__

#include <future>
#include <memory>
#include <string>
#include "blocking_queue.h"
#include "logger.h"

#define STOP_LOG "LogManager_Stop"

namespace Components {

/**
 * @class LogManager
 * @brief The LogManager
*/
class LogManager {
private:
    NUtils::DataStructure::BlockingQueue<std::string> bq;
    std::future<void> fut;
    std::shared_ptr<NUtils::Logger> logger;
    bool active;

    void asyncAction();
public:
    LogManager(std::shared_ptr<NUtils::Logger> logger);
    void addLog(std::string log);
    void executeAsync();
    void stopAsync();
};

}

#endif