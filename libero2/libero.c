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
#include <pthread.h>
#include <stdlib.h>

#define BUFSIZE 2048

#include "lib/gqueue.h"

GListInfo ModuleList;
gqueue ReadyQueue;

void *patching(void *);
void *processing(void *);

static void die(struct ipq_handle *h)
{
        ipq_perror("passer");
        ipq_destroy_handle(h);
        exit(1);
}

void init(void){
	glist_init(&ModuleList);
	queue_init(&ReadyQueue,1000);
	LoadModules(&ModuleList);
}


int main(int argc, char **argv){
	pthread_t th1, th2;
	struct ipq_handle *h;
	pthread_t process1,process2,process3;
	int status;

	init();


	h = ipq_create_handle(0,PF_INET);
	if(!h) 	die(h);
 	
	status = ipq_set_mode(h, IPQ_COPY_PACKET, BUFSIZE);
	if (status < 0)
		die(h);
 
	

	pthread_create(&th1, NULL,patching,(void *)h);
//	pthread_create(&process1, NULL,patching,(void *)h);
//	pthread_create(&process2, NULL,patching,(void *)h);
	pthread_create(&process3, NULL,processing,NULL);

	pthread_join(th1, NULL);
//	pthread_join(process1, NULL);
//	pthread_join(process2, NULL);
	pthread_join(process3, NULL);
	
	return 0;
}
int safe_queue(gqueue *queue,gpointer data){
	fprintf(stderr,"append\n");
	queue_write(queue,data);
	return 1;
}
gpointer safe_dequeue(gqueue *queue){
	gpointer *node;
	queue_read(queue,(gpointer *)&node);
	if(node!=NULL) fprintf(stderr,"dequeue\n");
	return node;
}
typedef struct _ctx{
	char *buf;
	int flag;
	int status;
	struct ipq_handle *h;
	gpointer extra;
} packet_ctx;

void *patching(void *t){
	int status;
	char *buf;
	struct ipq_handle *h;
	packet_ctx *ctx;

	h = (struct ipq_handle *)t;


	
while(1){
		ctx = (packet_ctx *)malloc(sizeof(packet_ctx));
		ctx->buf=(char *)malloc(sizeof(char) * BUFSIZE);
		ctx->status= ipq_read(h,ctx->buf,BUFSIZE,0);
		if(ctx->status < 0){
			fprintf(stderr,"err!\n");
			die(h);
		}
		ctx->h = h;
		fprintf(stderr,"buffer size :%d\n",ReadyQueue.currentSize);
		if(ReadyQueue.currentSize<30) safe_queue(&ReadyQueue,ctx);
		else{
					fprintf(stderr,"sleep 1 second\n");
					sleep(1);
		}

	}

}




void *processing(void *t){
	packet_ctx *ctx;
	int status;	

	do{
		/*
		
		* process쓰레드 들이 동시에 큐에 달라 붙어서 같은 자료를 처리 할수 있다.
		* 이 경우 어느 한쪽에서 메시지를 처리하고 메모리를 해제하면 나머지 쪽에서 문제가 생길수 있다. 그래서 큐에서 하나의 쓰레드에만 값을 넘겨 줘야 한다.
		*/
		if(ReadyQueue.currentSize>0) ctx = safe_dequeue(&ReadyQueue);
		else continue;
		if(ctx==NULL) continue;
	//	fprintf(stdout,"queuesize %d\n",ReadyQueue.currentSize);
		//여기까지 데이터 읽기
			fprintf(stderr,"processing\n");
			if (status < 0){
				die(ctx->h);
			}

                        
			switch (ipq_message_type(ctx->buf)) {
				case NLMSG_ERROR:
					fprintf(stderr, "Received error message %d\n",
					ipq_get_msgerr(ctx->buf));
					break;
				case IPQM_PACKET: {
					ipq_packet_msg_t *m = ipq_get_packet(ctx->buf);
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
					ip =(struct iphdr *) packet;
					tcp =(struct tcphdr *) packet + sizeof(struct iphdr);
					dump(tcp,240);
					glist_rewind(&ModuleList);
					/*****
					*  run modules 
					*/					
					while((module=(module_prototype)glist_next(&ModuleList))!=NULL){
						result = (*module)(ctx->h,m,(const char*)tcp,240);
						if(result == NF_DROP) break;
					}
					/*
					* 위에서 처리 한 결과대로 처리한다.
					* verdict는 한번만 호출되어야 하는것 같다.
					*/
					status = ipq_set_verdict(ctx->h, m->packet_id,result, 0, NULL);
					if (status < 0) die(ctx->h);
					break;
        		                }
				default:
					fprintf(stderr, "Unknown message type!\n");
					break;
			}
	} while (1);
        
	ipq_destroy_handle(ctx->h);
	return 0;


}

