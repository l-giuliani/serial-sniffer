#include "systemServices.h"

#include <memory>
#include <string>
#include "logManager.h"
#include "context.h"
#include "appenders.h"

namespace NServices {

/**
 * @brief logger initialization
 * @return The Logger
*/
std::shared_ptr<NUtils::Logger> initLogger() {
    NUtils::NLogger::ConsolleAppenderDto appenderDto;
    appenderDto.setFormattedType(NUtils::NLogger::ConsolleAppender::FORMATTER_TYPE::HEX);

    std::shared_ptr<NUtils::Logger> logger = std::make_shared<NUtils::Logger>();
    logger->addAppender(NUtils::NLogger::Appender::Type::CONSOLE, &appenderDto);

    return logger;
}

/**
 * @brief System initialization
*/
void SystemServices::initSystem() {
    std::shared_ptr<NUtils::Logger> logger = initLogger();
    std::shared_ptr<Components::LogManager> logManager = std::make_shared<Components::LogManager>(logger);

    Context::setLogManager(logManager);
}

/**
 * @brief The program params parser
*/
bool SystemServices::parseParams(NDto::SystemDto& systemDto, int argc, char* argv[]) {
    if(argc < 2) {
        return false;
    }
    std::string serialPort = std::string(argv[1]);
    //TODO make some sort of validation
    systemDto.setSerialPort(serialPort);

    return true;
}

}