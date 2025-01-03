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

KernelComm::KernelComm() {
    this->connected = false;
}

/**
 * @brief Initialize and connect netlink socket.
 */
bool KernelComm::initAndConnect() {
    this->sock = nl_socket_alloc();
    if (!this->sock) {
        return false;
    }

    if (genl_connect(this->sock)) {
        nl_socket_free(this->sock);
        return false;
    }
    this->connected = true;
}

static int onReceive(struct nl_msg *msg, void *arg) {
    printf("Messaggio ricevuto!\n");

    struct nlmsghdr *nlh = nlmsg_hdr(msg);
    struct genlmsghdr *genl_hdr = (struct genlmsghdr *)nlmsg_data(nlh);

    // Ottieni l'attributo
    struct nlattr *attrs[256];
    genlmsg_parse(nlh, 0, attrs, 256 - 1, NULL);
    if (attrs[1]) {
        char *data = nla_get_string(attrs[1]);
        printf("Dati ricevuti: %s\n", data);
    }

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
    int familyId = genl_ctrl_resolve(sock, family);
    if (familyId < 0) {
        return -1;
    }

    int groupId = genl_ctrl_resolve_grp(sock, family, group);
    if (groupId < 0) {
        return -1;
    }
    int ret = nl_socket_add_membership(sock, groupId);
    if (ret < 0) {
        nl_socket_free(sock);
        return -1;
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
    int familyId = genl_ctrl_resolve(sock, family);
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

bool KernelComm::registerCallback(int (*on_receive)(struct nl_msg*, void*)) {
    if(!this->connected) {
        return false;
    }

    nl_socket_modify_cb(this->sock, NL_CB_VALID, NL_CB_CUSTOM, onReceive, NULL);
}

void KernelComm::startListening() {
    if(!this->connected) {
        return;
    }

    
}

void KernelComm::recv() {
    nl_recvmsgs_default(this->sock);
}

KernelComm::~KernelComm() {
    nl_socket_free(this->sock);
}