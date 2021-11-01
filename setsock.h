#ifndef SETSOCK_H_SENTRY
#define SETSOCK_H_SENTRY

#include <strings.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "ping.h"

int create_socket(int, int, int);
struct addrinfo *host_serv(const char*);

#endif
