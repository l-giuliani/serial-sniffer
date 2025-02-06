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

#define DEF_KEEP_ALIVE_TMO  300

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
    this->keepAliveTmo = DEF_KEEP_ALIVE_TMO;
}

/**
 * @brief Set keep Alive Tmo
 * @param keepAliveTmo The keep Alive Tmo
*/
void AsyncSniffer::setKeepAliveTmo(const uint32_t& keepAliveTmo) {
    this->keepAliveTmo = keepAliveTmo;
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
        auto now = std::chrono::system_clock::now();
        res = kc.recv();
        if(now > this->lastKeepAlive) {
            std::cout << "sto inviando il keep alive\n"<<std::fflush;
            this->keepAliveData.reset();
            this->kc.sendData(GENL_FAMILY_NAME, SNIFF_CMD_KEEP_ALIVE, &this->keepAliveData);
            std::cout << "ho finito di inviare il keep alive\n"<<std::fflush;
            this->lastKeepAlive = now + std::chrono::seconds(this->keepAliveTmo);
        } 
        
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
    //this->lastKeepAlive = std::chrono::system_clock::now() + std::chrono::seconds(this->keepAliveTmo);
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

/**
 * @brief AsyncSerialSniffer constructor
*/
AsyncSerialSniffer::AsyncSerialSniffer(std::string serialPort) : AsyncSniffer() {
    this->testMode = false;
    this->serialPort = serialPort;
    this->keepAliveData.add(SNIFF_ATTR_SER_DEVICE, serialPort);
}

/**
 * @brief AsyncSerialSniffer constructor
*/
AsyncSerialSniffer::AsyncSerialSniffer() : AsyncSniffer() {
    this->testMode = false;
    this->serialPort = "/dev/ttyS1";
    this->keepAliveData.add(SNIFF_ATTR_SER_DEVICE, this->serialPort);
}

/**
 * @brief The serialport setter
 * @param serialPort serial port name
*/
void AsyncSerialSniffer::setSerialPort(std::string serialPort) {
   this->serialPort = serialPort;
}

/**
 * @brief set test mode
*/
void AsyncSerialSniffer::setTestMode(bool testMode) {
    if(testMode) {
        if(!this->testMode) {
            this->testMode = true;
            this->keepAliveData.add(SNIFF_ATTR_EXEC_TEST, this->serialPort);
        }
    }
}