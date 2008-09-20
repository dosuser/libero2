#include <linux/netfilter.h>
#include <libipq.h>
#include <stdio.h>

//to use some structure
#include <netinet/ip.h>
#include <netinet/tcp.h>


//module loading
#include "module.h"


#include "dump.h"
#include <pcre.h>
#include <lib/ds.h>
#include <module.h>

#define BUFSIZE 9048 

GListInfo ModuleList;

static void die(struct ipq_handle *h)
{
        ipq_perror("passer");
        ipq_destroy_handle(h);
        exit(1);
}
void init(void){
	glist_init(&ModuleList);
	LoadModules(&ModuleList);

}
int main(int argc, char **argv)
{
	
	module_prototype httpFilters[20];



	int status;
	struct ipq_handle *h;

	init();
	
	h = ipq_create_handle(0, PF_INET);
	if (!h)
	die(h);
            
 	status = ipq_set_mode(h, IPQ_COPY_PACKET, BUFSIZE);
	if (status < 0)
		die(h);
                
	do{
		unsigned char buf[BUFSIZE];
		status = ipq_read(h, buf, BUFSIZE, 0);
		if (status < 0){
			fprintf(stderr,"eroror\n");
			die(h);
			}
                        
			switch (ipq_message_type(buf)) {
				case NLMSG_ERROR:
					fprintf(stderr, "Received error message %d\n",
					ipq_get_msgerr(buf));
					break;
				case IPQM_PACKET: {
					ipq_packet_msg_t *m = ipq_get_packet(buf);
					struct tcphdr *tcp=NULL;
					struct iphdr *ip=NULL;
					unsigned char *packet = NULL;
					unsigned char *tcpPayload=NULL;
					module_prototype module= NULL;
					int result=0;
					GListInfo *moduleList;

					dump_ipq(m);
					
					if(m->data_len > sizeof(struct iphdr)){
						packet = (unsigned char *)m + sizeof(*m);
					}
					                                
					dump_ip((struct iphdr *)packet);
					ip = packet;
					tcp = packet + sizeof(struct iphdr);
					dump(tcp,240);
					glist_rewind(&ModuleList);
					/*****
					*  run modules 
					*/					
					while((module=(module_prototype)glist_next(&ModuleList))!=NULL){
						result = (*module)(h,m,(const char*)tcp,240);
						if(result == NF_DROP) break;
					}
					
					status = ipq_set_verdict(h, m->packet_id,result, 0, NULL);
					if (status < 0) die(h);
					break;
        		                }
				default:
					fprintf(stderr, "Unknown message type!\n");
					break;
			}
	} while (1);
        
	ipq_destroy_handle(h);
	return 0;
}
