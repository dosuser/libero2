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
#include <libero.h>
#include <string.h>

#define MAXIMUM_WINDOW_SIZE 30000
GListInfo ModuleList;
gqueue ReadyQueue;

void *patching(void *);
void *processing(void *);

pthread_mutex_t queueLock= PTHREAD_MUTEX_INITIALIZER;


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
	pthread_create(&process3, NULL,processing,NULL);

	pthread_join(th1, NULL);
	pthread_join(process3, NULL);
	
	return 0;
}
int safe_queue(gqueue *queue,gpointer data){
	dlog(0,"append\n");
	queue_write(queue,data);
	return 1;
}
gpointer safe_dequeue(gqueue *queue){
	gpointer *node;
	queue_read(queue,(gpointer *)&node);
	if(node!=NULL) dlog(0,"dequeue\n");
	return node;
}
void *patching(void *t){
	/**************
	* 변수 설정
	*************/
	int status;/*{{{*/
	char *buf;
	struct ipq_handle *h;
	packet_ctx *ctx;
	h = (struct ipq_handle *)t;

/*}}}*/
	while(1){
		ctx = (packet_ctx *)malloc(sizeof(packet_ctx));
		ctx->buf=(char *)malloc(sizeof(char) * BUFSIZE);
		ctx->status= ipq_read(h,ctx->buf,BUFSIZE,0);
		if(ctx->status < 0){
			dlog(0,"err!\n");
			die(h);
		}
		ctx->h = h;
		dlog(0,"buffer size :%d\n",ReadyQueue.currentSize);
		
		if(ReadyQueue.currentSize<30){
			pthread_mutex_lock(&queueLock);
			safe_queue(&ReadyQueue,ctx);
			pthread_mutex_unlock(&queueLock);		
		}
		else{
					dlog(0,stderr,"sleep 1 second\n");
					sleep(1);
		}

	}

}




void *processing(void *t){
	packet_ctx *ctx;
	int status;	

	do{
		/*********************************************
		* process쓰레드 들이 동시에 큐에 달라 붙어서 같은 자료를 처리 할수 있다.
		* 이 경우 어느 한쪽에서 메시지를 처리하고 메모리를 해제하면 나머지 쪽에서 문제가 생길수 있다. 
		* 그래서 큐에서 하나의 쓰레드에만 값을 넘겨 줘야 한다.
		**************************************************/
		pthread_mutex_lock(&queueLock);/*{{{*/
		if(ReadyQueue.currentSize>0){
			ctx = safe_dequeue(&ReadyQueue);
			pthread_mutex_unlock(&queueLock);
		}
		else{
			pthread_mutex_unlock(&queueLock);
			continue;
		}
/*}}}*/

	//에러 처리
		if(ctx==NULL) continue;/*{{{*/
			dlog(0,"processing\n");
			if (status < 0){
				die(ctx->h);
			}
/*}}}*/
                        
			switch (ipq_message_type(ctx->buf)) {
				//에러 처리
				case NLMSG_ERROR:
					dlog(0, "Received error message %d\n",
					ipq_get_msgerr(ctx->buf));
					break;
				case IPQM_PACKET: {
					/**********************
					*초기 변수 설정
					*
					*************************/
					ipq_packet_msg_t *m = ipq_get_packet(ctx->buf);/*{{{*/
					struct tcphdr *tcp=NULL;
					struct iphdr *ip=NULL;
					unsigned char *packet = NULL;
					unsigned char *tcpPayload=NULL;
					module_prototype module= NULL;
					int result=0;
					GListInfo *moduleList;
					sin_packet_info *packetInfo = malloc(sizeof(sin_packet_info));

					memset(packetInfo,0,sizeof(sin_packet_info));
					
					


					dump_ipq(m);/*}}}*/

					if(m->data_len > sizeof(struct iphdr)){
						packet = (unsigned char *)m + sizeof(ipq_packet_msg_t);//sizeof(*m)
					}
					
					ip =(struct iphdr *) packet;
					if(ip->protocol != 6){ //6 is TCP protocol number
						ipq_set_verdict(ctx->h,m->packet_id,NF_ACCEPT,0,NULL);
					}
					
					tcp =(struct tcphdr *) packet + sizeof(struct iphdr);
					dump_ip((struct iphdr *)packet);
					dump((char *)tcp,240);
					
					/***********************************
						packetInfo 설정

					***********************************/
				
					if(strcmp(m->outdev_name,"eth0")==0){
						packetInfo->direction = C2OUT;
					}else if(strcmp(m->indev_name,"eth0")==0){
						packetInfo->direction = OUT2C;
					}
					packetInfo->verdicted = 0;
					packetInfo->modified=0;
					packetInfo->action =SIN_ACCEPT;

				

					/************************
						분류기

					************************/
					/*
					while(0){
						char bound=-1;
						gpointer n;
						if(bound==C2O){//만일 패킷이 클라이언트에서 외부로 나가는 경우이면
							
							n = tree_find(&BWList, ip->daddr, NULL);
							if(n != NULL){
								cond *data = (cond *)n;
								if(data->type==DENY){
									if(data->action == CONNECTION_CLOSE){
										sessionManager(ctx->h,m->packet_id,CONNECTION_CLOSE);
										continue;// 다음 패킷 처리
								}
								}
								if(data->type==ALLOW){
									if(data->action == CONNECTION_ALLOW){
										sessionManager(ctx->h,m->packet_id,CONNECTION_ALLOW);
										continue;// 다음 패킷 처리

									}
								}
							}
						}
					}
					*/
					


							



					/**********************
					*  run modules 
					************************/					
					glist_rewind(&ModuleList);
					while((module=(module_prototype)glist_next(&ModuleList))!=NULL){
						result = (*module)(ctx->h,m,(const char*)tcp,240);
						/**********
						*허용이 아닌 경우 중단으로 해야 한다. 
						*이유는 ACCEPT이지만 다른 종류의 메시지를 보내야 하는 경우도 있기 때문이다.
						*다만 이 경우에 어떻게 하는게 좋을지 아직 모르겠다.
						***********/
						if(result == NF_DROP) break;
					}
					/*********************
					* 위에서 처리 한 결과대로 처리한다.
					* verdict는 한번만 호출되어야 하는것 같다.
					***********************/



					/********************
						윈도우 사이즈 조절
					*******************/
					if(result==NF_ACCEPT && ip->protocol == 6){
						uint16_t win;
						#if __BYTE_ORDER == __LITTLE_ENDIAN
							win = ntohs(tcp->window);
							if(win>MAXIMUM_WINDOW_SIZE){
								tcp->window =htons(MAXIMUM_WINDOW_SIZE);
								packetInfo->modified=1;
								//패킷을 수정 했으니 이것을 verdict 하도록 수정해야한다.

							}
						#elif __BYTE_ORDER == __BIG_ENDIAN
							if(win>MAXIMUM_WINDOW_SIZE) {
								tcp->window = MAXIMUM_WINDOW_SIZE;
								packetInfo->modified=1;
								//패킷을 수정 했으니 이것을 verdict 하도록 수정해야한다. 
						#endif
					}
					if(	packetInfo->modified == 0){
						status = ipq_set_verdict(ctx->h, m->packet_id,result, 0, NULL);
					}else{
						/*******************
							패킷 수정됐으니 체크섬 재계산-_-
						*******************/
						status = ipq_set_verdict(ctx->h, m->packet_id,result, 0,NULL);

					}

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
