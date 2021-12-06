#include "setsock.h"

int get_ipaddress()
{
	ps.ai = host_serv(ps.dest);
	if(!ps.ai)
		return GETIP_FAIL;
	char *h;
	if((h = sock_ntop_host(ps.ai->ai_addr, ps.ai->ai_addrlen)) == NULL)
		return CVRTIP_FAIL;
	if(ps.ai->ai_family == AF_INET){
		ps.send = send4_echo;
		ps.proc = proc4;
		ps.icmpproto = IPPROTO_ICMP;
	}/* else if(ai->ai_family == AF_INET6){
		//ps.send = send6_echo;
		//ps.proc = proc6;
		//ps.init = init6;
		ps.icmpproto = IPPROTO_ICMPV6;
		if(IN6_IS_ADDR_V4MAPPED(&(((struct sockaddr_in6*) ai->ai_addr)->sin6_addr)))
			return 3;
	}*/ else{
		return UNKN_FAMILY;
	}
	ps.sasend = ps.ai->ai_addr;
	ps.sarecv = calloc(1, ps.ai->ai_addrlen);
	ps.salen = ps.ai->ai_addrlen;
	printf("PING %s (%s): %d data bytes\n", ps.ai->ai_canonname ? ps.ai->ai_canonname : h, h, ps.datalen);
	return 0;
}

int create_and_send(int family, int type, int protocol)
{
	ps.sockfd = socket(family, type, protocol);
	if(ps.sockfd < 0){
		perror("socket");
		return CRTSOCK_FAIL;
	}
	if(ps.opt_ttl){
		if(setsockopt(ps.sockfd, IPPROTO_IP, IP_TTL, &ps.opt_ttl_value, sizeof(ps.opt_ttl_value)) == -1){
			perror("set socket option");
			return SETOPT_FAIL;
		}
	}
	setuid(getuid());
	signal(SIGALRM, sig_alarm);
	signal(SIGINT, get_statistics);
	build_icmphdr();
	sig_alarm(SIGALRM);
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
	n = getaddrinfo(host, NULL, &hints, &res);
	if(n != 0){
		printf("getting ip: %s\n", gai_strerror(n));
		return NULL;
	}
	return res;
}

char *sock_ntop_host(const struct sockaddr *sa, socklen_t salen)
{
	static char str[128];

	switch(sa->sa_family){
	case AF_INET:
	{
		struct sockaddr_in *sin = (struct sockaddr_in *) sa;
		if(inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL){
			perror("getting ip");
			return NULL;
		}
		return str;
	}/*
	case AF_INET6:
	{
		struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *) sa;
		if(inet_ntop(AF_INET6, &sin6->sin6_addr, str, sizeof(str)) == NULL)
			return NULL;
		return str;
	}*/
	default:
		return NULL;
	}
}
