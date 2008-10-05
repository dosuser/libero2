#include <stdio.h>
#include "ttree.h"
#include "iptree.h"


int main(void){



	int result;

	ttree tree;

	/* 초기화 */
	result = ttree_init(&tree,7);
	if(result < 0){

		fprintf(stderr,"ttree_init error : %d\n",result);
		return 0;
	
	}

	fprintf(stderr,"t tree inited\n");





	










	return 0;
}






