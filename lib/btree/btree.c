#include <stdio.h>
#include <string.h>


#define BTREE_LEFT_NODE 	-1
#define BTREE_RIGHT_NODE	1
#define BTREE_HERE	0
#define 

typedef void * gpointer;


typedef struct btreeNodeTag btreeNode;
typedef struct btreeNode btree;
typedef int (*btreeCompareFunc)(btree *tree,btreeNode *origin, gpointer data);


struct btreeNodeTag {
	btreeNode *parent;
	btreeNode *left;
	btreeNode *right;
	int minimum;
	int maximum;
	int *indexs;
	gpoiner *datas;
	int length;
};

struct btreeTag{
	btreeNode *rootNode;
	int bucketLength;
};


int btree_insertData(btree *tree,gpointer data,btreeCompareFunc compareFunc){
	btreeNode *node=NULL;
	int result=-1;
	result = btree_search(tree->root,gpointer data,compareFunc,&node);

	if(node != NULL){
		

	}

	
}
int btree_insertNode(btree *tree,btreeNode *node){
		
}
int btree_removeNode(btree *tree,btreeNode *node){

}
int btree_init(btree *tree,int length){
	btreeNode *rootNode;

	
	if(tree->root!=NULL){
		return -2;
	}

	/* btree */
	rootNode = (btreeNode *)malloc(sizeof(btreeNode));
	if(rootNode ==NULL){
		return -1;
	}

	tree->root = rootNode;
	tree->length = length;

	/*	root Node */
	rootNode->parent=NULL;
	rootNode->left=NULL;
	rootNode->right=NULL;
	rootNode->minimum=0;
	rootNode->maximum=0;
	rootNode->indexs= malloc(sizeof(int) * length);
	rootNode->datas= malloc(sizeof(gpointer) * length);
	if(rootNode->datas==NULL){
		return -1;
	}
	rootNode->length=0;
	retrun 1;
}

int btree_searchNode(btreeNode *origin,gpointer data,btreeCompareFunc compareFunc,btreeNode **target){
	btreeNode pointNode=origin;
	int result;
	while(1){
		result = compareFunc(pointNode,gpointer data);
		if(result == BTREE_LEFT_NODE){
			if(pointNode->left != NULL){
				pointNode==pointNode->left;
				continue;
			}else{
				*targetNode = pointNode;
				return BTREE_NODATA;
			}
		}else
		if(result == BTREE_RIGHT_NODE){
			if(pointNode->right != NULL){
				pointNode= pointNode->right;
				continue;
			}else{
				*targetNode = pointNode;
				return BTREE_NODATA;
			}
		}else
		if(result == BTREE_HERE){
			break;
		}
	}
	
	/* 해당 노드에서 몇번째에 있는지 확인 해아함 */

	*target=NULL;
	result = BTREE_NODATA;

	return result;
	
}


