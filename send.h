#ifndef SEND_H_SENTRY
#define SEND_H_SENTRY

#include <string.h>
#include <stddef.h>

#include "ping.h"


void all_for_send();
void sig_alarm(int);
void get_statistics(int);
void build_icmphdr();
void send4_echo();
void set_timer();
unsigned short in_cksum(unsigned short*, int);

#endif
