#include "../../siteinfo.c"
#include "ttree.h"
#include "iptree.h"
int ipTTreeCompareFunc(ttree *tree, ttreeNode *origin, gpointer data){

	cond *originData;
	cond *compareData;

	int loop;

	/* for binary search */
	int lowerBound, upperBound, middle;
	originData = origin->datas[0];
	if(originData->daddr > compareData->daddr){
		return TTREE_LEFT_NODE;
	}
	
	originData = origin->datas[origin->maximum];
	if(originData->daddr < compareData->daddr){
		if(origin->length == tree->length ) return TTREE_RIGHT_NODE;
		return TTREE_HERE;
	}
	

	/*  binary search */ 
	upperBound = origin->maximum;
	lowerBound = origin->minimum;

	while(lowerBound <= upperBound ){
		middle = (lowerBound + upperBound) / 2;
		if( ((cond *)origin->datas[middle])->daddr == ((cond *)data)->daddr){
			return middle;//찾기 성공
		}
		if( ((cond *)origin->datas[middle])->daddr > ((cond *)data)->daddr){
			upperBound = middle -1;
		}
		else {
			lowerBound = middle +1;
			
		}
		return TTREE_NOTFOUND;
	}
	
}
