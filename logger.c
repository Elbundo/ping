#include "logger.h"

FILE *logfd = NULL;

int create_log()
{
	logfd = fopen("ping.log", "w");
	if(!logfd){
		perror("create log");
		return LOG_ERR;
	}
	return 0;
}

void write_log(const char *code, const char* rec)
{
	char t[128];
	time_t cur_time;
	struct tm *m_time;
	int n;

	cur_time = time(NULL);
	m_time = localtime(&cur_time);
	strftime(t, 128, "%c", m_time);
	
	if((n = fprintf(logfd, "%s: %-10s %s\n", t, code, rec)) < 0)
		perror("Cannot write in log");
}

void close_log()
{
	fclose(logfd);
}
