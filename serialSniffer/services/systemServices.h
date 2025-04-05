#ifndef __SYS_SERVICES__
#define __SYS_SERVICES__

#include "dto.h"

namespace NServices {

/**
 * @class SystemServices
 * @brief system services
*/
class SystemServices {
public:
    static void initSystem();
    static bool parseParams(NDto::SystemDto& systemDto, int argc, char* argv[]);
};

}

#endif