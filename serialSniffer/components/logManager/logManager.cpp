#include "logManager.h"

namespace Components {

/**
 * @brief The LogManager constructor
 * @param logger The logged used to log
*/
LogManager::LogManager(std::shared_ptr<NUtils::Logger> logger) {
    this->active = false;
    this->logger = logger;
}

/**
 * @brief Use The log in order to log informations
 * @param log The log
*/
void LogManager::addLog(std::string log) {
    this->bq.push(log);
}

/**
 * @brief start async log execution
*/
void LogManager::executeAsync() {
    if(this->active) {
        return;
    }
    this->active = true;
    fut = std::async(&LogManager::asyncAction, this);
}

/**
 * @brief The log async action
 * Thread remains blocked until something to log becomes available
*/
void LogManager::asyncAction() {
    while(this->active) {
        std::string log = this->bq.pop();
        if(log.compare(STOP_LOG) == 0) {
            continue;
        }
        logger->log(log);
    }
}

/**
 * @brief stop async log execution
*/
void LogManager::stopAsync() {
    if(!this->active) {
        return;
    }
    this->active = false;
    this->bq.push(STOP_LOG);
    this->fut.get();
}

}