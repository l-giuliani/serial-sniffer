#ifndef __APPENDERS_H__
#define __APPENDERS_H__

#include <memory>
#include "logger.h"
#include "utils.h"


namespace NUtils {
namespace NLogger {

/**
 * @class ConsolleAppender
 * @brief Log to console by log formatter
*/
class ConsolleAppender : public Appender {
private:
    std::shared_ptr<NUtils::StrFormatter> strFormatter;
public:
    typedef enum {
        NONE,
        HEX
    } FORMATTER_TYPE;

    ConsolleAppender(AppenderDto* appenderDto);
    void log(const std::string& log);
};

/**
 * @class ConsolleAppenderDto
 * @brief The appender Dto passed in constructor
*/
class ConsolleAppenderDto : public AppenderDto {
private:
    ConsolleAppender::FORMATTER_TYPE formatterType;
public:
    void setFormattedType(ConsolleAppender::FORMATTER_TYPE formatterType);
    ConsolleAppender::FORMATTER_TYPE getFormattedType();
};

}
}

#endif