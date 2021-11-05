#include <stdio.h>
#include <stdlib.h>

#include "ping.h"
#include "prep.h"
#include "setsock.h"

struct ping_state ps = {0};

int main(int argc, char *argv[])
{
	gettimeofday(&ps.start, NULL);
	switch(parse_cmdline(argc, argv)){
	case 0:
		preparing();
		break;
	default:
		printf("ping.c error\n");
		break;
	}
	return 0;
}
