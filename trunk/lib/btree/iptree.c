#include "../../siteinfo.c"

int ipBtreeCompareFunc(btree *tree, btreeNode  *origin, gpointer data){

	cond *originData;
	cond *compareData;

	int loop;

	originData = origin->datas[0];
	if(originData->daddr > compareData->daddr){
		return BTREE_LEFT_NODE;
	}
	
	originData = origin->datas[origin->maximum];
	if(originData->daddr < compareData->daddr){
		if(origin->length == tree->length ) return BTREE_RIGHT_NODE;
		return BTREE_HERE;
	}
	

	/*  binary search */ 
	for(loop=0;loop<origin->length;loop++){
		originData = origin->datas[loop];
		
		if(originData-




}
