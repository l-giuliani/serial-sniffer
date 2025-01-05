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
 * @class KernelComm
 * @brief Communication with kernel modules by generic netlink
 */
class KernelComm {
private:
    struct nl_sock* sock;
    bool connected;
    std::vector<KernelCommSubscriber*> subscribers;

    void notifySubscribers(const KernelMulticastData& kmd);
    static int onReceive(struct nl_msg *msg, void *arg);
public:
    KernelComm();
    bool initAndConnect();
    int registerToMulticastGroup(const char* family, const char* group);
    bool removeFromMulticastGroup(const char* family, const char* group);
    bool registerCallback();
    void startListening();
    void recv();
    ~KernelComm();
};

#endif