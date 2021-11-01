#include "prep.h"
#include <stdio.h>

int parse_cmdline(int argc, char **argv){
	int opt;
	char *outpack_fill = NULL;
	while((opt = getopt(argc, argv, "c:i:p:t:vq")) != -1){
		switch(opt){
			case 'c':
				ps.npackets = strtol_or_err(optarg, "invalid argument", 1, LONG_MAX);
				break;
			case 'i':
			{
				double optval = strtod_or_err(optarg, "bad timing interval", 0, (int)INT_MAX/1000);
				ps.interval = (int)(optval * 1000);
				ps.opt_interval = 1;
			}
				break;
			case 'p':
				ps.opt_pingfilled = 1;
				outpack_fill = strdup(optarg);
				if(!outpack_fill)
					error(EXIT_FAILURE, errno, "memory allocation failed");
				break;
			case 't':
				ps.ttl = strtol_or_err(optarg, "invalid argument", 0, 255);
				ps.opt_ttl = 1;
				break;
			case 'v':
				//printf("version\n");
				exit(0);
			case 'q':
				ps.opt_quiet = 1;
				break;
			default:
				exit(1);
				break;
		}
	}
	argc -= optind;
	argv += optind;
	if(!argc)
		printf("wtf\n");
	ps.dest = strdup(argv[argc - 1]);
	if(!ps.dest)
		return 1;
	return 0;
}

long strtol_or_err(const char *str, const char *errmsg, const long min, const long max)
{
	long num;
	char *end = NULL;

	errno = 0;

	if(str == NULL || *str == '\0')
		error(EXIT_FAILURE, 0, "%s: '%s'", errmsg, str);

	num = strtol(str, &end, 10);

	if(errno || str == end || (end && *end))
		error(EXIT_FAILURE, 0, "%s: '%s'", errmsg, str);

	if(num < min || num > max)
		error(EXIT_FAILURE, 0, "%s: '%s': out of range: %lu <= value <= %lu", errmsg, str, min, max);
	
	return num;
}

double strtod_or_err(const char *str, const char *errmsg, const double min, const double max)
{
	double num;
	char *end = NULL;

	errno = 0;

	if(str == NULL || *str == '\0')
		error(EXIT_FAILURE, 0, "%s: '%s'", errmsg, str);

	num = strtod(str, &end);
	
	if(errno || str == end || (end && *end))
		error(EXIT_FAILURE, errno, "%s: '%s'", errmsg, str);

	switch(fpclassify(num)){
		case FP_NORMAL:
		case FP_ZERO:
			break;
		default:
			errno = ERANGE;
			error(EXIT_FAILURE, errno, "%s: '%s'", errmsg, str);
	}

	if(num < min || num > max)
		error(EXIT_FAILURE, 0, "%s: '%s': out of range: %lf <= value <= %lf", errmsg, str, min, max);
	
	return num;
}
