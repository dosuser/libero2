#ifndef ANTS_QUEUE_H
#define ANTS_QUEUE_H

#define ERROR_BUFFER_FULL -10
#define ERROR_BUFFER_EMPTY -20
#define QUEUE_BUFFER_SIZE 1000

#include "gtype.h"

struct queue_info {
	gpointer *buffer;
	int front;
	int rear;
	int queueSize;
	int currentSize;
};

typedef struct queue_info gqueue;
int queue_init(gqueue *pQueue,int queueSize);
int isEmpty(gqueue *pQueue);
int isFull(gqueue *pQueue);
int queue_writeg(gqueue *pQueue,gpointer node);
int queue_read(gqueue *pQueue,gpointer *node);
int queue_top(gqueue *pQueue,gpointer *node);


#endif
