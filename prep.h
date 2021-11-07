#ifndef PREP_H_SENTRY
#define PREP_H_SENTRY

#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <float.h>
#include <errno.h>
#include <error.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include "ping.h"

int parse_cmdline(int, char**);
void usage();
int fill(char*, unsigned char*, size_t);
int strtol_or_err(long*, const char*, const char*, const long, const long);
int strtod_or_err(double*, const char*, const char*, const double, const double);

#define PARSE_ERR 1
#define QUIT 2

#endif
