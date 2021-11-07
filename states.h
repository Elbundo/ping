#ifndef STATES_H_SENTRY
#define STATES_H_SENTRY

#include <stdio.h>
#include "ping.h"
#include "prep.h"
#include "setsock.h"

void parsing();
void getting_ip();
void creating_sock_and_send_first();
void receiving_and_processing();

#endif