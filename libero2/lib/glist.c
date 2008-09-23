#include <glist.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void glist_init(GListInfo *list){
	list->firstNode=NULL;
	list->lastNode=NULL;
	list->currentNode=NULL;
	list->length=0;
}

GList* glist_prepend(GListInfo *list, gpointer data){
	GList *new;
	new = (GList *)malloc(sizeof(GList));
	new->prev = NULL;//맨 앞 노드이니 prev NULL
	new->next =  list->firstNode;//리스트의 처음이 없다고해도 null일 태니 괜찮음
	new->data = (gpointer)data;

	if(list->firstNode){ //맨앞 노드가 널이 아닌경우 
		list->firstNode->prev = new; //맨 앞 노드의 앞은 new
		list->firstNode=new;
	}else{
		list->firstNode=new;
	}

	list->firstNode = new;
	


}

GList* glist_append(GListInfo *list ,gpointer data){
	GList *new;
	new=(GList *)malloc(sizeof(GList));
	new->prev = (gpointer)list->lastNode;
	new->next = NULL;
	new->data = (gpointer)data;
	
	if(list->firstNode==NULL){
		list->firstNode=new;
		list->currentNode=new;
	}else{
		list->lastNode->next = new;
	}
	list->lastNode = new;
	list->length++;
}
/**
* n번째의 노드를 반환한다
* 
*		만약 n번째 노드가 존재 하지 않을 경우 NULL을 반환한다.
*		마지막 노드는 next값이 NULL이다.
*		노드가 없는 경우는 tmp 가 NULL인 경우이다.
*/
GList* glist_nth(GListInfo *list,int position){

	GList *tmp;
	int l;
	tmp = list->firstNode;
	if(list->length < position) return NULL;
	for(l=0;l<position;l++){
		if(tmp==NULL) return NULL;
		tmp=(GList *)tmp->next;
	}
	return tmp;
}
/**
*	position 0부터 시작하여 n-1까지의 사이에 값을 끼워 넣는다. 기존의 값은 뒤로 밀려난다.
*
*
*
*/
GList* glist_insert(GListInfo *list, gpointer data,int position){
	int loop=0;
	GList *tmp;
	GList *new;
	tmp =glist_nth(list,position);
	new = (GList *)malloc(sizeof(GList));

	if(tmp ==NULL) return NULL;
	
	//new 설정
	new->prev = tmp->prev;// tmp 자리를 교체 
	new->next = tmp; //tmp 는 밀려남
	
	//tmp의 앞 설정
	tmp->prev = new; //tmp의 앞은 new

	return new;
	
}

int glist_remove(GListInfo *list, GList *node){
	if(node){
		if (node->prev) node->prev->next = node->next;//이전 노드는 현재 노드의 다음을 가리킨다.
    if (node->next) node->next->prev = node->prev;//다음 노드는 현재 이전 노드를 가리킨다.
   
		if (node == list->firstNode) list->firstNode = list->firstNode->next;// 만약 노드가 첫번째 노드인 경우에는 리스트의 첫 노드를  현재 노드의 다음 노드로 가리킨다. 

		node->next = NULL;//만약에 접근을 한다고 해도 다음 없음
    node->prev = NULL;//만야겡 접근을 한다고 해도 이전 없음
		free((void *)node);//해제
		--list->length;
		return 1;
  }
	return -1;
}
int glist_remove_by_num(GListInfo *list, int position){
	GList *node = glist_nth(list,position);
	if(node==NULL) return -1;
	return glist_remove(list,node);
}	



GList* glist_remove_by_position(GListInfo *list,int position);
/**
* current 노드가 가리키고 있는 노드의 값을 리턴한다. current노드를 다음 노드로 옮긴다.
*
*/
gpointer glist_next(GListInfo *list){
	GList *tmp=list->currentNode;
	if(list->currentNode==NULL) return NULL;
	list->currentNode = list->currentNode->next;
	return tmp->data;
}
/**
* current 노드가 가리키고 있는 노드의 값을 리턴한다.
* current 노드를 이전 노드로 옮긴다.
*/
gpointer glist_prev(GListInfo *list){
	GList *tmp=list->currentNode;;
	if(list->currentNode==NULL) return NULL;
	list->currentNode = list->currentNode->prev;
	return tmp->data;
}

GListInfo* glist_ndup(GListInfo *list){
	GListInfo *tmp = (GListInfo *) malloc(sizeof(GListInfo));
	memcpy(list,tmp,sizeof(GListInfo));
	return tmp;	
}
/**
* current 가 첫번째 노드를 가리키게 한다.
*
*/
void glist_rewind(GListInfo *list){
	list->currentNode = list->firstNode;
}
