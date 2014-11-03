#include "bst.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const* argv[])
{
	const int keys[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	const int n = sizeof(keys) / sizeof(int);

	struct bst *t = bst_create(keys[0]);
	for (int i = 1; i < n; i++)
		bst_insert(&t,keys[i]);

	bst_delete(&t,3);
	bst_delete(&t,8);

	//bst_traverse(t,BST_PREORDER   | BST_RECURSIVE);
	//bst_traverse(t,BST_INORDER    | BST_RECURSIVE);
	//bst_traverse(t,BST_POSTORDER  | BST_RECURSIVE);
	//bst_traverse(t,BST_PREORDER   | BST_STACK    );
	//bst_traverse(t,BST_INORDER    | BST_STACK    );
	//bst_traverse(t,BST_POSTORDER  | BST_STACK    );
	//bst_traverse(t,BST_PREORDER   | BST_STACK2   );
	//bst_traverse(t,BST_INORDER    | BST_STACK2   );
	//bst_traverse(t,BST_POSTORDER  | BST_STACK2   );
	//bst_traverse(t,BST_PREORDER   | BST_MORRIS   );
	//bst_traverse(t,BST_INORDER    | BST_MORRIS   );
	//bst_traverse(t,BST_PREORDER   | BST_DEBUG    );
	//bst_traverse(t,BST_INORDER    | BST_DEBUG    );
	//bst_traverse(t,BST_POSTORDER  | BST_DEBUG    );
	//bst_traverse(t,BST_LEVELORDER | BST_DEBUG    );
	//bst_traverse(t,BST_LEVELORDER);

	return 0;
}
