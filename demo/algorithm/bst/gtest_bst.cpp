#include "bst.h"
#include <gtest/gtest.h>

TEST(Traversal, Recursive) {
	const int keys[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	const int n = sizeof(keys) / sizeof(int);

	struct bst *t = bst_create(1);
	for (int i = 1; i < n; i++)
		bst_insert(&t,keys[i]);

	bst_delete(&t,3);
	bst_delete(&t,8);
	bst_delete(&t,14);

	printf("RECURSIVE TRAVERSAL\n");
	printf("PREORDER\n");
	bst_traverse(t,BST_PREORDER   | BST_RECURSIVE);
	printf("INORDER\n");
	bst_traverse(t,BST_INORDER    | BST_RECURSIVE);
	printf("POSTORDER\n");
	bst_traverse(t,BST_POSTORDER  | BST_RECURSIVE);

	bst_destroy(t);
}

TEST(Traversal, Stack) {
	const int keys[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	const int n = sizeof(keys) / sizeof(int);

	struct bst *t = bst_create(1);
	for (int i = 1; i < n; i++)
		bst_insert(&t,keys[i]);

	bst_delete(&t,3);
	bst_delete(&t,8);
	bst_delete(&t,14);

	printf("STACK TRAVERSAL\n");
	printf("PREORDER\n");
	bst_traverse(t,BST_PREORDER   | BST_STACK);
	printf("INORDER\n");
	bst_traverse(t,BST_INORDER    | BST_STACK);
	printf("POSTORDER\n");
	bst_traverse(t,BST_POSTORDER  | BST_STACK);

	bst_destroy(t);
}

TEST(Traversal, Morris) {
	const int keys[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	const int n = sizeof(keys) / sizeof(int);

	struct bst *t = bst_create(1);
	for (int i = 1; i < n; i++)
		bst_insert(&t,keys[i]);

	bst_delete(&t,3);
	bst_delete(&t,8);
	bst_delete(&t,14);

	printf("MORRIS TRAVERSAL\n");
	printf("PREORDER\n");
	bst_traverse(t,BST_PREORDER   | BST_MORRIS);
	printf("INORDER\n");
	bst_traverse(t,BST_INORDER    | BST_MORRIS);
	printf("POSTORDER\n");
	bst_traverse(t,BST_POSTORDER  | BST_MORRIS);

	bst_destroy(t);
}

TEST(Traversal, Stack2) {
	const int keys[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	const int n = sizeof(keys) / sizeof(int);

	struct bst *t = bst_create(1);
	for (int i = 1; i < n; i++)
		bst_insert(&t,keys[i]);

	bst_delete(&t,3);
	bst_delete(&t,8);
	bst_delete(&t,14);

	printf("STACK2 TRAVERSAL\n");
	printf("PREORDER\n");
	bst_traverse(t,BST_PREORDER   | BST_STACK2);
	printf("INORDER\n");
	bst_traverse(t,BST_INORDER    | BST_STACK2);
	printf("POSTORDER\n");
	bst_traverse(t,BST_POSTORDER  | BST_STACK2);

	bst_destroy(t);
}

TEST(Traversal, Level) {
	const int keys[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	const int n = sizeof(keys) / sizeof(int);

	struct bst *t = bst_create(1);
	for (int i = 1; i < n; i++)
		bst_insert(&t,keys[i]);

	bst_delete(&t,3);
	bst_delete(&t,8);
	bst_delete(&t,14);

	printf("LEVELORDER\n");
	bst_traverse(t,BST_LEVELORDER);

	bst_destroy(t);
}

TEST(Traversal, Debug) {
	const int keys[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	const int n = sizeof(keys) / sizeof(int);

	struct bst *t = bst_create(1);
	for (int i = 1; i < n; i++)
		bst_insert(&t,keys[i]);

	bst_delete(&t,3);
	bst_delete(&t,8);
	bst_delete(&t,14);

	printf("DEBUG TRAVERSAL\n");
	printf("PREORDER\n");
	bst_traverse(t,BST_PREORDER   | BST_DEBUG);
	printf("INORDER\n");
	bst_traverse(t,BST_INORDER    | BST_DEBUG);
	printf("POSTORDER\n");
	bst_traverse(t,BST_POSTORDER  | BST_DEBUG);
	printf("LEVELORDER\n");
	bst_traverse(t,BST_LEVELORDER | BST_DEBUG);

	bst_destroy(t);
}
