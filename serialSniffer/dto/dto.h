#ifndef __DTO_H__
#define __DTO_H__

#include <string>
#include "serialLibs.h"

namespace NDto {

/**
 * @class SystemDto
 * Dto of system params
*/
class SystemDto {
private:
    std::string serialPort;
public:
    std::string getSerialPort();
    void setSerialPort(const std::string& serialPort);
};

}

#endif