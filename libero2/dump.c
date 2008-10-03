#include "dump.h"
#include <linux/netfilter.h>
#include <sys/socket.h>
//#define dlog(x,y) fprintf(stderr,x,y)

void dump_ipq(ipq_packet_msg_t *ipq){
	dlog(1,"indev: %s\n",ipq->indev_name);
	dlog(1,"outdev: %s\n",ipq->outdev_name);



}

void dump_ip(struct iphdr *ipd){
	dlog(1,"SEQ number : %d\n",ntohs(ipd->id));
	dlog(1,"protocol : %u\n",ipd->protocol);
	dlog(1,"tot_len :%u\n", ipd->tot_len);
	dlog(1,"Source : %s\n",inet_ntoa(ipd->saddr));
	dlog(1,"Dest   : %s\n",inet_ntoa(ipd->daddr));
					

}


void dump(const char *str,int size){
	int r;
	for(r=0;r<size;r++){
		dlog(1,"%2C ",str[r]);
		if(r % 30==0) dlog(1,"\n");
	}
	dlog(1,"\n\n");
}
