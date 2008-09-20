#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <gqueue.h>
#define ALLOCATION_ERROR -1

int queue_init(gqueue *pQueue,int queueSize){
	if(pQueue==NULL){
		return -1;
	}
	pQueue->queueSize = queueSize;
	pQueue->buffer =(gpointer *)malloc(sizeof(gpointer) * queueSize);
	if(pQueue->buffer==NULL) return ALLOCATION_ERROR;

	pQueue->front = 0;
	pQueue->rear = 0;
	pQueue->currentSize=0;
	memset(pQueue->buffer,0,queueSize);
	return 1;
}

int isEmpty(gqueue *pQueue){
	return pQueue->front==pQueue->rear;
}
int isFull(gqueue *pQueue){
	return pQueue->front==(pQueue->rear +1)% pQueue->queueSize;
}
int front_go(gqueue *pQueue){
	pQueue->front++;
	pQueue->front %= pQueue->queueSize;
	pQueue->currentSize--;
	return pQueue->front;
}
int rear_go(gqueue *pQueue){
	pQueue->rear++;
	pQueue->rear %= pQueue->queueSize;
	pQueue->currentSize++;
	return pQueue->rear;
}


int queue_write(gqueue *pQueue,gpointer target){
	if(isFull(pQueue)) return ERROR_BUFFER_FULL;
	pQueue->buffer[ pQueue->rear]  = target;
	rear_go(pQueue);
	return 1;
}

int queue_read(gqueue *pQueue,gpointer *target){
	if(isEmpty(pQueue)) return 0;
	*target=pQueue->buffer[pQueue->front];
	front_go(pQueue);
	return 1;
}

int queue_top(gqueue *pQueue,gpointer *target){
	if(isEmpty(pQueue)) return 0;
	target=(void *)*(pQueue->buffer + pQueue->front );
	return 1;
}
