#ifndef __NETLINK_DATA__
#define __NETLINK_DATA__

#define GENL_FAMILY_NAME "sern_family"
#define GENL_MULTICAST_GROUP "sern_group"

enum {
    SNIFF_CMD_UNSPEC,
    SNIFF_CMD_SNIFFED,
    SNIFF_CMD_SEND_TEST,
    SNIFF_CMD_KEEP_ALIVE,
    __SNIFF_CMD_MAX,
};
#define SNIFF_CMD_MAX (__SNIFF_CMD_MAX - 1)

enum {
    SNIFF_ATTR_UNSPEC,
    SNIFF_ATTR_BUFFER,
    __SNIFF_CMD_SNIFFED_ATTR_MAX,
};
#define SNIFF_CMD_SNIFFED_ATTR_MAX (__SNIFF_CMD_SNIFFED_ATTR_MAX - 1)

enum {
    SNIFF_ATTR_KEEP_ALIVE_UNSPEC,
    SNIFF_ATTR_SER_DEVICE,
    SNIFF_ATTR_EXEC_TEST,
    __SNIFF_CMD_KEEP_ALIVE_ATTR_MAX
};
#define SNIFF_CMD_KEEP_ALIVE_ATTR_MAX (__SNIFF_CMD_KEEP_ALIVE_ATTR_MAX - 1)

static inline int get_max_attr(void)
{
    int max_val = SNIFF_CMD_SNIFFED_ATTR_MAX;
    if (SNIFF_CMD_KEEP_ALIVE_ATTR_MAX > max_val) {
        max_val = SNIFF_CMD_KEEP_ALIVE_ATTR_MAX;
    }
    return max_val;
}

#define SNIFF_ATTR_MAX get_max_attr()

#endif