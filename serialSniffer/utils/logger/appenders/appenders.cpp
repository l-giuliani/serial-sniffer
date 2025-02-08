#include "appenders.h"

#include <iostream>

namespace NUtils {
namespace NLogger {

void ConsolleAppenderDto::setFormattedType(ConsolleAppender::FORMATTER_TYPE formatterType) {
    this->formatterType = formatterType;
}

ConsolleAppender::FORMATTER_TYPE ConsolleAppenderDto::getFormattedType() {
    return this->formatterType;
}

ConsolleAppender::ConsolleAppender(AppenderDto* appenderDto) {
    this->strFormatter = nullptr;
    ConsolleAppenderDto* cad = static_cast<ConsolleAppenderDto*>(appenderDto);
    if(cad->getFormattedType() == ConsolleAppender::FORMATTER_TYPE::CONSOLLE) {
        this->strFormatter = std::make_shared<NUtils::HexFormatter>();
    }
}

void ConsolleAppender::log(const std::string& log) {
    std::string outlog = log;
    if(this->strFormatter != nullptr) {
        outlog = this->strFormatter->format(log);
    }
    std::cout << outlog << std::endl;
    
}

}
}