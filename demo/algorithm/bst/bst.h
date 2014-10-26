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

void bst_destroy(struct bst *t);

#ifdef __cplusplus
}
#endif



#endif /* end of include guard */
