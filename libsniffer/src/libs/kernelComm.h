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

class KernelCommSubscriber {
public:
    virtual void onData() = 0;
};

/**
 * @class KernelComm
 * @brief Communication with kernel modules by generic netlink
 */
class KernelComm {
private:
    struct nl_sock* sock;
    bool connected;
public:
    KernelComm();
    bool initAndConnect();
    int registerToMulticastGroup(const char* family, const char* group);
    bool removeFromMulticastGroup(const char* family, const char* group);
    bool registerCallback(int (*on_receive)(struct nl_msg*, void*));
    void startListening();
    void recv();
    ~KernelComm();
};

#endif