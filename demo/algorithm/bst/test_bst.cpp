#include "bst.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const* argv[])
{
	const int keys[] = {0,1,2,3,4,5,6,7,8,9,10};
	const int N = sizeof(keys)/sizeof(int);

	/*
	 *struct bst *t = bst_create(keys[0]);
	 *bst_insert(&t,keys[1]);
	 *bst_insert(&t,keys[2]);
	 *bst_insert(&t,keys[3]);
	 *bst_insert(&t,keys[4]);
	 */

	struct bst *t = bst_create(1);
	bst_insert(&t,2);
	bst_insert(&t,3);
	bst_insert(&t,4);
	bst_insert(&t,5);
	//bst_insert(&t,6);

	bst_traverse(t,BST_INORDER);

	return 0;
}
