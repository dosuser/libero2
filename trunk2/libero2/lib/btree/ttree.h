#ifndef __TTREE_H
#define __TTREE_H


#define TTREE_LEFT_NODE 	-1
#define TTREE_RIGHT_NODE	1
#define TTREE_HERE	0
#define TTREE_NOTFOUND -2
#define TTREE_NODATA -3


typedef void * gpointer;


typedef struct ttreeNodeTag ttreeNode;
typedef struct ttreeTag ttree;
typedef int (*ttreeCompareFunc)(ttree *tree,ttreeNode *origin, gpointer data);



struct ttreeNodeTag {
	ttreeNode *parent;
	ttreeNode *left;
	ttreeNode *right;
	int minimum;
	int maximum;
	int *indexs;
	gpointer *datas;
	int length;
};

struct ttreeTag{
	ttreeNode *rootNode;
	int length;
};

int ttree_insertData(ttree *tree,gpointer data,ttreeCompareFunc compareFunc);

int ttree_init(ttree *tree,int length);

int ttree_searchNode(ttree *tree,ttreeNode *origin,gpointer data,ttreeCompareFunc compareFunc,ttreeNode **targetNode);







#endif
