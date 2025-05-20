#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kallsyms.h>
#include <linux/kobject.h>
#include <linux/string.h>
#include "services/user_comm_services.h"

static ssize_t deviceName_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t deviceName_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);
static ssize_t sniff_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t sniff_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);


static struct kobject *kobj_serial_sniffer;
static struct kobj_attribute serial_sniffer_dv_attr = __ATTR(deviceName, 0664, deviceName_show, deviceName_store);
static struct kobj_attribute serial_sniffer_active_attr = __ATTR(active, 0664, sniff_show, sniff_store);
static char deviceName[64] = "None";
static bool sniff = false;

void (*listener_function)(char*) = NULL;

void serial_listener_callback(char* buffer, int len) {
    pr_info("AWESOME CALLED !!!\n");
}
EXPORT_SYMBOL(serial_listener_callback);

static ssize_t deviceName_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    memcpy(buf, deviceName, strlen(deviceName));
    return strlen(deviceName);
}

static ssize_t deviceName_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    memcpy(deviceName, buf, count);
    deviceName[count] = '\0';
    pr_info("User wrote: %s\n", deviceName);
    return count;
}

static ssize_t sniff_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    u8 sniffVal = sniff?1:0;
    sprintf(buf, "%d", sniffVal);
    return 1;
}

static ssize_t sniff_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    u8 sniffVal;
    if(((count != 1) && (count > 1 && buf[1] != '\n')) || (buf[0] != '0' && buf[0] != '1')) {
        return -EINVAL;
    }
    sniffVal = buf[0] - '0';
    sniff = (sniffVal==1?true:false);
    return count;
}

int initSysfsInterface(void) {
    
    kobj_serial_sniffer = kobject_create_and_add("serial_sniffer",kernel_kobj);
    if(kobj_serial_sniffer == NULL) {
        return -ENOMEM;
    }

    if(sysfs_create_file(kobj_serial_sniffer,&serial_sniffer_dv_attr.attr)){
        pr_err("Cannot create sysfs file......\n");
        return -ENOMEM;
    }

    if(sysfs_create_file(kobj_serial_sniffer,&serial_sniffer_active_attr.attr)){
        pr_err("Cannot create sysfs file......\n");
        return -ENOMEM;
    }

    return 0;
}

static int __init serial_bridge_init(void) {
    // int res = initSysfsInterface();
    // if (res != 0) {
    //     return res;
    // }

    listener_function = (void (*)(char*))kallsyms_lookup_name("set_listener_function");
    if(listener_function != NULL) {
        listener_function("serial_listener_callback");
    }
    pr_info("sto per inizializzare netlink\n");
    user_comm_init();

    return 0;

}

static void __exit serial_bridge_exit(void) {
    //kobject_put(kobj_serial_sniffer);
    //sysfs_remove_file(kobj_serial_sniffer, &serial_sniffer_dv_attr.attr);
    //sysfs_remove_file(kobj_serial_sniffer, &serial_sniffer_active_attr.attr);
    user_comm_free();
}

module_init(serial_bridge_init);

module_exit(serial_bridge_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Lorenzo Giuliani <lorenzo.giuliani@live.com>");

MODULE_DESCRIPTION("Linux kernel serial bridge");