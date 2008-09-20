#include "dump.h"
#include <linux/netfilter.h>
#include <sys/socket.h>
//#define printf(x,y) fprintf(stderr,x,y)

void dump_ipq(ipq_packet_msg_t *ipq){
	fprintf(stderr,"indev: %s\n",ipq->indev_name);
	fprintf(stderr,"outdev: %s\n",ipq->outdev_name);



}

void dump_ip(struct iphdr *ipd){
	printf("SEQ number : %d\n",ntohs(ipd->id));
	printf("protocol : %u\n",ipd->protocol);
	printf("tot_len :% u\n", ipd->tot_len);
	printf("Source : %s\n",inet_ntoa(ipd->saddr));
	printf("Dest   : %s\n",inet_ntoa(ipd->daddr));
	printf("\n");
					

}


void dump(const char *str,int size){
	int r;
	for(r=0;r<size;r++){
		printf("%2C ",str[r]);
		if(r % 30==0) printf("\n");
	}
	printf("\n\n");
}
