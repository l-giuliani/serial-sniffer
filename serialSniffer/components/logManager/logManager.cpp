#include "logManager.h"

namespace Components {

/**
 * @brief The LogManager constructor
 * @param logger The logged used to log
*/
LogManager::LogManager(std::shared_ptr<NUtils::Logger> logger) {
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
    fut = std::async(&LogManager::asyncAction, this);
}

/**
 * @brief The log async action
 * Thread remains blocked until something to log becomes available
*/
void LogManager::asyncAction() {
    std::string log = this->bq.pop();
    logger->log(log);
}

}