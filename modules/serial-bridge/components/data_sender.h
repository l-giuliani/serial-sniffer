#ifndef _DATA_SENDER_H_
#define _DATA_SENDER_H_

#include <linux/circ_buf.h>
#include <linux/workqueue.h>

typedef struct {
    struct circ_buf buffer;
    struct work_struct work;
} DATA_SENDER_PARAMS;

int data_sender_init(void);
void data_sender_start_worker(void);

#endif