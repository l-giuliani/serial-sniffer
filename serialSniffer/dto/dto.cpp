    #include "dto.h"

namespace NDto {
    
    std::string SystemDto::getSerialPort() {
        return this->serialPort;
    }

    void SystemDto::setSerialPort(const std::string& serialPort) {
        this->serialPort = serialPort;
    }

}