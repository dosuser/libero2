#ifndef _GLIST_H
#define _GLIST_H

#include <gtype.h>


typedef struct _GList GList;
struct _GList{
	gpointer data;
	struct GList *next;
	struct GList *prev;
};


struct _GListInfo{
	GList* firstNode;
	GList* lastNode;
	GList* currentNode;
};
typedef struct _GListInfo GListInfo;

GList* glist_append(GListInfo *list ,gpointer data);
GList* glist_nth(GListInfo *list,int position);
GList* glist_insert(GListInfo *list, gpointer data,int position);
gpointer glist_next(GListInfo *list);
gpointer glist_prev(GListInfo *list);
GListInfo* glist_ndup(GListInfo *list);
void glist_rewind(GListInfo *list);
#endif
