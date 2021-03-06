#include <stdio.h>
#include <linux/netfilter.h>
#include <libipq.h>
#include <pcre.h>
#include "../lib/logging.h"
#include "../libero.h"
#define OVECCOUNT 3*20

int getModuleType(){
	
}

int module_main(const struct ipq_handle *h,ipq_packet_msg_t *ipq,const char *msg,unsigned int msgLength){

	pcre *rex1=NULL;
	const char *errMsg=NULL;
	int errOffSet=-1;
	int opt=0;
	int ovector[OVECCOUNT];
	int rc=0;
	int status=-1;
	
	rex1 = pcre_compile("XSS",opt,&errMsg,&errOffSet,NULL);
	rc = pcre_exec(rex1,NULL,msg,msgLength,0,0,ovector,OVECCOUNT);

	if(rc < 0){
		switch(rc){
						case PCRE_ERROR_NOMATCH:
							dlog(0,"test1: there is no XSS\n");
							return SIN_CONTINUE;
							break;
						default:
							dlog(0,"error regualar expression was broken!\n");
							exit(1);
		}
		pcre_free(rex1);
	}

	if(rc==0){
					rc=OVECCOUNT/3;
	}
	dlog(0,"denyed\n");
	pcre_free(rex1);
	return SIN_DENY;
}
