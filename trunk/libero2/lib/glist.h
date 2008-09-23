#ifndef _GLIST_H
#define _GLIST_H

#include <gtype.h>


typedef struct _GList GList;
struct _GList{
	gpointer data;
	GList *next;
	GList *prev;
};


struct _GListInfo{
	GList* firstNode;
	GList* lastNode;
	GList* currentNode;
	int length;
};
typedef struct _GListInfo GListInfo;

GList* glist_append(GListInfo *list ,gpointer data);
GList* glist_nth(GListInfo *list,int position);
GList* glist_insert(GListInfo *list, gpointer data,int position);
gpointer glist_next(GListInfo *list);
gpointer glist_prev(GListInfo *list);
GListInfo* glist_ndup(GListInfo *list);
int glist_remove(GListInfo *list, GList *node);
int glist_remove_by_num(GListInfo *list, int position);
void glist_rewind(GListInfo *list);

#endif
