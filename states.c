#include "states.h"

void parsing(int argc, char *argv[])
{
	switch(parse_cmdline(argc, argv)){
	case OK:
		getting_ip();
		break;
	case ERR:
		printf("ping.c error\n");
		break;
	case QUIT:
		break;
	}
}

void getting_ip()
{
	switch(get_ipaddress()){
	case OK:
//write log
		creating_sock_and_send_first();
		break;
	case GETIP_FAIL:
//write log
		break;
	case CVRTIP_FAIL:
//write log
		break;
	case UNKN_FAMILY:
//write log
		break;
	}
}

void creating_sock_and_send_first()
{
	switch(create_and_send(ps.sasend->sa_family, SOCK_RAW, ps.icmpproto)){
	case OK:
		receiving_and_processing();
	case CRTSOCK_FAIL:
//write log
		break;
	case SETOPT_FAIL:
//write log
		break;
	}
}

void receiving_and_processing()
{
	switch(readloop()){
	case 1:
		printf("not_ok!\n");
		break;
	}
}