#ifndef _BST_H_
#define _BST_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Linear representation of binary search tree (BST):
 * 	enclosed in {}
 * 	separated by comma
 * 	# = null
 * 	integer = val
 * 	spaces (' ') are ignored
 */

struct bst {
	int key;
	int bf; // balance factor
	struct bst *left;
	struct bst *right;
};

/*
 * Create a binary tree of integral values from its linear representation *str.
 * Return an allocated empty node if str==NULL.
 * Return NULL if it fails to create the tree.
 */
struct bst *bst_from_string(const char *str);

/*
 * Output the linear representation of the tree to *out.
 * The string *out is null terminated and can be used in bst_create() directly.
 */
void bst_to_string(const struct bst *t, char *out);

/*
 * Deep copy a tree. Return the root pointer to the new tree. The new tree need
 * to be destroyed by bst_destroy().
 */
struct bst *bst_copy(const struct bst *t);

/*
 * 	1	is height balanced
 * 	0	not balanced
 */
int bst_is_balanced(const struct bst *t);

/* Depth/Height of binary search tree */
int bst_height(const struct bst *t);

void bst_insert(struct bst *t, const int val);

/*
 * Return the pointer to the first node with value val.
 * Return NULL if no match was found.
 */
struct bst *bst_seek(const bst *t, const int val);

/*
 * Delete the node pointed to by *node.
 *
 * 	BE CAREFUL USING ME
 * Assume *node is a valid node in the tree but do not check it.
 * Designed to be used after bst_seek().
 */
int bst_delete(struct bst *t, struct bst *node);

void bst_destroy(const struct bst *t);


	
#ifdef __cplusplus
}
#endif



#endif /* end of include guard */
