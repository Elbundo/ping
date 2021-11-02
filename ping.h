#ifndef PING_H_SENTRY
#define PING_H_SENTRY

#include <sys/time.h>
#include <netinet/ip_icmp.h>

struct ping_state{
	int sockfd;

	struct sockaddr *sasend;
	struct sockaddr *sarecv;
	socklen_t salen;

	char *dest;
	int datalen;

	int ttl;
	long npackets;
	//long ntransmitted;
	//long nreceived;
	int interval; /* (msec) */

	unsigned char
		opt_quiet:1,
		opt_ttl:1,
		opt_pingfilled:1,
		opt_interval:1;

	void (*proc)(char*, ssize_t, struct msghdr*, struct timeval);
	void (*send)(void);
	void (*init)(void);


};

extern struct ping_state ps;

#endif
