#ifndef _DATA_SENDER_H_
#define _DATA_SENDER_H_

#include <linux/kfifo.h>
#include <linux/workqueue.h>

#define MAX_BUFFER_LEN  1024

typedef struct {
    char kbuf[MAX_BUFFER_LEN];
    int len;
} DATA_SENDER_ITEM;

typedef struct {
    struct kfifo fifo;    
    struct work_struct work;
    bool active;
} DATA_SENDER_PARAMS;

int data_sender_init(void);
void data_sender_start_worker(void);
void data_sender_delete(void);
int data_sender_add_data(char* buffer, int len);

#endif