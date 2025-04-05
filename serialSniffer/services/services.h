#ifndef __SERVICES__
#define __SERVICES__

#include "dto.h"

namespace NServices {

/**
 * @class Services
 * @brief The 
*/
class Services {
private:
    static bool asyncSniffing;
public:
    static void sniffStart(NDto::SystemDto& systemDto);
    static void sniffStop();
};

}

#endif