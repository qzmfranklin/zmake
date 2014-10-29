#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stack>
#include "bst.h"

static int _bst_is_leaf(const struct bst *p)
{
	return !p->left && !p->right;
}

static void _bst_print_node(const struct bst *p)
{
	printf("%d\n",p->key);
}

static void _bst_print_node_debug(const struct bst *p)
{
	if (p == NULL)
		return;
	fprintf(stderr,"\t%5d, bf = %2d, self = %p, left = %p, right = %p\n",
			p->key,p->bf,p,p->left,p->right);
}

struct bst *bst_create(const int key)
{
	//fprintf(stderr,"bst_create(%d)\n",key);
	struct bst *root = (struct bst*) malloc(sizeof(struct bst));
	if (root) {
		root->key    = key;
		root->bf     = 0;
		root->left   = NULL;
		root->right  = NULL;
	}
	//_bst_print_node_debug(root);
	return root;
}

static struct bst *_bst_alloc_node(const int key)
{
	//fprintf(stderr,"_bst_alloc_node(%d)\n",key);
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
	//fprintf(stderr,"_bst_LR(%p)\n",x);
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
	//fprintf(stderr,"_bst_LL(%p)\n",x);
	struct bst *y = x->left;
	x->left  = y->right;
	y->right = x;

	x->bf-=2;
	y->bf--;

	return y;
}

/* p->bf = +2, returns the root of the subtree */
static struct bst *_bst_rotate_right(struct bst *p)
{
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
	//fprintf(stderr,"_bst_RL(%p)\n",x);
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
	//fprintf(stderr,"_bst_RR(%p)\n",x);
	struct bst *y = x->right;
	x->right = y->left;
	y->left  = x;

	x->bf+=2;
	y->bf++;

	/*
	 *_bst_print_node_debug(y);
	 *_bst_print_node_debug(x);
	 *_bst_print_node_debug(y->right);
	 */

	return y;
}

/* p->bf = -2, returns the root of the subtree */
static struct bst *_bst_rotate_left(struct bst *p)
{
	if (p->right->bf == +1)
		_bst_RL(p);
	return _bst_RR(p);
}

/* returns the root of the whole tree */
static struct bst *_bst_update_bf_and_rotate(struct bst *p, struct bst *q,
		::std::stack<struct bst*> *s)
{
	//fprintf(stderr,"_bst_update_bf_and_rotate(p,q,&s), new node q =\n");
	//_bst_print_node_debug(q);

	/*
	 * _bst_rotate_left/right returns the root of the balanced (tmp)
	 * subtree. The old root of the unbalanced subtree (p)'s parent
	 * needs to be updated to adopt the new child.
	 *
	 * If p was already the root of the whole tree (s.empty() == true),
	 * let p = tmp and return p. Otherwise update p's parent to adopt
	 * tmp.
	 *
	 * If tmp is already balanced, with or without rebalancing, exit
	 * loop. In this case, clear the stack (s) to retrieve the root
	 * of the whole tree.
	 */
	while (1) {
		//fprintf(stderr,"\t\ts->size() = %lu\n",s->size());

		if (p->left == q)
			p->bf++;
		else
			p->bf--;

		//_bst_print_node_debug(p);

		struct bst *tmp; // new root of the subtree
		if (p->bf < -1)
			tmp = _bst_rotate_left(p);
		else if (p->bf > +1)
			tmp = _bst_rotate_right(p);
		else
			tmp = p;

		if (s->empty()) {
			p = tmp;
			break;
		}

		q = p;
		p = s->top();
		s->pop();

		if (p->left == q)
			p->left = tmp;
		else
			p->right = tmp;

		if (tmp->bf == 0)
			break;
	}

	while (!s->empty()) {
		p = s->top();
		s->pop();
	}

	//fprintf(stderr,"_bst_update_bf_and_rotate(p,q,&s) ends here "
			//"with p = \n");
	//_bst_print_node_debug(p);

	return p;
}

struct bst *bst_insert(struct bst **t, const int key)
{
	//fprintf(stderr,"\nbst_insert(%d)\n",key);
	if (*t == NULL)
		return NULL;

	/* Avoid recursion with a stack */
	struct bst *p = *t;
	::std::stack<struct bst*> s;
	while (p) {
		/*
		 * // debug use only
		 *static int i=0;
		 *i++;
		 *if (i>20) {
		 *        fprintf(stderr,"too many iterations, abort\n");
		 *        abort();
		 *}
		 */
		if (key == p->key) {
			break;
		} else if (key > p->key) {
			s.push(p);
			p = p->right;
		} else {
			s.push(p);
			p = p->left;
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

// TODO
void bst_delete(struct bst *t, const struct bst *node)
{
	fprintf(stderr,"bst_delete(%p,%p) not implemented yet\n",t,node);
}

void bst_print_node(const struct bst *p)
{
	_bst_print_node(p);
}

static void _bst_traverse_recursive_preorder(struct bst *t)
{
	if (t == NULL)
		return;
	_bst_print_node(t);
	_bst_traverse_recursive_preorder(t->left);
	_bst_traverse_recursive_preorder(t->right);
}

static void _bst_traverse_recursive_inorder(struct bst *t)
{
	if (t == NULL)
		return;
	_bst_traverse_recursive_inorder(t->left);
	_bst_print_node(t);
	_bst_traverse_recursive_inorder(t->right);
}

static void _bst_traverse_recursive_postorder(struct bst *t)
{
	if (t == NULL)
		return;
	_bst_traverse_recursive_postorder(t->left);
	_bst_traverse_recursive_postorder(t->right);
	_bst_print_node(t);
}

static void _bst_traverse_recursive(struct bst *t, const int mode)
{
	switch (mode) {
	case BST_PREORDER:
		_bst_traverse_recursive_preorder(t);
		break;
	case BST_INORDER:
		_bst_traverse_recursive_inorder(t);
		break;
	case BST_POSTORDER:
		_bst_traverse_recursive_postorder(t);
		break;
	default:
		fprintf(stderr,"_bst_traverse_recursive: unknown mode %d\n",mode);
		break;
	}
}

static void _bst_traverse_stack_preorder(struct bst *t)
{
	::std::stack<struct bst*> s;
	s.push(t);
	while (!s.empty()) {
		struct bst *p = s.top();
		s.pop();
		_bst_print_node(p);
		if (p->right)
			s.push(p->right);
		if (p->left)
			s.push(p->left);
	}
}

static void _bst_traverse_stack_inorder(struct bst *t)
{
	::std::stack<struct bst*> s;
	struct bst *p = t;
	while (1)
		if (p) {
			s.push(p);
			p = p->left;
		} else {
			if (s.empty())
				break;
			p = s.top();
			s.pop();
			_bst_print_node(p);
			p = p->right;
		}
}

static struct bst *_bst_peek_stack(::std::stack<struct bst*> *s)
{
	return (s->empty())  ?  NULL  :  s->top();
}

/*
 * See http://www.geeksforgeeks.org/iterative-postorder-traversal-using-stack/
 * for details.
 */
static void _bst_traverse_stack_postorder(struct bst *t)
{
	::std::stack<struct bst*> s;
	struct bst *p = t;
	while (1) {
		while (p) {
			if (p->right)
				s.push(p->right);
			s.push(p);
			p = p->left;
		}
		if (s.empty())
			break;
		p = s.top();
		s.pop();
		if (p->right  &&  p->right == _bst_peek_stack(&s)) {
			s.pop();
			s.push(p);
			p = p->right;
		} else {
			_bst_print_node(p);
			p = NULL;
		}
	}
}

static void _bst_traverse_stack(struct bst *t, const int mode)
{
	switch (mode) {
	case BST_PREORDER:
		_bst_traverse_stack_preorder(t);
		break;
	case BST_INORDER:
		_bst_traverse_stack_inorder(t);
		break;
	case BST_POSTORDER:
		_bst_traverse_stack_postorder(t);
		break;
	default:
		fprintf(stderr,"_bst_traverse_stack: unknown mode %d\n",mode);
		break;
	}
}

static void _bst_traverse_morris_inorder(struct bst *t)
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
			prev->right = curr;
			curr = curr->left;
		} else {
			_bst_print_node(curr);
			prev->right = NULL;
			curr = curr->right;
		}
	}
}

static void _bst_traverse_morris_preorder(struct bst *t)
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

//TODO
static void _bst_traverse_morris_postorder(struct bst *t)
{
	fprintf(stderr,"_bst_traverse_morris_postorder not implemented yet\n");
	return;
}

static void _bst_traverse_morris(struct bst *t, const int mode)
{
	switch (mode) {
	case BST_PREORDER:
		_bst_traverse_morris_preorder(t);
		break;
	case BST_INORDER:
		_bst_traverse_morris_inorder(t);
		break;
	case BST_POSTORDER:
		_bst_traverse_morris_postorder(t);
		break;
	default:
		fprintf(stderr,"_bst_traverse_morris: unknown mode %d\n",mode);
		break;
	}
}

static void _bst_traverse_debug(struct bst *t, const int mode)
{
	switch (mode) {
	case BST_PREORDER:
		//_bst_traverse_debug_preorder(t);
		break;
	case BST_INORDER:
		//_bst_traverse_debug_inorder(t);
		break;
	case BST_POSTORDER:
		//_bst_traverse_debug_postorder(t);
		break;
	default:
		fprintf(stderr,"_bst_traverse_debug: unknown mode %d\n",mode);
		break;
	}
}

void bst_traverse(struct bst *t, const int mode)
{
	//fprintf(stderr,"bst_traverse(%p,0x%X)\n",t,mode);
	if (mode & BST_RECURSIVE)
		_bst_traverse_recursive(t,mode & BST_MASK);
	else if (mode & BST_STACK)
		_bst_traverse_stack(t,mode & BST_MASK);
	else if (mode & BST_MORRIS)
		_bst_traverse_morris(t,mode & BST_MASK);
	else if (mode & BST_DEBUG)
		_bst_traverse_debug(t,mode & BST_MASK);
	else {
		printf("Unknown mode, default to BST_STACK\n");
		_bst_traverse_stack(t,mode & BST_MASK);
	}
	printf("\n");
}

void bst_destroy(struct bst *t);
