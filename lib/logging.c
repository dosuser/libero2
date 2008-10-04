#include <stdio.h>

#include <stdarg.h>
#include "logging.h"
void dlog(int level, char *format, ...){
	va_list ap;
	if(level < DLEVEL) return;
	if(DEBUG_FP==NULL) DEBUG_FP = fopen( "log.txt", "a");

	va_start( ap, format);
	vfprintf( DEBUG_FP, format, ap);
	va_end(ap);
	fflush(DEBUG_FP);
}



