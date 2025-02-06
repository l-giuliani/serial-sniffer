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

#include <chrono>
#include <functional>
#include <future>
#include <stdint.h>
#include <string>

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
protected:
    bool active;
    bool initialized;
    std::future<void> future;
    KernelCommListener kcl;
    KernelComm kc;

    uint32_t keepAliveTmo;
    std::chrono::time_point<std::chrono::system_clock> lastKeepAlive;
    DataToSend keepAliveData;

    void executeAsync();
public:
    AsyncSniffer();
    void setKeepAliveTmo(const uint32_t& keepAliveTmo);
    bool init(std::function<void(uint8_t*, int)> callback);
    void start();
    void stop();
    void uninit();
};

/**
 * @class serial sniffer
*/
class AsyncSerialSniffer : public AsyncSniffer {
private:
    std::string serialPort;
    bool testMode;
public:
    AsyncSerialSniffer(std::string serialPort);
    AsyncSerialSniffer();
    void setSerialPort(std::string serialPort);
    void setTestMode(bool testMode);
};

#endif