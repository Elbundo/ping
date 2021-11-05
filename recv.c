#include "recv.h"

void all_for_recv()
{
	switch(readloop()){
	case 0:
		printf("ok!\n");
		break;
	default:
		printf("recv.c error\n");
		break;
	}
}

int readloop()
{
	char recvbuf[1500];
	char controlbuf[1500];
	struct msghdr msg;
	struct iovec iov;
	ssize_t n;
	struct timeval tval;

	iov.iov_base = recvbuf;
	iov.iov_len = sizeof(recvbuf);
	msg.msg_name = ps.sarecv;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = controlbuf;
	for(;;){
		msg.msg_namelen = ps.salen;
		msg.msg_controllen = sizeof(controlbuf);
		n = recvmsg(ps.sockfd, &msg, 0);
		if(n < 0){
			if(errno == EINTR)
				continue;
			else
				printf("recvmsg error\n");
		}
		ps.nreceived++;
		gettimeofday(&tval, NULL);
		proc(recvbuf, n, &msg, &tval);
	}
	return 0;
}

void tv_sub(struct timeval *out, struct timeval *in)
{
	if((out->tv_usec -= in->tv_usec) < 0){
		--out->tv_sec;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}

void proc(char *ptr, ssize_t len, struct msghdr *msg, struct timeval *tvrecv)
{
	int hlen1, icmplen;
	double rtt;
	struct ip *ip;
	struct icmp *icmp;
	struct timeval *tvsend;

	ip = (struct ip *) ptr;
	hlen1 = ip->ip_hl << 2;
	if(ip->ip_p != IPPROTO_ICMP)
		return;
	icmp = (struct icmp *) (ptr + hlen1);
	if( (icmplen = len - hlen1) < 8)
		return;
	if(icmp->icmp_type == ICMP_ECHOREPLY){
		if(icmp->icmp_id != getpid())
			return;
		if(icmplen < 16)
			return;

		tvsend = (struct timeval *) icmp->icmp_data;
		tv_sub(tvrecv, tvsend);
		rtt = tvrecv->tv_sec * 1000.0 + tvrecv->tv_usec / 1000.0;
		printf("%d bytes from %%s: seq=%u, ttl=%d, rtt=%.3f ms\n", icmplen, /*sock_ntop(ps.sarecv, ps.salen),*/ icmp->icmp_seq, ip->ip_ttl, rtt);
	}
}
