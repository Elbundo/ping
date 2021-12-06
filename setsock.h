#ifndef SETSOCK_H_SENTRY
#define SETSOCK_H_SENTRY

#include <strings.h>
#include <stddef.h>
#include <netdb.h>

#include "ping.h"

int send_first();
int get_ipaddress();
int create_and_send(int, int, int);
struct addrinfo *host_serv(const char*);
char *sock_ntop_host(const struct sockaddr*, socklen_t);

#define GETIP_FAIL 1
#define CVRTIP_FAIL 2
#define UNKN_FAMILY 4

#define CRTSOCK_FAIL 1
#define SETOPT_FAIL 2

#endif
