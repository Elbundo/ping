#include "send.h"

char sendbuf[BUFSIZE];
int len;
struct icmp *my_icmp;
int nsent = 1;

void sig_alarm(int s)
{
	(*ps.send)();
	if(!ps.opt_limpack || ps.ntransmitted < ps.opt_npackets)
		set_timer();
}

void get_statistics(int s)
{
	int percent = 100*((ps.ntransmitted - ps.nreceived) / ps.ntransmitted);
	struct timeval end;
	gettimeofday(&end, NULL);
	long time = (end.tv_sec - ps.start.tv_sec) * 1000 + (end.tv_usec - ps.start.tv_usec) / 1000;
	printf("\n");
	printf("--- %s ping statistics ---\n", ps.dest);
	printf("%ld packets transmitted, %ld received, %d%% packet loss, time %ldms\n", ps.ntransmitted, ps.nreceived, percent, time);
	write_log("OK", "The program completed correctly");
	finalize();
}

void send4_echo()
{
	gettimeofday((struct timeval*) my_icmp->icmp_data, NULL);
	my_icmp->icmp_seq = nsent++;
	my_icmp->icmp_cksum = 0;
	my_icmp->icmp_cksum = in_cksum((u_short*)my_icmp, len);
	if(sendto(ps.sockfd, sendbuf, len, 0, ps.sasend, ps.salen) < 0){
		printf("send error\n");
		return;
	}
	ps.ntransmitted++;
}

void build_icmphdr()
{	
	my_icmp = (struct icmp*) sendbuf;
	my_icmp->icmp_type = ICMP_ECHO;
	my_icmp->icmp_code = 0;
	my_icmp->icmp_id = getpid();
	memcpy(my_icmp->icmp_data, ps.pattern, ps.datalen);
	len = 8 + ps.datalen;
}

void set_timer()
{
	struct itimerval it;
	it.it_interval.tv_sec = 0;
	it.it_interval.tv_usec = 0;
	it.it_value.tv_sec = ps.opt_interval / 1000;
	it.it_value.tv_usec = 1000 * (ps.opt_interval % 1000);
	setitimer(ITIMER_REAL, &it, NULL);
}

unsigned short in_cksum(unsigned short *addr, int len)
{
    int sum = 0;
    u_short answer = 0;
    u_short *w = addr;
    int nleft = len;

    while (nleft > 1)
    {
	  sum += *w++;
	  nleft -= 2;
    }

    if (nleft == 1)
    {
	  *(u_char *) (&answer) = *(u_char *) w;
	  sum += answer;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return (answer);
}
