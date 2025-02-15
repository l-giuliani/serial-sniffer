#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <vector>
#include <memory>

namespace NUtils {
namespace NLogger {

/**
 * @class Appender
 * @brief interface for Log Appenders
*/
class Appender {
public:
    typedef enum {
        CONSOLE
    } Type;
    virtual void log(const std::string& log) = 0;
};

/**
 * @class AppenderDto
 * @brief interface for Log AppenderDto
*/
class AppenderDto {

};

}

/**
 * @class Logger
 * @brief Logger with appenders
*/
class Logger {
private:
    std::vector<std::shared_ptr<NLogger::Appender>> appenders;
public:
    void addAppender(NLogger::Appender::Type appenderType, NLogger::AppenderDto* appenderDto);
    void log(const std::string& log);
};

}

#endif