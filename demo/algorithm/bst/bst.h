#ifndef _BST_H_
#define _BST_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * AVL Binary Search Tree
 * No recursion or parent pointers
 */


struct bst{
	int key;
	int bf; // balance factor
	struct bst *left;
	struct bst *right;
};

/*
 * Return the pointer to the root node
 * Return NULL in case of allocation failure
 */
struct bst *bst_create(const int key);

/*
 * Return the pointer to the newly inserted node
 * Return NULL in case of allocation failure or collision with existing node
 * *t may be changed, points to the new root node
 */
struct bst *bst_insert(struct bst **t, const int key);

/*
 * Return the pointer to the first node with value val
 * Return NULL if no match was found
 */
struct bst *bst_search(struct bst *t, const int key);

/*
 * Only use me to delete a node returned by bst_search() or bst_insert()
 * *t may be changed, points to the new root node
 */
void bst_delete(struct bst **t, const struct bst *node);

/*
 * bst_traverse() has three different modes. The mode is specified by the mode
 * parameter. Admissible mode values are defined by the following enum type.
 */
enum {
	BST_PREORDER  = 0x1,
	BST_INORDER   = 0x2,
	BST_POSTORDER = 0x3,

	BST_RECURSIVE = 0x1<<4,
	BST_STACK     = 0x2<<4,
	BST_MORRIS    = 0x3<<4
};
void bst_traverse(struct bst *t, const int mode);

void bst_destroy(struct bst *t);

void bst_print_node(const struct bst *p);

#ifdef __cplusplus
}
#endif



#endif /* end of include guard */
