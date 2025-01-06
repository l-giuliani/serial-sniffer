#ifndef __LIBSNIFFER__
#define __LIBSNIFFER__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void test();

int initializeSniffer(const char* portName);
int startAsyncSniffing(void(*callback)(uint8_t*, int));
int stopAsyncSniffing();

#ifdef __cplusplus
}
#endif

#endif