#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h> 
#include <linux/string.h>
#include <linux/tty.h>

static struct kprobe kp;
static int i = 0;
void (*listener_function)(char*, int) = NULL;

static int handler_pre(struct kprobe *p, struct pt_regs *regs) {
    struct tty_struct *tty; 
    char idx[3];
    const unsigned char* buf;
    size_t count;
    char kbuf[512];

    if(listener_function == NULL) {
        return 0;
    }

    tty = (struct tty_struct*)regs->di;
    
    if (strncmp(tty->name, "pts", 3) == 0) {
        u16 pts_number;
        int devnamelen = strlen(tty->name) - 3;
        if(devnamelen > 0 && devnamelen <= 3){
            int conv_res;
            memset(idx,0, sizeof(idx));
            memcpy(idx, &tty->name[3], devnamelen);            
            conv_res = kstrtou16(idx, 10, &pts_number);
            if(conv_res == 0) {
                if (pts_number == 6) {
                    i++;
                    if(i == 2) {
                        i=0;
                        return 0;
                    }
                    buf = (const unsigned char *)regs->si;
                    count = (size_t)regs->dx;
                    if(count > 0) {
                        memcpy(kbuf, buf, count);
                        kbuf[count] = '\0';
                        pr_info("Scrittura intercettata sul PTS %d, testo: %s\n", pts_number,kbuf);
                        listener_function(kbuf, count);                        
                    }
                }
            }
        }
    }  

    return 0;
}

void set_listener_function(char* _listener_function) {
    listener_function = (void (*)(char*, int))kallsyms_lookup_name(_listener_function);
}
EXPORT_SYMBOL(set_listener_function);

void lg_sniff_keep_alive(char* device) {
    //TODO handle keep alive
}

static int __init functions_override_init(void) {
    int ret;

    kp.symbol_name = "pty_write";  // Modifica con il nome corretto della funzione
    kp.pre_handler = handler_pre;

    ret = register_kprobe(&kp);
    if (ret < 0) {
        pr_err("Errore nella registrazione di kprobe: %d\n", ret);
        return ret;
    }

    return 0;

}

static void __exit functions_override_exit(void) {
    unregister_kprobe(&kp);
}

module_init(functions_override_init);

module_exit(functions_override_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Lorenzo Giuliani <lorenzo.giuliani@live.com>");

MODULE_DESCRIPTION("Linux kernel kprobe tty interceptor");