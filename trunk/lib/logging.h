#include <pthread.h>
#include <stdio.h>

#ifndef LOGGING
#define LOGGING

#define THDLOG(level,fm,args...) \
pthread_mutex_lock(&mutexDebug);\
dlog(level,fm,args);\
pthread_mutex_unlock(&mutexDebug);


static int TOKEN=0;
static int DLEVEL=0;
static FILE *DEBUG_FP=NULL;
static char *DEBUG_FILENAME=NULL;
pthread_mutex_t mutexDebug = PTHREAD_MUTEX_INITIALIZER;
void dlog(int level, char *format, ...);


#endif
