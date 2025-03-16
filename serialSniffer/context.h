#ifndef __CONTEXT__
#define __CONTEXT__

#include <memory>
#include "logManager.h"

class Context {
public:
    static std::shared_ptr<Components::LogManager> logManager;

    static std::shared_ptr<Components::LogManager> getLogManager();
    static void setLogManager(std::shared_ptr<Components::LogManager> logManager);
};

#endif