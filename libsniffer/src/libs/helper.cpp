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

#include <iostream>
#include <thread>
#include <chrono>

KernelCommListener::KernelCommListener() {
    this->callback = nullptr;
}

/**
 * @brief Set the callback for data arrived from the kernel
 * 
 * @param callback The callback to be called on data
 */
void KernelCommListener::setCallback(std::function<void(uint8_t*, int)> callback) {
    this->callback = callback;
}

/**
 * @brief Set the callback for data arrived from the kernel
 * 
 * @param kmd The data arrived from Kernel Netlink
 */
void KernelCommListener::onData(const KernelMulticastData& kmd) {
    if(this->callback == nullptr) {
        return;
    }
    if(kmd.getCommand() != SNIFF_CMD_SNIFFED) {
        return;
    }
    const KernelMulticastData::KernelMulticastEl& kme = kmd.getAttr(SNIFF_ATTR_BUFFER-1);
    this->callback((uint8_t*)kme.data, kme.len);
}

/**
 * @brief AsyncSniffing constructor
*/
AsyncSniffer::AsyncSniffer() {
    this->active = false;
    this->initialized = false;
}

/**
 * @brief initialize KernelComm in order to riceive data via Netlink
 * @param callback the callback called when data arrive from kernel
 * @return true if initialized correctly, false otherwise
*/
bool AsyncSniffer::init(std::function<void(uint8_t*, int)> callback) {
    if(this->initialized) {
        return true;
    }
    kcl.setCallback(callback);
    kc.subscribe(&kcl);
    bool res = kc.initAndConnect();
    if(!res) {
        return false;
    }
    kc.disableSeqCheck();
    res = kc.setSocketTimeout(1000);
    if(!res) {
        return false;
    }
    int gid = kc.registerToMulticastGroup(GENL_FAMILY_NAME, GENL_MULTICAST_GROUP);
    if(gid == -1) {
        return false;
    }
    res = kc.registerCallback();
    if(!res) {
        return false;
    }
    
    this->initialized = true;
    return true;
}

/**
 * @brief async netlink read 
*/
void AsyncSniffer::executeAsync() {
    bool res;
    while(this->active) {
        res = kc.recv();
        if(!res) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

/**
 * @brief start receiving data from Kernel
*/
void AsyncSniffer::start() {
    if(!this->initialized) {
        return;
    }
    if(this->active) {
        return;
    }
    this->active = true;
    this->future = std::async(&AsyncSniffer::executeAsync, this);
}

/**
 * @brief stop receiving data from Kernel
*/
void AsyncSniffer::stop() {
    if(!this->initialized) {
        return;
    }
    if(!this->active) {
        return;
    }
    this->active = false;
    this->future.get();
}

/**
 * @brief uninitialize AsyncSniffer
*/
void AsyncSniffer::uninit() {
    this->stop();
    kc.disconnect();
    this->initialized = false;
}