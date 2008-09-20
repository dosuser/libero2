#include <glist.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void glist_init(GListInfo *list){
	list->firstNode=NULL;
	list->lastNode=NULL;
	list->currentNode=NULL;
}

GList* glist_append(GListInfo *list ,gpointer data){
	GList *new;
	new=(GList *)malloc(sizeof(GList));
	new->prev = (gpointer)list->lastNode;
	new->data = (gpointer)data;
	if(list->firstNode==NULL){
		list->firstNode=new;
		list->currentNode=new;
		list->lastNode=new;
	}else{
		list->lastNode->next = (struct GList *)new;
	}
		list->lastNode = new;
}
GList* glist_nth(GListInfo *list,int position){
	GList *tmp;
	int l;
	tmp = list->firstNode;
	for(l=0;l<position;l++){
		if(tmp == (GList *)tmp->next) return NULL;
		tmp=(GList *)tmp->next;
	}

	return tmp;
}
GList* glist_insert(GListInfo *list, gpointer data,int position){
	int loop=0;
	GList *tmp=glist_nth(list,position);
	GList *new=(GList *)malloc(sizeof(GList));

	if(tmp ==NULL) return NULL;
	new->prev = (struct GList *)tmp->prev;
	new->next = (struct GList *)tmp;

	tmp->prev = (struct GList *)new;
	return new;
	
}
GList* glist_remove(GListInfo *list, gpointer data);
GList* glist_remove_by_position(GListInfo *list,int position);

gpointer glist_next(GListInfo *list){
	GList *tmp=list->currentNode;
	if(list->currentNode==NULL) return NULL;
	if(list->currentNode==list->currentNode->next) return NULL;
	list->currentNode = list->currentNode->next;
	return tmp->data;
}
gpointer glist_prev(GListInfo *list){
	GList *tmp=list->currentNode;;
	if(list->currentNode->prev==NULL) return NULL;
	list->currentNode = list->currentNode->prev;
	return tmp->data;
}

GListInfo* glist_ndup(GListInfo *list){
	GListInfo *tmp = (GListInfo *) malloc(sizeof(GListInfo));
	memcpy(list,tmp,sizeof(GListInfo));
	return tmp;	
}
void glist_rewind(GListInfo *list){
	list->currentNode = list->firstNode;
}
