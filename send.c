#include "send.h"
#include "recv.h"
#include <stdio.h>
#include <stdlib.h>

void all_for_send()
{
	switch(set_timer()){
	case 0:
		all_for_recv();
		break;
	default:
		printf("send.c error\n");
		break;
	}
}

void sig_alarm(int s)
{
	send_echo();
}

void finalize(int s)
{
	int percent = 100*((ps.ntransmitted - ps.nreceived) / ps.ntransmitted);
	struct timeval end;
	gettimeofday(&end, NULL);
	long time = (end.tv_sec - ps.start.tv_sec) * 1000 + (end.tv_usec - ps.start.tv_usec) / 1000;
	printf("\n");
	printf("--- %s ping statistics ---\n", ps.dest);
	printf("%ld packets transmitted, %ld received, %d%% packet loss, time %ldms\n", ps.ntransmitted, ps.nreceived, percent, time);
	exit(0);
}

void send_echo()
{
	int nsent = 1;
	char sendbuf[1500];
	int len;
	struct icmp *my_icmp;
	my_icmp = (struct icmp*) sendbuf;
	my_icmp->icmp_type = ICMP_ECHO;
	my_icmp->icmp_code = 0;
	my_icmp->icmp_id = getpid();
	my_icmp->icmp_seq = nsent++;
	memset(my_icmp->icmp_data, 0xa5, (ps.datalen = 56));
	gettimeofday((struct timeval*) my_icmp->icmp_data, NULL);
	len = 8 + ps.datalen;
	my_icmp->icmp_cksum = 0;
	my_icmp->icmp_cksum = in_cksum((u_short*)my_icmp, len);
	if(sendto(ps.sockfd, sendbuf, len, 0, ps.sasend, ps.salen) < 0){
		printf("send error\n");
		return;
	}
	ps.ntransmitted++;
	
}

int set_timer()
{
	struct itimerval it;
	it.it_interval.tv_sec = 1;
	it.it_interval.tv_usec = 0;	
	it.it_value.tv_sec = 1;
	it.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &it, NULL);
	return 0;
}

unsigned short in_cksum(unsigned short *addr, int len)
{
    register int sum = 0;
    u_short answer = 0;
    register u_short *w = addr;
    register int nleft = len;
    /*
     * Our algorithm is simple, using a 32 bit accumulator (sum), we add
     * sequential 16 bit words to it, and at the end, fold back all the
     * carry bits from the top 16 bits into the lower 16 bits.
     */
    while (nleft > 1)
    {
	  sum += *w++;
	  nleft -= 2;
    }
    /* mop up an odd byte, if necessary */
    if (nleft == 1)
    {
	  *(u_char *) (&answer) = *(u_char *) w;
	  sum += answer;
    }
    /* add back carry outs from top 16 bits to low 16 bits */
    sum = (sum >> 16) + (sum & 0xffff);		/* add hi 16 to low 16 */
    sum += (sum >> 16);				/* add carry */
    answer = ~sum;				/* truncate to 16 bits */
    return (answer);
}
