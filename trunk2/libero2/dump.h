#ifndef DUMP_H

#include <stdio.h>
#include <netinet/ip.h>
#include <linux/netfilter_ipv4/ip_queue.h>

void dump_ipq(ipq_packet_msg_t *ipq);
void dump_ip(struct iphdr *ipd);
void dump(const char *str,int size);




#endif
