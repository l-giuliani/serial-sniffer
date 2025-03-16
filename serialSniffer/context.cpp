#include "context.h"

std::shared_ptr<Components::LogManager> Context::logManager = nullptr;

void Context::setLogManager(std::shared_ptr<Components::LogManager> logManager) {
    Context::logManager = logManager;
}

std::shared_ptr<Components::LogManager> Context::getLogManager() {
    return Context::logManager;
}