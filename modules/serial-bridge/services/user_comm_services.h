#ifndef __USER_COMM_SERVICES__
#define __USER_COMM_SERVICES__

int user_comm_init(void);
void user_comm_free(void);
void user_comm_send(void); 
int send_multicast_message(const char* buffer, int buffer_len);

#endif