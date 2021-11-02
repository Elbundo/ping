#include "setsock.h"

int setup()
{
	struct addrinfo *ai;
	ai = host_serv(ps.dest);
	if(!ai)
		return 1;
	char h[128];
	struct sockaddr_in *sin = (struct sockaddr_in*) ai->ai_addr;
	if(inet_ntop(AF_INET, &sin->sin_addr, h, sizeof(h)) == NULL)
		return 2;
	//initializaition
	ps.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if(ps.sockfd < 0)
		return 3;
	setuid(getuid());
	ps.sasend = ai->ai_addr;
	ps.sarecv = calloc(1, ai->ai_addrlen);
	ps.salen = ai->ai_addrlen;
	return 0;
}

int create_socket(int family, int type, int protocol)
{
	ps.sockfd = socket(family, type, protocol);
	if(ps.sockfd < 0)
		return 1;
	return 0;
}

struct addrinfo *host_serv(const char *host)
{
	int n;
	struct addrinfo hints, *res;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_flags = AI_CANONNAME;
	hints.ai_family = 0;
	hints.ai_socktype = 0;

	if((n = getaddrinfo(host, NULL, &hints, &res)) != 0)
		return NULL;
	return res;
}
