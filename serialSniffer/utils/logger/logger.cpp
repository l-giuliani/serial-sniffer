#include "logger.h"

#include "appenders.h"

namespace NUtils {

/**
 * @brief Add appender to Log
 * 
 * create concrete appender ad add it to appenders list
*/
void Logger::addAppender(NLogger::Appender::Type appenderType, NLogger::AppenderDto* appenderDto) {
    if(appenderType == NLogger::Appender::Type::CONSOLE) {
        this->appenders.push_back(std::make_shared<NLogger::ConsolleAppender>(appenderDto));
    }
}

/**
 * @brief performs log action
 * 
 * log using every appender
*/
void Logger::log(const std::string& log) {
    for (std::shared_ptr<NLogger::Appender> a : this->appenders) {
        a->log(log);
    }
}


}