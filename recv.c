#include "recv.h"

int readloop()
{
	char recvbuf[BUFSIZE];
	char controlbuf[BUFSIZE];
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
		gettimeofday(&tval, NULL);
		(*ps.proc)(recvbuf, n, &msg, &tval);
		if(ps.opt_limpack && ps.ntransmitted == ps.opt_npackets)
			get_statistics(SIGINT);
	}
	return 1;
}

void tv_sub(struct timeval *out, struct timeval *in)
{
	if((out->tv_usec -= in->tv_usec) < 0){
		--out->tv_sec;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}

void proc4(char *ptr, ssize_t len, struct msghdr *msg, struct timeval *tvrecv)
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
		if(!ps.opt_quiet)
			printf("%d bytes from %s: seq=%u, ttl=%d, rtt=%.3f ms\n",
				icmplen, sock_ntop_host(ps.sarecv, ps.salen), icmp->icmp_seq, ip->ip_ttl, rtt);
		ps.nreceived++;
	}else if(icmp->icmp_type == ICMP_DEST_UNREACH){
		switch(icmp->icmp_code){
		case ICMP_NET_UNREACH:
			printf("Destination Net Unreachable\n");
			break;
		case ICMP_HOST_UNREACH:
			printf("Destination Host Unreachable\n");
			break;
		case ICMP_NET_UNKNOWN:
			printf("Destination Net Unknown\n");
			break;
		case ICMP_HOST_UNKNOWN:
			printf("Destination Host Unknown\n");
			break;
		case ICMP_HOST_ISOLATED:
			printf("Source Host Isolated\n");
			break;
		default:
			printf("Destination Unreachable, Bad Code: %d\n", icmp->icmp_code);
			break;
		}
	}else if(icmp->icmp_type == ICMP_TIME_EXCEEDED){
		switch(icmp->icmp_code){
		case ICMP_EXC_TTL:
			printf("Time to live exceeded\n");
			break;
		default:
			printf("Time exceeded, Bad Code: %d\n", icmp->icmp_code);
			break;
		}
	}else if(icmp->icmp_type == ICMP_ECHO){
	}else{
		printf("Bad ICMP type: %d\n", icmp->icmp_type);
	}
}
