#ifndef SETSOCK_H_SENTRY
#define SETSOCK_H_SENTRY

#include <strings.h>
#include <stddef.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>

#include "ping.h"

void preparing();
int setup();
int create_socket(int, int, int);
struct addrinfo *host_serv(const char*);

#endif
