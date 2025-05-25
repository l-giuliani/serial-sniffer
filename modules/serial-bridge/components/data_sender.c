#include "data_sender.h"

#include <linux/string.h>

static struct workqueue_struct *my_wq;
static DATA_SENDER_PARAMS params;

static void work_fn (struct work_struct* work) {
    DATA_SENDER_PARAMS* _data;

    _data = container_of(work, DATA_SENDER_PARAMS, work);   

    while(_data->active) {
        if(kfifo_is_empty(&params.fifo)) {
            //TODO block
        }
    }
}

int data_sender_add_data(char* buffer, int len) {
    DATA_SENDER_ITEM data_sender_item;

    if(len >= MAX_BUFFER_LEN) {
        return -1;
    }

    memcpy(data_sender_item.kbuf, buffer, len);
    data_sender_item.len = len;

    if(kfifo_is_full(&params.fifo)) {
        return -2;
    }
    kfifo_in(&params.fifo, &data_sender_item, 1);
    return 1;
}

int data_sender_init(void) {
    params.active = false;

    if (kfifo_alloc(&params.fifo, sizeof(DATA_SENDER_PARAMS) * 128, GFP_KERNEL)) {
        return -ENOMEM;
    }

    my_wq = alloc_ordered_workqueue("data_sender_ordered_wq", WQ_MEM_RECLAIM);
    if (!my_wq) {
        pr_err("Can not create workqueue\n");
        return -ENOMEM;
    }

    INIT_WORK(&params.work, work_fn);
    

    return 0;
}

void data_sender_start_worker(void) {
    queue_work(my_wq, &params.work);
}

void data_sender_delete(void) {
    params.active = false;
    flush_workqueue(my_wq);
    destroy_workqueue(my_wq);
}