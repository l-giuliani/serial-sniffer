/**
 * @file helper.h
 * @brief Helpers for communication with kernel via Netlink
 *
 * This file contains the declaration of classes used for helpers
 * 
 *
 * @author Lorenzo Giuliani
 */

#include "kernelComm.h"

#include <functional>
#include <stdint.h>

/**
 * @class KernelCommListener
 * @brief Listener of Kernel Messages
 */
class KernelCommListener : public KernelCommSubscriber {
private:
    std::function<void(uint8_t*, int*)> callback;
public:
    void setCallback(std::function<void(uint8_t*, int*)> callback);
    void onData(const KernelMulticastData& kmd);
};