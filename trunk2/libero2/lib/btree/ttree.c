#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ttree.h"


int ttree_insertData(ttree *tree,gpointer data,ttreeCompareFunc compareFunc){
	ttreeNode *node=NULL;
	ttreeNode *newNode=NULL;
	int result=-1;
	int loop;
	//값을 넣을 노드를 찾는다.
	result = (*compareFunc)(tree,tree->rootNode,data,compareFunc,&node);
	//값을 넣을 노드가 발견 되지 않으면 노드를 추가한다. 
	
	if(result == TTREE_LEFT_NODE){
		newNode =  ttree_createNode(tree);
		node->left = newNode;
		newNode->parent=node;
	}else if(result == TTREE_RIGHT_NODE){
		newNode = ttree_createNode(tree);
		node->right = newNode;
		newNode->parent =node;
	}else if(result == TTREE_HERE){
		newNode = node;
	}
	

	if(newNode->length < tree->length){
		for(loop=0;loop<tree->length){
			if(tree->datas[loop]==NULL){
				tree->datas[loop] = data;
			}
		}
	}



	


}
int ttree_insertNode(ttree *tree,ttreeNode *node){
		
}
int ttree_removeNode(ttree *tree,ttreeNode *node){

}
int ttree_init(ttree *tree,int length){/*{{{*/
	ttreeNode *rootNode;

	
	if(tree->rootNode!=NULL){
		return -2;
	}

	rootNode = ttree_createNode(tree);
	tree->rootNode = rootNode;
	tree->length = length;
	

	return 1;
}/*}}}*/


ttreeNode *ttree_createNode(ttree *tree){
	ttreeNode *node;
	
	/* ttree */
	node = (ttreeNode *)malloc(sizeof(ttreeNode));
	if(node ==NULL){
		return -1;
	}
	/*	root Node */
	node->parent=NULL;
	node->left=NULL;
	node->right=NULL;
	node->minimum=-1;
	node->maximum=-1;
	node->indexs= malloc(sizeof(int) * tree->length);
	node->datas= malloc(sizeof(gpointer) * tree->length);
	if(node->datas==NULL){
		return -1;
	}
	node->length=0;

	return node;
}

int ttree_searchNode(ttree *tree,ttreeNode *origin,gpointer data,ttreeCompareFunc compareFunc,ttreeNode **targetNode){/*{{{*/
	ttreeNode *pointNode=origin;
	int result;
	while(1){
		result = compareFunc(tree,pointNode,data);
		if(result == TTREE_LEFT_NODE){
			if(pointNode->left != NULL){
				pointNode==pointNode->left;
				continue;
			}else{
				*targetNode = pointNode;
				return TTREE_NOTFOUND;
			}
		}else
		if(result == TTREE_RIGHT_NODE){
			if(pointNode->right != NULL){
				pointNode= pointNode->right;
				continue;
			}else{
				*targetNode = pointNode;
				return TTREE_NOTFOUND;
			}
		}else
		if(result == TTREE_HERE){
			*targetNode = pointNode;
			return TTREE_HERE;
		}
	}
	
	/* 해당 노드에서 몇번째에 있는지 확인 해아함 */
	
}/*}}}*/


