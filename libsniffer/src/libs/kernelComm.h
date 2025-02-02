/**
 * @file kernelComm.h
 * @brief Communication with kernel module
 *
 * This file contains the declaration of classes used for communicating with the kernel
 * 
 *
 * @author Lorenzo Giuliani
 */

#ifndef __KERNEL_COMM__
#define __KERNEL_COMM__

#include <vector>
#include <cstdint>
#include <variant>
#include <string>
#include <memory>

#include "netlink/netlink.h"
#include "netlink/genl/genl.h"
#include "netlink/genl/family.h"
#include "netlink/genl/ctrl.h"
#include "netlink/msg.h"
#include "netlink/attr.h"

/**
 * @class KernelMulticastData
 * @brief Data passed to subscribers
 */
class KernelMulticastData {
public:
    /**
     * @struct KernelMulticastEl
     * @brief Data arrived from netlink
     */
    typedef struct {
        void* data;
        int len;
    } KernelMulticastEl;

    void setCommand(int command);
    int getCommand() const;
    void addAttr(KernelMulticastData::KernelMulticastEl attr);
    const KernelMulticastData::KernelMulticastEl& getAttr(int attr) const;
    void printAttrs() const;
private:
    int command;
    std::vector<KernelMulticastData::KernelMulticastEl> attrs;
};

/**
 * @class KernelCommSubscriber
 * @brief Subscriber for KernelComm
 */
class KernelCommSubscriber {
public:
    virtual void onData(const KernelMulticastData& kmd) = 0;
};

/**
 * @class DataDto
 * @brief data to send over netlink
*/
class DataDto {
public:
    using DataVariant = std::variant<int, std::string>;
    DataDto(int attribute, const DataVariant& dataVariant);
    int getAttribute();
    DataVariant& getData();
private:
    DataVariant data;
    int attribute;
};

/**
 * @class DataToSend
*/
class DataToSend {
private:
    std::vector<std::shared_ptr<DataDto>> data;
    std::vector<std::shared_ptr<DataDto>>::iterator it;
public:
    DataToSend();
    void add(int attribute, const DataDto::DataVariant& datavariant);
    void reset();
    std::shared_ptr<DataDto> next();
};

/**
 * @class KernelComm
 * @brief Communication with kernel modules by generic netlink
 */
class KernelComm {
private:
    struct nl_sock* sock;
    bool connected;
    std::vector<KernelCommSubscriber*> subscribers;

    int resolveFamily(const char* family);
    void notifySubscribers(const KernelMulticastData& kmd);
    static int onReceive(struct nl_msg *msg, void *arg);
public:
    KernelComm();
    void subscribe(KernelCommSubscriber* subscriber);
    std::vector<KernelCommSubscriber*>& getSubscribers();
    bool initAndConnect();
    void disableSeqCheck();
    bool setSocketTimeout(int timeout_ms);
    int registerToMulticastGroup(const char* family, const char* group);
    bool removeFromMulticastGroup(const char* family, const char* group);
    bool registerCallback();
    void startListening();
    bool recv();
    int sendData(const char* family, int command, DataToSend* data);
    bool getConnected();
    bool disconnect();
    ~KernelComm();
};

#endif