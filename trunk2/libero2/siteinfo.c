#ifndef __SITE_INFO_H
#define __SITE_INFO_H

#include <sys/types.h>

typedef struct _cond cond;
struct _cond {

	u_int32_t saddr;
	u_int32_t daddr;
	char *site;
	char *fullURL;

	time_t due;
	char extCode;
	void *ext;
};

		
#endif
