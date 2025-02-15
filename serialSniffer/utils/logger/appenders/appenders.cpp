#include "appenders.h"

#include <iostream>

namespace NUtils {
namespace NLogger {

/**
 * @brief Set Formatter Type
*/
void ConsolleAppenderDto::setFormattedType(ConsolleAppender::FORMATTER_TYPE formatterType) {
    this->formatterType = formatterType;
}

/**
 * @brief Get Formatter Type
*/
ConsolleAppender::FORMATTER_TYPE ConsolleAppenderDto::getFormattedType() {
    return this->formatterType;
}

/**
 * @brief The ConsolleAppender constructor
 * 
 * creates concrete formatted if any 
*/
ConsolleAppender::ConsolleAppender(AppenderDto* appenderDto) {
    this->strFormatter = nullptr;
    ConsolleAppenderDto* cad = static_cast<ConsolleAppenderDto*>(appenderDto);
    if(cad->getFormattedType() == ConsolleAppender::FORMATTER_TYPE::HEX) {
        this->strFormatter = std::make_shared<NUtils::HexFormatter>();
    }
}

/**
 * @brief performs the log on console after formatter transformation
*/
void ConsolleAppender::log(const std::string& log) {
    std::string outlog = log;
    if(this->strFormatter != nullptr) {
        outlog = this->strFormatter->format(log);
    }
    std::cout << outlog << std::endl;
}

}
}