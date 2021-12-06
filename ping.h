#ifndef PING_H_SENTRY
#define PING_H_SENTRY

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netinet/ip6.h>
#include <netinet/icmp6.h>

#include "prep.h"
#include "setsock.h"
#include "recv.h"
#include "send.h"
#include "states.h"
#include "logger.h"

#define _(STR) (STR)

#define BUFSIZE 1500
#define OK 0

struct ping_state{
	int sockfd;
	char *dest;
	int datalen;

	long ntransmitted;
	long nreceived;
	struct timeval start;
/*-------protocol setup-------*/
	void (*proc)(char*, ssize_t, struct msghdr*, struct timeval*);
	void (*send)(void);
	void (*init)(void);

	struct sockaddr *sasend;
	struct sockaddr *sarecv;
	socklen_t salen;

	int icmpproto;
/*--------Options-------*/
	long opt_npackets;
	int opt_interval;
	unsigned char opt_ttl_value;
	unsigned char pattern[84];
	unsigned char
		opt_ttl:1,
		opt_limpack:1,
		opt_quiet:1;
};

extern struct ping_state ps;

#endif
