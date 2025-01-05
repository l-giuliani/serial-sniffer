#ifndef __LIBSNIFFER__
#define __LIBSNIFFER__

#ifdef __cplusplus
extern "C" {
#endif

void test();

int initializeSniffer(const char* portName);
int startAsyncSniffing(void(*callback)(uint8_t*, int));

#ifdef __cplusplus
}
#endif

#endif