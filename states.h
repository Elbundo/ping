#ifndef STATES_H_SENTRY
#define STATES_H_SENTRY

#include "ping.h"

void starting(int argc, char *argv[]);
void parsing(int argc, char *argv[]);
void getting_ip();
void creating_sock_and_send_first();
void receiving_and_processing();

#endif
