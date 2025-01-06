/**
 * @file helper.h
 * @brief Helpers for communication with kernel via Netlink
 *
 * This file contains the declaration of classes used for helpers
 * 
 *
 * @author Lorenzo Giuliani
 */

#ifndef __SNIFFER_HELPER__
#define __SNIFFER_HELPER__

#include "kernelComm.h"

#include <functional>
#include <future>
#include <stdint.h>

/**
 * @class KernelCommListener
 * @brief Listener of Kernel Messages
 */
class KernelCommListener : public KernelCommSubscriber {
private:
    std::function<void(uint8_t*, int)> callback;
public:
    KernelCommListener();
    void setCallback(std::function<void(uint8_t*, int)> callback);
    void onData(const KernelMulticastData& kmd);
};

/**
 * @class AsyncSniffing
 * @brief Async Sniffing reader
*/
class AsyncSniffer {
private:
    bool active;
    bool initialized;
    std::future<void> future;
    KernelCommListener kcl;
    KernelComm kc;

    void executeAsync();
public:
    AsyncSniffer();
    void init(std::function<void(uint8_t*, int)> callback);
    void start();
    void stop();
};

#endif