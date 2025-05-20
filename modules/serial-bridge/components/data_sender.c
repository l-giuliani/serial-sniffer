#include "data_sender.h"


#define BUFFER_SIZE 1024

static struct workqueue_struct *my_wq;
static char buffer_mem[BUFFER_SIZE];
static DATA_SENDER_PARAMS params;

static void work_fn (struct work_struct* work) {
    DATA_SENDER_PARAMS *data;

    data = container_of(work, DATA_SENDER_PARAMS, work);       
}

int data_sender_init(void) {
    params.buffer.buf = buffer_mem;
    params.buffer.head = 0;
    params.buffer.tail = 0;

    my_wq = alloc_ordered_workqueue("data_sender_ordered_wq", WQ_MEM_RECLAIM);
    if (!my_wq) {
        pr_err("Can not create workqueue\n");
        return -ENOMEM;
    }

    INIT_WORK(&params.work, work_fn);
    queue_work(my_wq, &params.work);

    return 0;
}

void data_sender_start_worker(void) {

}