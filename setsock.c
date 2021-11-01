#include "setsock.h"

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
