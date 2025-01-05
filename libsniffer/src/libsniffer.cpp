#include "libsniffer.h"

#include "kernelComm.h"
#include <stdio.h>

void test() {
    printf("simple test\n");
    fflush(stdout);
}

int initializeSniffer(const char* portName) {
    return 0;
}

int startAsyncSniffing(void(*callback)(uint8_t*, int)) {
    return 0;
}