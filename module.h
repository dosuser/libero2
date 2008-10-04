#ifndef __MODULE_H
#define __MODULE_H
#include <sys/socket.h>// netlink.h need this header file
#include <linux/netlink.h>
#include <linux/netfilter_ipv4/ip_queue.h>
#include <libipq.h>

#include "lib/ds.h"

typedef int (*getModuleType)(void);
typedef int (*module_prototype)(const struct ipq_handle *h,ipq_packet_msg_t *ipq,const char *msg,unsigned int msgLength);
module_prototype insmod(const char *module,int insertPoint);
int LoadModules(GListInfo *moduleList);

#endif

