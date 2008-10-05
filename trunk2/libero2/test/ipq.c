/*
 * packet dumper using ip_queue.ko and netfilter/libipq
 *  bushi at mizi dot com
 *
 * sudo /sbin/modprobe ip_queue
 * gcc -O2 -s -o ipq ipq.c -lpthread -lipq -Wall
 * sudo ./ipq
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <setjmp.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <linux/types.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <linux/netfilter.h>
#include <libipq.h>

/* from linux kernel */
#include "list.h"
#ifndef list_first_entry
#  define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)
#endif

#ifndef countof
#  define countof(x) (sizeof(x)/sizeof(x[0]))
#endif

#define SERVER "219.240.12."

//#define RESCHED_THREAD		(1)
#define THREAD_SCHED_INTERVAL	(1)
#define BYTES_PER_LINE		(16)

#define BUFFSZ			(8096)

static const int sig_list[] = {
		SIGHUP,   SIGINT, SIGQUIT,  SIGILL, SIGTRAP, SIGABRT,  SIGIOT,
		SIGFPE,  SIGKILL, SIGUSR1, SIGSEGV, SIGPIPE, SIGTERM,
		/*SIGCHLD, SIGCONT, SIGSTOP, SIGTSTP,*/ SIGTTIN, SIGTTOU,
#ifdef SIGSTKFLT
		SIGSTKFLT,
#endif
};

typedef struct context_s {
	struct ipq_handle *ipq;
	unsigned char *buf;
	struct list_head msgs;
	pthread_t dump_pid;
	pthread_mutex_t mutex;

	pthread_cond_t cond;
	pthread_mutex_t wait;
} context_t;

typedef struct msg_s {
	struct list_head node;
	ipq_packet_msg_t pkt;
} msg_t;

static jmp_buf setjmp_buffer;

static void sig_handler(int sig)
{
	if (sig)
		fprintf(stderr, "-- sig %d --\n", sig);
	longjmp(setjmp_buffer, 1);
}

inline void __print_hex_line(FILE *out, const int nm, const char *buf, int len)
{
	char line_buf[nm * 3 + nm + 2 + 1];
	char tmp[4];
	unsigned char *b = (unsigned char *)buf;
	int i;

	memset(line_buf, 0x20, sizeof(line_buf));
	line_buf[nm * 3] = '|';
	line_buf[sizeof(line_buf)-2] = '|';
	
	for (i = 0; i < len; i++, b++) {
		sprintf(&tmp[0], "%02X", *b);
		if (isprint(*b))
			sprintf(&tmp[2], "%c", *b);
		else
			tmp[2] = ' ';
		line_buf[i*3 + 0] = tmp[0];
		line_buf[i*3 + 1] = tmp[1];
		line_buf[nm*3 + i + 1] = tmp[2];
	}

	line_buf[sizeof(line_buf)-1] = 0;
	fprintf(out, "%s\n", line_buf);
}

inline void print_hex(FILE *out, const char *buf, int len)
{
	int left = len;

	if (len <= 0)
		return;

	while (left) {
		int this_len = left > BYTES_PER_LINE ? BYTES_PER_LINE:left;
		__print_hex_line(out, BYTES_PER_LINE, buf, this_len);
		left -= this_len;
		buf += this_len;	
	}
}

inline void print_ascii(FILE *out, const char *buf, int len)
{
	int i;
	int need_lf = 1;

	if (len <= 0)
		return;

	if (buf[len - 1] == '\n')
		need_lf = 0;
		
	for (i=0; i < len; i++, buf++) {
		int c = *buf;
		if (isprint(c) || (c == '\n') || (c == '\r'))
			putc(c, out);	
		else
			putc(' ', out);
	}
	if (need_lf)
		putc('\n', out);
}

static void print_ip(FILE *out, const ipq_packet_msg_t *pkt)
{
	struct iphdr *iph = (struct iphdr *)&pkt->payload[0];
	unsigned int iphdr_len;
	int protocol;
	unsigned int src_ip, dest_ip, tot_len;
	char src_ip_str[16], dest_ip_str[16];

	src_ip = ntohl(iph->saddr);
	dest_ip = ntohl(iph->daddr);
	protocol = iph->protocol;
	iphdr_len = iph->ihl << 2;
	tot_len = ntohs(iph->tot_len);
	
	sprintf(src_ip_str, "%d.%d.%d.%d",
			(src_ip >> 24) & 0xff,
			(src_ip >> 16) & 0xff,
			(src_ip >> 8) & 0xff,
			(src_ip >> 0) & 0xff
		);
	sprintf(dest_ip_str, "%d.%d.%d.%d",
			(dest_ip >> 24) & 0xff,
			(dest_ip >> 16) & 0xff,
			(dest_ip >> 8) & 0xff,
			(dest_ip >> 0) & 0xff
		);

	switch (protocol) {
	case IPPROTO_TCP: {
		struct tcphdr *tcp;
		unsigned int src_port, dest_port;
		unsigned int tcpdata_offset;
		const unsigned char *payload;
		int payload_length, payload_offset;

		tcp = (struct tcphdr *)(&pkt->payload[0] + iphdr_len);
		tcpdata_offset = tcp->doff << 2;
		src_port = ntohs(tcp->source);
		dest_port = ntohs(tcp->dest);

		payload_offset = iphdr_len + tcpdata_offset;
		payload_length = tot_len - payload_offset;
		payload = &pkt->payload[payload_offset];

		if (!strncmp(src_ip_str, SERVER, strlen(SERVER))) {
			fprintf(out, "INFO: %s:%d -> ????:%d, %d of %d bytes\n",
					src_ip_str, src_port,
					dest_port,
					payload_length, pkt->data_len);
		} else {
			fprintf(out, "INFO: %s:%d <- ????:%d, %d of %d bytes\n",
					dest_ip_str, dest_port,
					src_port,
					payload_length, pkt->data_len);
		}

		if (dest_port == 80)
			print_ascii(out, (char*)payload, payload_length);
		else
			print_hex(out, (char*)payload, payload_length);
		}
		break;
#if 0
	case IPPROTO_UDP:
		struct udphdr *udp;
		udp = (struct udphdr *)(&pkt->payload[0] + (iph->ihl << 2));
		/* ToDo */
		break;
	case IPPROTO_ICMP:
		break;
#endif
	default:
		fprintf(out, "not supported protocol: %d\n", protocol);
	}	
}

inline void queue_msg(context_t *cxt, msg_t *msg)
{
	INIT_LIST_HEAD(&msg->node);
	pthread_mutex_lock(&cxt->mutex);
	list_add_tail(&msg->node, &cxt->msgs);
	pthread_mutex_unlock(&cxt->mutex);
}

inline msg_t *dequeue_msg(context_t *cxt)
{
	msg_t *msg = NULL;
	pthread_mutex_lock(&cxt->mutex);
	if (!list_empty(&cxt->msgs)) {
		msg = list_first_entry(&cxt->msgs, msg_t, node);
		list_del(&msg->node);
	}
	pthread_mutex_unlock(&cxt->mutex);
	return msg;
}

inline void resched_thread(context_t *cxt)
{
	pthread_mutex_lock(&cxt->wait);
	pthread_cond_signal(&cxt->cond);
	pthread_mutex_unlock(&cxt->wait);
}

static void dump_all(context_t *cxt, const ipq_packet_msg_t *pkt, FILE *out)
{
	char *data;
	int data_len;

	data_len = (int)pkt->data_len - (sizeof(struct iphdr) + 32);
	data = (char*)&pkt->payload[sizeof(struct iphdr) + 32];

//	print_hex(out, (char*)&pkt->payload[0], (int)pkt->data_len);
	print_ip(out, pkt);
//	print_ascii(out, data, data_len);

	fflush(out);
}

static void dump_thread_cleanup(void *arg)
{
	/* nothing to do */
	fprintf(stderr, "\nthread killed\n");
}

static void *dump_thread(void *arg)
{
	context_t *cxt = arg;
	msg_t *msg;
	struct timespec ts;
	struct timeval tp;

	pthread_cleanup_push(dump_thread_cleanup, cxt)

	while (1) {
		int ret;
		pthread_testcancel();

		pthread_mutex_lock(&cxt->wait);
		gettimeofday(&tp, NULL);
		ts.tv_sec  = tp.tv_sec;
		ts.tv_nsec = tp.tv_usec * 1000;
		ts.tv_sec += THREAD_SCHED_INTERVAL;
		ret = pthread_cond_timedwait(&cxt->cond, &cxt->wait, &ts);
		pthread_mutex_unlock(&cxt->wait);

		if (ret && (ret != ETIMEDOUT)) {
			perror("thread::wait");
			break;
		}

		while ((msg = dequeue_msg(cxt))) {
			pthread_testcancel();

			dump_all(cxt, &msg->pkt, stdout);

			free(msg);
		}
	}

	pthread_cleanup_pop(1);

	return (void**)NULL;
}

static int init_context(context_t *cxt)
{
	int i;
	struct ipq_handle *h;

	fprintf(stderr, "\ninitialising...\n");

	h = ipq_create_handle(0, PF_INET);
	if (!h) {
		ipq_perror("ipq_create_handle()");
		return -1;
	}

	if (ipq_set_mode(h, IPQ_COPY_PACKET, BUFFSZ) < 0) {
		ipq_perror("ipq_set_mode()");
		ipq_destroy_handle(h);
		return -1;
	}

	cxt->buf = malloc(BUFFSZ);
	if (!cxt->buf) {
		perror("malloc()");
		ipq_destroy_handle(h);
		return -1;
	}

	cxt->ipq = h;
	INIT_LIST_HEAD(&cxt->msgs);
	pthread_mutex_init(&cxt->mutex, NULL);
	pthread_mutex_init(&cxt->wait, NULL);
	pthread_cond_init(&cxt->cond, NULL);

	pthread_create(&cxt->dump_pid, NULL, dump_thread, cxt);

	for (i=0;i<countof(sig_list);i++) {
		if (signal(sig_list[i], sig_handler) == SIG_ERR) {
			/* anyway, continue */	
		}
	}

	return 0;
}

static void deinit_context(context_t *cxt)
{
	int i;
	msg_t *msg, *tmp;

	fprintf(stderr, "\ndeinitialising...\n");

	for (i=0;i<countof(sig_list);i++) {
		signal(sig_list[i], SIG_DFL);
	}

	if (cxt->dump_pid) {
		pthread_cancel(cxt->dump_pid);
		pthread_join(cxt->dump_pid, (void**)NULL);
		cxt->dump_pid = 0;
	}

	ipq_destroy_handle(cxt->ipq);
	cxt->ipq = NULL;

	free(cxt->buf);
	cxt->buf = NULL;

	pthread_mutex_lock(&cxt->mutex);
	list_for_each_entry_safe(msg, tmp, &cxt->msgs, node) {
		list_del(&msg->node);
		free(msg);
	}
	pthread_mutex_unlock(&cxt->mutex);

	pthread_mutex_destroy(&cxt->mutex);
	pthread_mutex_destroy(&cxt->wait);
	pthread_cond_destroy(&cxt->cond);
}

static inline int proc_packet(context_t *cxt)
{
	ipq_packet_msg_t *pkt = ipq_get_packet(cxt->buf);
	msg_t *new_msg;
	int ret;

	/* accept first of all */
	ret = ipq_set_verdict(cxt->ipq, pkt->packet_id, NF_ACCEPT, 0, 0);

	/* queue  */
	new_msg = malloc(sizeof(msg_t) + pkt->data_len);
	if (!new_msg) {
		perror("malloc()");
		return ENOMEM;
	}
	memcpy(&new_msg->pkt, pkt, sizeof(*pkt) + pkt->data_len);
	queue_msg(cxt, new_msg);

#if RESCHED_THREAD
	/* wakeup immediately */
	resched_thread(cxt);
#endif

	return ret;
}

static void proc_ipq(context_t *cxt)
{
	int ret, type;
	ipq_packet_msg_t *m=NULL;
	fprintf(stderr, "\nprocessing...\n");

	do {
		ret = ipq_read(cxt->ipq, cxt->buf, BUFFSZ, 0);
		if (ret < 0) {
			/* error */
			ipq_perror("ipq_read()");
			return;
		}
		if (ret == 0) {
			/* timed-out */
			continue;
		}



		type = ipq_message_type(cxt->buf);
		switch (type) {
		case IPQM_PACKET:
			ret = proc_packet(cxt);
			if (ret < 0) {
				fprintf(stderr, "ipq: %s\n", ipq_errstr());
			}
			m= ipq_get_packet(cxt->buf);


			ipq_set_verdict(cxt->ipq,m->packet_id,NF_ACCEPT,0,NULL);
			break;
		case NLMSG_ERROR:
			ret = ipq_get_msgerr(cxt->buf);
			fprintf(stderr, "NETLINK: %s\n", strerror(ret));
			return;
		default:
			fprintf(stderr, "unknown packet type.\n");
		}
	} while (1);
}

int main(int argc, char **argv)
{
	context_t context = { .ipq = NULL, .buf = NULL};

	if (init_context(&context)) {
		return EXIT_FAILURE;
	}

	if (!setjmp(setjmp_buffer)) {
		proc_ipq(&context);
	}

	deinit_context(&context);	

	return EXIT_SUCCESS;
}
