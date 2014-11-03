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
 * Delete node by key
 * Can only fail due to nonexisting key
 * The AVL tree remains intact if deletion fails
 */
void bst_delete(struct bst **t, const int key);

/*
 * bst_traverse() has three different modes. The mode is specified by the mode
 * parameter. Admissible mode values are defined by the following enum type.
 * So far, he following combinations are supported and behave as expected:
 * 	( * = PRE/IN/POST )
 * 	BST_(*)ORDER (default to BST_STACK)
 * 	BST_(*)ORDER | BST_RECURSIVE
 * 	BST_(*)ORDER | BST_STACK(2)
 * 	BST_(PRE/IN)ORDER | BST_MORRIS
 * 	BST_(*)ORDER | BST_DEBUG (use stack)
 * 	BST_LEVELORDER
 * 	BST_LEVELORDER | BST_DEBUG
 * Any other combination of flags is either utterly unsupported or capable of
 * leading to undefined behavior
 */
enum {
	BST_PREORDER  = 0x1,
	BST_INORDER   = 0x2,
	BST_POSTORDER = 0x3,
	BST_LEVELORDER= 0x4,

	BST_MASK      = 0xFF, // internal use

	BST_RECURSIVE = 0x1<<8, // default
	BST_STACK     = 0x1<<9,
	BST_STACK2    = 0x1<<10,
	BST_MORRIS    = 0x1<<11,
	BST_DEBUG     = 0x1<<12
};
void bst_traverse(struct bst *t, const int mode);

void bst_destroy(struct bst *t);

void bst_print_node(const struct bst *p);

#ifdef __cplusplus
}
#endif



#endif /* end of include guard */
