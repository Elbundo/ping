#include <stdio.h>
#include <stdlib.h>

#include "ping.h"
#include "prep.h"
#include "setsock.h"
#include "states.h"

struct ping_state ps = {0};

int main(int argc, char *argv[])
{
/*Default options*/
	ps.opt_interval = 1000;
	ps.datalen = 56;
	memset(ps.pattern, 0xd1, 84);
	gettimeofday(&ps.start, NULL);
/*---------------*/
	starting(argc, argv);
	return 0;
}
