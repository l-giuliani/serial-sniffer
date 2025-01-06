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
    return 0;
}

/**
 * @brief start sniffing
 * @param callback the callback called when data arrives
 * @return 0;
*/
int startAsyncSniffing(void(*callback)(uint8_t*, int)) {
    AsyncSniffer& asyncSniffer = snifferData.getAsyncSniffer();
    
    asyncSniffer.init(callback);
    asyncSniffer.start();

    return 0;
}

/**
 * @brief stop sniffing
 * @return 0
*/
int stopAsyncSniffing() {
    AsyncSniffer& asyncSniffer = snifferData.getAsyncSniffer();

    asyncSniffer.stop();
    return 0;
}