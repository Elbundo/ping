#include "prep.h"

int parse_cmdline(int argc, char **argv){
	int opt;
	char *outpack_fill = NULL;
	while((opt = getopt(argc, argv, "c:i:p:t:vq")) != -1){
		switch(opt){
		case 'c':
		{
			ps.opt_limpack = 1;
			int err = strtol_or_err(&ps.opt_npackets, optarg, "invalid argument", 1, LONG_MAX);
			if(err)
				return PARSE_ERR;
			break;
		}
		case 'i':
		{
			double optval; 
			int err = strtod_or_err(&optval, optarg, "bad timing interval", 0, (int)INT_MAX/1000);
			if(err)
				return PARSE_ERR;
			ps.opt_interval = (int)(optval * 1000);
			break;
		}
		case 'p':
			outpack_fill = strdup(optarg);
			if(!outpack_fill){
				printf("memory allocation failed\n");
				return PARSE_ERR;
			}
			break;
		case 't':
			ps.opt_ttl = 1;
			long optval;
			int err = strtol_or_err(&optval, optarg, "invalid argument", 0, 255);
			if(err)
				return PARSE_ERR;
			ps.opt_ttl_value = (unsigned char)optval;
			break;
		case 'v':
			printf("version 1.0\n");
			return QUIT;
		case 'q':
			ps.opt_quiet = 1;
			break;
		default:
			usage();
			return PARSE_ERR;
		}
	}
	argc -= optind;
	argv += optind;
	if(!argc){
		printf("usage error: Destination address required\n");
		return PARSE_ERR;
	}
	ps.dest = strdup(argv[argc - 1]);
	if(!ps.dest){
		printf("memory allocation failed\n");
		return PARSE_ERR;
	}
	if(outpack_fill){
		int err = fill(outpack_fill, ps.pattern, ps.datalen);
		if(err)
			return PARSE_ERR;
		free(outpack_fill);
	}
	return OK;
}

void usage()
{
	printf("Usage\n");
	printf("  ping [options] <destination>\n\n");
	printf("Options:\n");
	printf("  <destination>\tdns name or ip address\n");
	printf("  -c <count>\tstop after <count> replies\n");
	printf("  -i <interval>\tseconds between sending each packet\n");
	printf("  -p <pattern>\tcontents of padding byte\n");
	printf("  -q\t\tquiet output\n");
	printf("  -t <ttl>\tdefine time to live\n");
	printf("  -v\t\tprint version and exit\n");
	printf("\nFor more details see docs.\n");
}

int strtol_or_err(long *num, const char *str, const char *errmsg, const long min, const long max)
{
	char *end = NULL;

	if(str == NULL || *str == '\0'){
		printf("%s: '%s'", errmsg, str);
		return 1;
	}

	*num = strtol(str, &end, 10);

	if(errno || str == end || (end && *end)){
		printf("%s: '%s'", errmsg, str);
		return 2;
	}

	if(*num < min || *num > max){
		printf("%s: '%s': out of range: %lu <= value <= %lu\n", errmsg, str, min, max);
		return 3;
	}
	
	return 0;
}

int strtod_or_err(double *num, const char *str, const char *errmsg, const double min, const double max)
{
	char *end = NULL;

	if(str == NULL || *str == '\0'){
		printf("%s: '%s'", errmsg, str);
		return 1;
	}

	*num = strtod(str, &end);
	
	if(errno || str == end || (end && *end)){
		printf("%s: '%s'", errmsg, str);
		return 2;
	}

	switch(fpclassify(*num)){
		case FP_NORMAL:
		case FP_ZERO:
			break;
		default:
			printf("%s: '%s'", errmsg, str);
			return 3;
	}

	if(*num < min || *num > max){
		printf("%s: '%s': out of range: %lf <= value <= %lf", errmsg, str, min, max);
		return 4;
	}
	
	return 0;
}

int fill(char *patp, unsigned char *packet, size_t packet_size)
{
	int ii, jj;
	unsigned int pat[16];
	char *cp;
	
	unsigned char *bp = packet + 8;
	for (cp = patp; *cp; cp++) {
		if (!isxdigit(*cp)){
			printf("patterns must be specified as hex digits: %s", cp);
			return 1;
		}
	}
	ii = sscanf(patp, "%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x", 
			&pat[0], &pat[1], &pat[2], &pat[3], &pat[4], &pat[5], &pat[6], &pat[7], 
			&pat[8], &pat[9], &pat[10], &pat[11], &pat[12], &pat[13], &pat[14], &pat[15]);
	if (ii > 0) {
		size_t kk;
		size_t max = packet_size < (size_t)ii + 8 ? 0 : packet_size - (size_t)ii + 8;
		
		for (kk = 0; kk <= max; kk += ii)
			for (jj = 0; jj < ii; ++jj)
				bp[jj + kk] = pat[jj];
	}
	if (!ps.opt_quiet) {
		printf("PATTERN: 0x");
		for (jj = 0; jj < ii; ++jj)
			printf("%02x", bp[jj] & 0xFF);
		printf("\n");
	}
	return 0;
}
