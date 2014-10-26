#include <stdlib.h>
#include <assert.h>
#include <stack>
#include "bst.h"

struct bst *bst_create(const int key)
{
	struct bst *root = (struct bst*) malloc(sizeof(struct bst));
	if (root) {
		root->key    = key;
		root->bf     = 0;
		root->left   = NULL;
		root->right  = NULL;
	}
	return root;
}

static struct bst *_bst_alloc_node(const int key)
{
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
	struct bst *y = x->left;
	x->left  = y->right;
	y->right = x;

	x->bf--;
	y->bf--;

	return y;
}

/* p->bf = -2, returns the root of the subtree */
static struct bst *_bst_rotate_right(struct bst *p)
{
	if (p->left->bf == +1)
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
	struct bst *y = x->right;
	x->right = y->left;
	y->left  = x;

	x->bf++;
	y->bf++;

	return y;
}

/* p->bf = +2, returns the root of the subtree */
static struct bst *_bst_rotate_left(struct bst *p)
{
	if (p->right->bf == -1)
		_bst_RL(p);
	return _bst_RR(p);
}

/* returns the root of the whole tree */
static struct bst *_bst_update_bf_and_rotate(struct bst *p, struct bst *q,
		::std::stack<struct bst*> *s)
{
	struct bst *tmp = p;

	while ( !s->empty() ) {
		q = p;
		p = s->top();
		s->pop();

		if (p->left == q) {
			p->bf -= 1;
			p->left = tmp;
		} else {
			p->bf += 1;
			p->right = tmp;
		}

		if (p->bf == 0)
			break;
		else if (p->bf < -1)
			tmp = _bst_rotate_right(p);
		else if (p->bf > +1)
			tmp = _bst_rotate_left(p);
	}
	if (p->left == q)
		p->left = tmp;
	else
		p->right = tmp;

	return tmp;
}

struct bst *bst_insert(struct bst **t, const int key)
{
	if (t == NULL)
		return NULL;

	/* Avoid recursion with a stack */
	struct bst *p = *t;
	::std::stack<struct bst*> s;
	while (p)
		if (key == p->key) {
			break;
		} else if (key > p->key) {
			s.push(p);
			p = p->right;
		} else {
			s.push(p);
			p = p->left;
		}

	if (p)
		return NULL;

	struct bst *q = _bst_alloc_node(key);
	if (q == NULL)
		return NULL;

	/* Update balance factor and rotate as proper */
	p = s.top();
	s.pop();
	if (key < p->key) {
		p->left = q;
		if (p->right) {
			p->bf = 0;
			return q;
		}
		p->bf = -1;
		*t = _bst_update_bf_and_rotate(p,q,&s);
	} else {
		p->right = q;
		if (p->left) {
			p->bf = 0;
			return q;
		}
		p->bf = +1;
		*t = _bst_update_bf_and_rotate(p,q,&s);
	}

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

void bst_delete(struct bst *t, const struct bst *node);

void bst_destroy(struct bst *t);
