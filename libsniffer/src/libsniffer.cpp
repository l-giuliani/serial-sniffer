#include "libsniffer.h"

#include <stdio.h>
#include <future>
#include "kernelComm.h"
#include "snifferData.h"
#include "helper.h"

void test() {
    printf("simple test\n");
    fflush(stdout);
}

/**
 * @brief initialize the sniffer
 * @param portName the serial port name
 * @return 0
*/
int initializeSniffer(const char* portName) {
    AsyncSerialSniffer& asyncSniffer = snifferData.getAsyncSniffer();

    asyncSniffer.setSerialPort(portName);

    return 0;
}

/**
 * @brief start sniffing
 * @param callback the callback called when data arrives
 * @return 0 OK, -1 error;
*/
int startAsyncSniffing(void(*callback)(uint8_t*, int)) {
    AsyncSerialSniffer& asyncSniffer = snifferData.getAsyncSniffer();
    
    bool res = asyncSniffer.init(callback);
    if(!res) {
        return -1;
    }

    asyncSniffer.start();

    return 0;
}

/**
 * @brief stop sniffing
 * @return 0
*/
int stopAsyncSniffing() {
    AsyncSerialSniffer& asyncSniffer = snifferData.getAsyncSniffer();

    asyncSniffer.stop();
    asyncSniffer.uninit();
    return 0;
}