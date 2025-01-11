/**
 * @file kernelComm.h
 * @brief Communication with kernel module
 *
 * This file contains the implementation of methods 
 * used for communicating with the kernel
 * 
 * @author Lorenzo Giuliani
 */

#include "kernelComm.h"

#include "netlink_data.h"

/**
 * @brief set command in kernel multicast data
 * 
 * @param command the command
 */
void KernelMulticastData::setCommand(int command) {
    this->command = command;
}
/**
 * @brief get command
 * @return The command
 */
int KernelMulticastData::getCommand() const {
    return this->command;
}
/**
 * @brief add attribute in kernel multicast data
 * @param attr couple data and len relatives to an attribute
 */
void KernelMulticastData::addAttr(KernelMulticastData::KernelMulticastEl attr) {
    this->attrs.push_back(attr);
}

/**
 * @brief get attribute from kernel multicast data
 * @param attr id of attribute requested
 */
const KernelMulticastData::KernelMulticastEl& KernelMulticastData::getAttr(int attr) const {
    return this->attrs.at(attr);
}

/**
 * @brief KernelComm contructor
 */
KernelComm::KernelComm() {
    this->sock = nullptr;
    this->connected = false;
}

/**
 * @brief Resolve Netlink family
 * @param family The family name
 * @return The family id or error code
 */
int KernelComm::resolveFamily(const char* family) {
    return genl_ctrl_resolve(sock, family);
}

/**
 * @brief Notify subscribers
 * @param kmd The Kernel Multicast Data passed to subscribers
 */
void KernelComm::notifySubscribers(const KernelMulticastData& kmd) {
    std::vector<KernelCommSubscriber*>::iterator it;
    for(it = this->subscribers.begin(); it != this->subscribers.end(); it++) {
        (*it)->onData(kmd);
    }
}

/**
 * @brief Subscribe in order to get data from Netlink
 * @param subscriber The subscriber
*/
void KernelComm::subscribe(KernelCommSubscriber* subscriber) {
    this->subscribers.push_back(subscriber);
}

/**
 * @brief Subscribe in order to get data from Netlink get
 * @return The subscribers
*/
std::vector<KernelCommSubscriber*>& KernelComm::getSubscribers() {
    return this->subscribers;
}

/**
 * @brief Initialize and connect netlink socket.
 * @return true if connect operation is Ok
 */
bool KernelComm::initAndConnect() {
    this->sock = nl_socket_alloc();
    if (!this->sock) {
        return false;
    }

    if (genl_connect(this->sock) < 0) {
        nl_socket_free(this->sock);
        this->sock = nullptr;
        return false;
    }
    this->connected = true;
    return true;
}

/**
 * @brief Callback data from netlink
 * 
 * @param msg msg struct netlink
 * @param arg custom pointer passed to callback
 * 
 * @return NL_OK
 */
int KernelComm::onReceive(struct nl_msg *msg, void *arg) {
    KernelComm* self = static_cast<KernelComm*>(arg);

    struct nlmsghdr *nlh = nlmsg_hdr(msg);
    struct genlmsghdr *genl_hdr = static_cast<struct genlmsghdr *>(nlmsg_data(nlh));

    KernelMulticastData kmd;
    kmd.setCommand(genl_hdr->cmd);

    struct nlattr *attrs[256];
    genlmsg_parse(nlh, 0, attrs, 256 - 1, NULL);
    bool endAttr = false;
    for(int i=1; i<256 && (!endAttr); i++){
        if (attrs[i]) {
            KernelMulticastData::KernelMulticastEl kme;
            kme.data = nla_data(attrs[1]);
            kme.len = nla_len(attrs[i]);
            kmd.addAttr(kme);
        } else {
            endAttr = true;
        }
    }
    self->notifySubscribers(kmd);

    return NL_OK;
}

/**
 * @brief Register to multicast group specifying family and group.
 * 
 * @param family netlink family name
 * @param group multicast group name
 * 
 * @return -1 in case of error, the groupId otherwise
 */
int KernelComm::registerToMulticastGroup(const char* family, const char* group) {
    if(!this->connected) {
        return -1;
    }
    int familyId = resolveFamily(family);
    if (familyId < 0) {
        return familyId;
    }

    int groupId = genl_ctrl_resolve_grp(sock, family, group);
    if (groupId < 0) {
        return -3;
    }
    int ret = nl_socket_add_membership(sock, groupId);
    if (ret < 0) {
        //nl_socket_free(sock);
        //sock = nullptr;
        return -4;
    }

    return groupId;
}

/**
 * @brief Unregister from multicast group specifying family and group.
 * 
 * @param family netlink family name
 * @param group multicast group name
 * 
 * @return false in case of error, true otherwise
 */
bool KernelComm::removeFromMulticastGroup(const char* family, const char* group) {
    if(!this->connected) {
        return false;
    }
    int familyId = resolveFamily(family);
    if (familyId < 0) {
        return false;
    }

    int groupId = genl_ctrl_resolve_grp(sock, family, group);
    if (groupId < 0) {
        return false;
    }

    int ret = nl_socket_drop_membership(sock, groupId);
    if (ret < 0) {
        return false;
    }

    return true;
}

/**
 * @brief Unregister from multicast group specifying family and group.
 * 
 * @param family netlink family name
 * @param group multicast group name
 * 
 * @return false in case of error, true otherwise
 */
bool KernelComm::registerCallback() {
    if(!this->connected) {
        return false;
    }
    int ret = nl_socket_modify_cb(this->sock, NL_CB_VALID, NL_CB_CUSTOM, KernelComm::onReceive, this);
    return (ret == 0);
}

void KernelComm::startListening() {
    if(!this->connected) {
        return;
    }

    
}

/**
 * @brief receiving from netlink
*/
void KernelComm::recv() {
    nl_recvmsgs_default(this->sock);
}

int KernelComm::sendData(const char* family, int command, uint8_t* data, int len) {
    struct nl_msg *msg = NULL;
    int res;

    int familyId = resolveFamily(family);
    if (familyId < 0) {
        return -1;
    }

    msg = nlmsg_alloc();
    if (!msg) {
        return -2;
    }

    if (!genlmsg_put(msg, NL_AUTO_PORT, NL_AUTO_SEQ, familyId, 0, 0, SNIFF_CMD_SEND_TEST, 1)) {
        nlmsg_free(msg);
        return -3;
    }

    res = nl_send_auto(sock, msg);
    nlmsg_free(msg);

    return res;
}

/**
 * @brief get connection status
 * @return connection status
*/
bool KernelComm::getConnected() {
    return this->connected;
}

/**
 * @brief Disconnect netlink
 * @return true if disconnect op is OK, false otherwise
*/
bool KernelComm::disconnect() {
    if(this->sock != nullptr) {
        nl_socket_free(this->sock);
        this->sock = nullptr;
        this->connected = false;
    }
    return true;
}

KernelComm::~KernelComm() {
    this->disconnect();
}