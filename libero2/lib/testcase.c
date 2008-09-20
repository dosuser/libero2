#include <stdio.h>
#include "gqueue.h"
#include "glist.h"



typedef struct __node2{
				int data;
} tnode;



int main(void){

				gqueue queue;
				tnode *t;
				GListInfo list;
				glist_init(&list);


				if(queue_init(&queue,20)<0){
								perror("init error");
				}


				tnode a;
				tnode b;
				a.data = 20;
				b.data = 30;

				tnode *d;

				glist_append(&list,(void *)&a);
				while((t =(tnode *) glist_next(&list))!=NULL){
				printf("%d\n",t->data);

				}

				return 0;
}




