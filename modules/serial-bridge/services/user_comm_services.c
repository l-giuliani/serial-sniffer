#include "user_comm_services.h"

#include <linux/netlink.h>
#include <net/genetlink.h>
#include <linux/string.h>

#include "../include/netlink_data.h"

#define GENL_FAMILY_NAME "ser_notif_family"
#define GENL_MULTICAST_GROUP "ser_notif_group"

static struct genl_family sniff_family;
static const struct genl_multicast_group sniff_mcgroup[] = {
    [0] = { .name = GENL_MULTICAST_GROUP },
};

/*static struct nla_policy sniff_policy[SNIFF_ATTR_MAX + 1] = {
    [SNIFF_ATTR_BUFFER] = { .type = NLA_BINARY },
};*/

static int sniff_cmd_handler(struct sk_buff *skb, struct genl_info *info) {
    char *msg;
    if (!info->attrs[SNIFF_ATTR_BUFFER]) {
        pr_err("Generic Netlink: messaggio vuoto ricevuto\n");
        return -EINVAL;
    }

    msg = nla_data(info->attrs[SNIFF_ATTR_BUFFER]);
    pr_info("Generic Netlink: ricevuto messaggio: %s\n", msg);

    return 0;
}

static const struct genl_ops sniff_ops[] = {
    {
        .cmd = SNIFF_CMD_SNIFFED,
        .flags = 0,
        //.policy = sniff_policy,
        .doit = sniff_cmd_handler,
    },
};

// Funzione per inviare messaggi multicast
int send_multicast_message(const char* buffer, int buffer_len) {
    struct sk_buff *skb;
    void *msg_header;
    int res;

    if(buffer == NULL || buffer_len == 0) {
        return -EINVAL;
    }

    skb = genlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
    if (!skb) {
        pr_err("Generic Netlink: error allocating memory\n");
        return -ENOMEM;
    }

    msg_header = genlmsg_put(skb, 0, 0, &sniff_family, 0, SNIFF_CMD_SNIFFED);
    if (!msg_header) {
        pr_err("Generic Netlink: error creating header\n");
        nlmsg_free(skb);
        return -EMSGSIZE;
    }

    res = nla_put(skb, SNIFF_ATTR_BUFFER, buffer_len, buffer);
    if (res) {
        pr_err("Generic Netlink: error adding attribute buffer\n");
        nlmsg_free(skb);
        return res;
    }

    genlmsg_end(skb, msg_header);

    res = genlmsg_multicast(&sniff_family, skb, 0, 0, GFP_KERNEL);
    if (res < 0) {
        pr_err("Generic Netlink: errore nell'invio del messaggio multicast: %d\n", res);
        return res;
    }

    return 0;
}

int user_comm_init(void) {
    int res;

    // Definizione della famiglia Generic Netlink
    sniff_family = (struct genl_family) {
        .name = GENL_FAMILY_NAME,
        .version = 1,
        .maxattr = SNIFF_ATTR_MAX,
        .module = THIS_MODULE,
        .ops = sniff_ops,
        .n_ops = ARRAY_SIZE(sniff_ops),
        .mcgrps = sniff_mcgroup,
        .n_mcgrps = ARRAY_SIZE(sniff_mcgroup)
    };

    // Registrazione della famiglia
    res = genl_register_family(&sniff_family);
    if (res) {
        pr_err("Generic Netlink: error registering family: %d\n", res);
        return res;
    }

    return 0;
}

void user_comm_free(void) {
    genl_unregister_family(&sniff_family);
}

void user_comm_send(void) {

}