#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stack>
#include "bst.h"

struct bst *bst_create(const int key)
{
	fprintf(stderr,"bst_create(%d)\n",key);
	struct bst *root = (struct bst*) malloc(sizeof(struct bst));
	if (root) {
		root->key    = key;
		root->bf     = 0;
		root->left   = NULL;
		root->right  = NULL;
	}
	bst_print_node(root);
	return root;
}

static struct bst *_bst_alloc_node(const int key)
{
	fprintf(stderr,"_bst_alloc_node(%d)\n",key);
	struct bst *q = (struct bst*) malloc(sizeof(struct bst));
	if (q == NULL)
		return NULL;
	q->key    = key;
	q->bf     = 0;
	q->left   = NULL;
	q->right  = NULL;

	return q;
}

/*
 *       X            X
 *      /            /
 *     Z      =>    Y
 *      \          /
 *       Y        Z
 */
static void _bst_LR(struct bst *x)
{
	fprintf(stderr,"_bst_LR(%p)\n",x);
	struct bst *z = x->left;
	struct bst *y = x->left->right;
	x->left  = y;
	z->right = y->left;
	y->left  = z;

	y->bf++;
	z->bf++;
}

/*
 *       X            Y
 *      /            / \
 *     Y      =>    Z   X
 *    /
 *   Z
 */
static struct bst *_bst_LL(struct bst *x)
{
	fprintf(stderr,"_bst_LL(%p)\n",x);
	struct bst *y = x->left;
	x->left  = y->right;
	y->right = x;

	x->bf--;
	y->bf--;

	return y;
}

/* p->bf = +2, returns the root of the subtree */
static struct bst *_bst_rotate_right(struct bst *p)
{
	fprintf(stderr,"_bst_rotate_right(%p)\n",p);
	if (p->left->bf == -1)
		_bst_LR(p);
	return _bst_LL(p);
}

/*
 *    X           X
 *     \           \
 *      Z    =>     Y
 *     /             \
 *    Y               Z
 */
static void _bst_RL(struct bst *x)
{
	fprintf(stderr,"_bst_RL(%p)\n",x);
	struct bst *z = x->right;
	struct bst *y = x->right->left;
	x->right = y;
	z->left  = y->right;
	y->right = z;

	z->bf--;
	y->bf--;
}

/*
 *    X             Y
 *     \           / \
 *      Y    =>   X   Z
 *       \
 *        Z
 */
static struct bst *_bst_RR(struct bst *x)
{
	fprintf(stderr,"_bst_RR(%p)\n",x);
	struct bst *y = x->right;
	x->right = y->left;
	y->left  = x;

	x->bf++;
	y->bf++;

	return y;
}

/* p->bf = -2, returns the root of the subtree */
static struct bst *_bst_rotate_left(struct bst *p)
{
	fprintf(stderr,"_bst_rotate_left(%p)\n",p);
	if (p->right->bf == +1)
		_bst_RL(p);
	return _bst_RR(p);
}

/* returns the root of the whole tree */
static struct bst *_bst_update_bf_and_rotate(struct bst *p, struct bst *q,
		::std::stack<struct bst*> *s)
{
	fprintf(stderr,"_bst_update_bf_and_rotate(p,q,&s) "
			"p = %p, q = %p, s->size() = %lu\n",p,q,s->size());
	struct bst *tmp = p;
	while (1) {
		fprintf(stderr,"\t\ts->size() = %lu\n",s->size());
		bst_print_node(p);

		if (p->left == q)
			p->bf++;
		else
			p->bf--;

		bst_print_node(p);

		if (p->bf == 0)
			break;
		else if (p->bf < -1)
			tmp = _bst_rotate_left(p);
		else if (p->bf > +1)
			tmp = _bst_rotate_right(p);
		fprintf(stderr,"\ttmp = %p\n",tmp);
		bst_print_node(p);

		if (s->empty())
			break;

		q = p;
		p = s->top();
		s->pop();

		if (p->left == q)
			p->left = tmp;
		else
			p->right = tmp;
		bst_print_node(p);
	}

	return tmp;
}

struct bst *bst_insert(struct bst **t, const int key)
{
	fprintf(stderr,"\nbst_insert(%d)\n",key);
	if (*t == NULL)
		return NULL;

	/* Avoid recursion with a stack */
	struct bst *p = *t;
	::std::stack<struct bst*> s;
	while (p) {
		static int i=0;
		//fprintf(stderr,"key = %d, p->key = %d, p = %p\n",key,p->key,p);
		if (key == p->key) {
			break;
		} else if (key > p->key) {
			s.push(p);
			p = p->right;
		} else {
			s.push(p);
			p = p->left;
		}
		i++;
		if (i>10) {
			fprintf(stderr,"too many iterations, abort\n");
			abort();
		}
	}

	if (p)
		return NULL;

	struct bst *q = _bst_alloc_node(key);
	if (q == NULL)
		return NULL;

	/* attach, update balance factor, rotate as proper */
	p = s.top();
	s.pop();
	if (key < p->key)
		p->left = q;
	else
		p->right = q;
	*t = _bst_update_bf_and_rotate(p,q,&s);

	return q;
}

struct bst *bst_search(struct bst *t, const int key)
{
	struct bst *p = t;
	while (p)
		if (key == p->key)
			break;
		else if (key > p->key)
			p = p->right;
		else
			p = p->left;
	return p;
}

void bst_delete(struct bst *t, const struct bst *node)
{
}

static void _bst_print_node(const struct bst *p)
{
	printf("\t%5d, bf = %2d, self = %p, left = %p, right = %p\n",
			p->key,p->bf,p,p->left,p->right);
}

void bst_print_node(const struct bst *p)
{
	_bst_print_node(p);
}

static void _bst_traverse_inorder(struct bst *t)
{
	struct bst *curr = t;
	while (curr) {
		if (curr->left == NULL) {
			_bst_print_node(curr);
			curr = curr->right;
			continue;
		}
		struct bst *prev = curr->left;
		while (prev->right != NULL  &&  prev->right != curr)
			prev = prev->right;
		if (prev->right == NULL) {
			prev->right = curr->right;
			curr = curr->left;
		} else {
			_bst_print_node(curr);
			prev->right = NULL;
			curr = curr->right;
		}
	}
}

static void _bst_traverse_preorder(struct bst *t)
{
	struct bst *curr = t;
	while (curr) {
		if (curr->left == NULL) {
			_bst_print_node(curr);
			curr = curr->right;
			continue;
		}
		struct bst *prev = curr->left;
		while (prev->right != NULL  &&  prev->right != curr)
			prev = prev->right;
		if (prev->right == NULL) {
			_bst_print_node(curr);
			prev->right = curr->right;
			curr = curr->left;
		} else {
			prev->right = NULL;
			curr = curr->right;
		}
	}
}

/*
 *enum {
 *        BST_PREORDER  = 0x1;
 *        BST_INORDER   = 0x2;
 *        BST_POSTORDER = 0x3;
 *
 *        BST_RECURSIVE = 0x1<<4;
 *        BST_STACK     = 0x2<<4;
 *        BST_MORRIS    = 0x3<<4;
 *};
 */
void bst_traverse(struct bst *t, const int mode)
{
	printf("bst_traverse(%p)\n",t);
	switch (mode) {
	case BST_PREORDER:
		_bst_traverse_preorder(t);
		break;
	case BST_INORDER:
		_bst_traverse_inorder(t);
		break;
	case BST_POSTORDER:
		fprintf(stderr,"bst_traverse: post-order not implemented yet...\n");
		break;
	default:
		fprintf(stderr,"bst_traverse: unknown mode %d\n",mode);
		break;
	}
}

void bst_destroy(struct bst *t);
