/**
 * @file helper.cpp
 * @brief Helpers for communication with kernel via Netlink
 *
 * This file contains the implementation of classes used for helpers
 * in the communication with the kernel via netlink
 *
 * @author Lorenzo Giuliani
 */

#include "helper.h"
#include "netlink_data.h"

/**
 * @brief Set the callback for data arrived from the kernel
 * 
 * @param callback The callback to be called on data
 */
void KernelCommListener::setCallback(std::function<void(uint8_t*, int*)> callback) {
    this->callback = callback;
}

/**
 * @brief Set the callback for data arrived from the kernel
 * 
 * @param kmd The data arrived from Kernel Netlink
 */
void KernelCommListener::onData(const KernelMulticastData& kmd) {
    if(kmd.getCommand() != SNIFF_CMD_SNIFFED) {
        return;
    }
    kmd.getAttr(SNIFF_ATTR_BUFFER);
    //todo organize data and send via callback
}