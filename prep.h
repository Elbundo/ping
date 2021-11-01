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

#include "ping.h"

int parse_cmdline(int, char**);
long strtol_or_err(const char*, const char*, const long, const long);
double strtod_or_err(const char*, const char*, const double, const double);

#endif
