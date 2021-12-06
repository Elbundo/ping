#include "states.h"

void starting(int argc, char*argv[])
{
	switch(create_log()){
	case OK:
		write_log("OK", _("Log file has been created"));
		parsing(argc, argv);
		break;
	case LOG_ERR:
		printf("Couldn't create log file\n");
		parsing(argc, argv);
		break;
	}
}

void parsing(int argc, char *argv[])
{
	write_log("", _("Parsing has begun..."));
	switch(parse_cmdline(argc, argv)){
	case OK:
		write_log("OK", _("Parsing finished successfully"));
		getting_ip();
		break;
	case PARSE_ERR:
		write_log("PARSE_ERR", _("Parsing error"));
		finalize();
		break;
	case QUIT:
		write_log("OK", _("The program completed correctly"));
		finalize();
		break;
	}
}

void getting_ip()
{
	write_log("", _("Getting an IP address..."));
	switch(get_ipaddress()){
	case OK:
		write_log("OK", _("IP address received"));
		creating_sock_and_send_first();
		break;
	case GETIP_FAIL:
		write_log("GETIP_FAIL", _("Couldn't get the address"));
		finalize();
		break;
	case CVRTIP_FAIL:
		write_log("CVRT_FAIL", _("Failed to convert the address"));
		finalize();
		break;
	case UNKN_FAMILY:
		write_log("UNKN_FAMILY", _("Unknown address family"));
		finalize();
		break;
	}
}

void creating_sock_and_send_first()
{
	write_log("", _("Creating socket and sending first ECHO_REQUEST..."));
	switch(create_and_send(ps.sasend->sa_family, SOCK_RAW, ps.icmpproto)){
	case OK:
		write_log("OK", _("The socket has been created, the echo request has been sent"));
		receiving_and_processing();
	case CRTSOCK_FAIL:
		write_log("CRTSOCK_FAIL", _("Failed to create socket"));
		finalize();
		break;
	case SETOPT_FAIL:
		write_log("SETOPT_FAIL", _("Failed to set options"));
		finalize();
		break;
	}
}

void receiving_and_processing()
{
	write_log("", _("Receiving and analyzing the ECHO_REPLY..."));
	switch(readloop()){
	case 1:
		write_log("err", _("Something is wrong!"));
		finalize();
		break;
	}
}
