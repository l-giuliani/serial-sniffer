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
#include <iostream>
#include <string>
#include <time.h>
#include <variant>

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

void KernelMulticastData::printAttrs() const {
    for(const KernelMulticastData::KernelMulticastEl& el : this->attrs) {
        if(el.data) {
            std::cout << "len: " << el.len << "\n" << std::fflush;
            std::cout << "attr: " << el.data << "\n" << std::fflush;
        } else {
            std::cout << "nulla da fare\n" << std::fflush;
        }
    }
}

/**
 * @brief DataDto constructor
 * @param attribute netlink attribute
 * @param dataVariant data
*/
DataDto::DataDto(int attribute, const DataVariant& dataVariant) {
    this->attribute = attribute;
    this->data = dataVariant;
}

int DataDto::getAttribute() {
    return this->attribute;
}

DataDto::DataVariant& DataDto::getData() {
    return this->data;
}

DataToSend::DataToSend() {
    this->reset();
}

void DataToSend::add(int attribute, const DataDto::DataVariant& datavariant) {
    std::shared_ptr<DataDto> data = std::make_shared<DataDto>(attribute, datavariant);
    this->data.push_back(data);
}

void DataToSend::reset() {
    this->it = data.begin();
}

std::shared_ptr<DataDto> DataToSend::next() {
    std::shared_ptr<DataDto> value;
    if(this->it == data.end()) {
        return nullptr;
    }
    value = *this->it;
    this->it++;
    return value;
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
 * @brief Netlink Socket timeout
 * @param timeout_ms The timeout in millisecond
 * @return true on success, false otherwise
*/
bool KernelComm::setSocketTimeout(int timeout_ms) {
    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;
    int fd = nl_socket_get_fd(sock);

    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        return false;
    }
    return true;
}

/**
 * @brief disable Seq Check in Netlink communication
 * 
 * This should be called for broadcast comm
*/
void KernelComm::disableSeqCheck() {
    nl_socket_disable_seq_check(this->sock);
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
            kme.data = nla_data(attrs[i]);
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
    //int ret = nl_socket_modify_cb(this->sock, NL_CB_VALID, NL_CB_CUSTOM, onReceiveTest, NULL);
    
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
bool KernelComm::recv() {
    int res = nl_recvmsgs_default(this->sock);
    return (res == 0);
}

/**
 * @brief Send data via Netlink
 * @param family The netlink family
 * @param command   The netlink command 
 * @param data  data to send
 * 
 * @return number of bytes sent or negative error code
*/
int KernelComm::sendData(const char* family, int command, DataToSend* data) {
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

    if (!genlmsg_put(msg, NL_AUTO_PORT, NL_AUTO_SEQ, familyId, 0, 0, command, 1)) {
        nlmsg_free(msg);
        return -3;
    }

    if(data != nullptr) {
        while(std::shared_ptr<DataDto> d = data->next()) {
            if(std::holds_alternative<int>(d->getData())) {
                if (nla_put_u32(msg, d->getAttribute(), (uint32_t)std::get<int>(d->getData())) < 0) {
                    nlmsg_free(msg);
                    return -4;
                }
            } else if(std::holds_alternative<std::string>(d->getData())) {
                if (nla_put_string(msg, d->getAttribute(), (std::get<std::string>(d->getData()).c_str())) < 0) {
                    nlmsg_free(msg);
                    return -5;
                }
            }
        }
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