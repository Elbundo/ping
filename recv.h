#ifndef RECV_H_SENTRY
#define RECV_H_SENTRY

#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include "ping.h"

void all_for_recv();
int readloop();
void tv_sub(struct timeval*, struct timeval*);
void proc(char*, ssize_t, struct msghdr*, struct timeval*);

#endif
