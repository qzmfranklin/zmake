#include "bst.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const* argv[])
{
	//const int keys[] = {3,9,7,5,4,8,2,6,1,10};
	//const int keys[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14};
	const int keys[] = {1,2,3,4,5,6,7,8,9,10};
	const int n = sizeof(keys) / sizeof(int);
	//const int n = 20;
	//int keys[n];
	//for (int i = 0; i < n; i++)
		//keys[i] = rand() % 20;

	//printf("input array = \n");
	//for (int i = 0; i < n; i++)
		//printf("%d\n",keys[i]);
	//printf("\n");


	struct bst *t = bst_create(keys[0]);
	for (int i = 1; i < n; i++) {
		bst_insert(&t,keys[i]);
	}

	//bst_traverse(t,BST_PREORDER | BST_RECURSIVE);
	//bst_traverse(t,BST_INORDER | BST_RECURSIVE);
	bst_traverse(t,BST_POSTORDER | BST_RECURSIVE);
	//bst_traverse(t,BST_PREORDER | BST_STACK);
	//bst_traverse(t,BST_INORDER | BST_STACK);
	bst_traverse(t,BST_POSTORDER | BST_STACK);
	//bst_traverse(t,BST_INORDER | BST_MORRIS);
	//bst_traverse(t,BST_POSTORDER | BST_RECURSIVE);

	return 0;
}
