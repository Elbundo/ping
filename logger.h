#ifndef LOGGER_H_SENTRY
#define LOGGER_H_SENTRY

#include <stdio.h>
#include <time.h>

int create_log();
void write_log(const char*, const char*);
void close_log();

#define LOG_ERR 1

#endif
