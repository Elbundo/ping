#ifndef PING_H_SENTRY
#define PING_H_SENTRY

#include <sys/time.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <sys/time.h>

struct ping_state{
	int sockfd;

	void (*proc)(char*, ssize_t, struct msghdr*, struct timeval);
	void (*send)(void);
	void (*init)(void);

	struct sockaddr *sasend;
	struct sockaddr *sarecv;
	socklen_t salen;

	char *dest;
	int datalen;

	long ntransmitted;
	long nreceived;
	struct timeval start;
//	unsigned char
//		opt_quiet:1,
//		opt_ttl:1,
//		opt_pingfilled:1,
//		opt_interval:1;
//--------Options-------
	long opt_npackets;
	int opt_interval;
	int opt_ttl;
	unsigned char
		opt_quiet:1;
//----------------------
};

extern struct ping_state ps;

#endif
