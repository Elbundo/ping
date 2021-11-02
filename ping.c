#include <stdio.h>

#include "ping.h"
#include "prep.h"
#include "setsock.h"

struct ping_state ps;

int main(int argc, char *argv[])
{
	struct addrinfo *ai;
	parse_cmdline(argc, argv);
	printf("%s\n", ps.dest);
	ai = host_serv(ps.dest);
	if(!ai){
		printf("not found\n");
		exit(1);
	}
	printf("flags = %d\n", ai->ai_flags);
	printf("family = %d\n", ai->ai_family);
	printf("socktype = %d\n", ai->ai_socktype);
	printf("protocol = %d\n", ai->ai_protocol);
	printf("addrlen = %d\n", ai->ai_addrlen);
	printf("sa_family = %d\n", ai->ai_addr->sa_family);
	char h[128];
	struct sockaddr_in *sin = (struct sockaddr_in*) ai->ai_addr;
	if(inet_ntop(AF_INET, &sin->sin_addr, h, sizeof(h)) == NULL){
		printf("bad\n");
	}
	printf("\nip = %s\n", h);	
	printf("\ncanonname = %s\n", ai->ai_canonname);
	free(ps.dest);
	return 0;
}
