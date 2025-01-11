#ifndef __NETLINK_DATA__
#define __NETLINK_DATA__

#define GENL_FAMILY_NAME "sern_family"
#define GENL_MULTICAST_GROUP "sern_group"

enum {
    SNIFF_CMD_UNSPEC,
    SNIFF_CMD_SNIFFED,
    SNIFF_CMD_SEND_TEST,
    __SNIFF_CMD_MAX,
};
#define SNIFF_CMD_MAX (__SNIFF_CMD_MAX - 1)

enum {
    SNIFF_ATTR_UNSPEC,
    SNIFF_ATTR_BUFFER,
    __SNIFF_ATTR_MAX,
};
#define SNIFF_ATTR_MAX (__SNIFF_ATTR_MAX - 1)

#endif