#include "services.h"

#include <iostream>
#include "libsniffer.h"
#include "context.h"

namespace NServices {

bool Services::asyncSniffing = false;

void serialCallback(uint8_t* buffer, int len) {
    std::string data((char*)buffer, len);
    Context::getLogManager()->addLog(data);
}

void Services::sniffStart(NDto::SystemDto& systemDto) {
    if(Services::asyncSniffing) {
        return;
    }
    Context::getLogManager()->executeAsync();
    initializeSniffer(systemDto.getSerialPort().c_str());
    int res = startAsyncSniffing(serialCallback);
    if(res == -1) {
        std::cout << "Error initializing async sniffer" << std::endl;
        return;
    }

    Services::asyncSniffing = true;
}

void Services::sniffStop() {
    if(!Services::asyncSniffing) {
        return;
    }
    stopAsyncSniffing();
    Context::getLogManager()->stopAsync();

    Services::asyncSniffing = false;
}

}