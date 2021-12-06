#ifndef RECV_H_SENTRY
#define RECV_H_SENTRY

#include "ping.h"

int readloop();
void tv_sub(struct timeval*, struct timeval*);
void proc4(char*, ssize_t, struct msghdr*, struct timeval*);

#endif
