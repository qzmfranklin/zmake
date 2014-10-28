#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stack>
#include "bst.h"

static void _bst_print_node(const struct bst *p)
{
	printf("%5d\n",p->key);
}

static void _bst_print_node_debug(const struct bst *p)
{
	fprintf(stderr,"\t%5d, bf = %2d, self = %p, left = %p, right = %p\n",
			p->key,p->bf,p,p->left,p->right);
}

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

	x->bf+=2;
	y->bf++;

	/*
	 *bst_print_node(y);
	 *bst_print_node(x);
	 *bst_print_node(y->right);
	 */
	fprintf(stderr,"_bst_RR(%p) ends here\n",x);

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
	fprintf(stderr,"_bst_update_bf_and_rotate(p,q,&s) "
			"p = %p, q = %p, s->size() = %lu\n",p,q,s->size());
	struct bst *tmp = p;
	while (1) {
		fprintf(stderr,"\t\ts->size() = %lu\n",s->size());

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

		if (s->empty())
			break;

		q = p;
		p = s->top();
		s->pop();

		if (p->left == q)
			p->left = tmp;
		else
			p->right = tmp;
	}

	while (!s->empty()) {
		tmp = s->top();
		s->pop();
	}

	fprintf(stderr,"_bst_update_bf_and_rotate(p,q,&s) ends here "
			"with tmp = %p\n",tmp);


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

	printf("\n");

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

static void _bst_traverse_recursive_preorder(const struct bst *t)
{
	if (t == NULL)
		return;
	_bst_print_node(t);
	_bst_traverse_recursive_preorder(t->left);
	_bst_traverse_recursive_preorder(t->right);
}

static void _bst_traverse_recursive_inorder(const struct bst *t)
{
	if (t == NULL)
		return;
	_bst_traverse_recursive_inorder(t->left);
	_bst_print_node(t);
	_bst_traverse_recursive_inorder(t->right);
}

static void _bst_traverse_recursive_postorder(const struct bst *t)
{
	if (t == NULL)
		return;
	_bst_traverse_recursive_postorder(t->left);
	_bst_traverse_recursive_postorder(t->right);
	_bst_print_node(t);
}

static void _bst_traverse_recursive(const struct bst *t, const int mode)
{
	fprintf(stderr,"_bst_traverse_recursive not implemented\n");
	return;

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

static void _bst_traverse_stack_preorder(const struct bst *t)
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

static void _bst_traverse_stack_inorder(const struct bst *t)
{
	::std::stack<struct bst*> s;
	s.push(t);
	while (!s.empty()) {
		struct bst *p = s.top();
		s.pop();
		if (!p->left && !p->right) {
			_bst_print_node(p);
			while (s.size() >= 1) {
				_bst_print_node(s.top());
				s.pop();
			}
		}
		if (p->right)
			s.push(p->right);
		s.push(p);
		if (p->left)
			s.push(p->left);
	}
}

static void _bst_traverse_stack_postorder(const struct bst *t)
{
	::std::stack<struct bst*> s;
	s.push(t);
	while (!s.empty()) {
		struct bst *p = s.top();
		s.pop();
		if (!p->left && !p->right) {
			_bst_print_node(p);
			while (s.size() >= 1) {
				_bst_print_node(s.top());
				s.pop();
			}
		}
		s.push(p);
		if (p->right)
			s.push(p->right);
		if (p->left)
			s.push(p->left);
	}
}

static void _bst_traverse_stack(const struct bst *t, const int mode)
{
	fprintf(stderr,"_bst_traverse_ not implemented\n");
	return;

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

static void _bst_traverse_morris(const struct bst *t, const int mode)
{
	switch (mode) {
	case BST_PREORDER:
		_bst_traverse_morris_preorder(t);
		break;
	case BST_INORDER:
		_bst_traverse_morris_inorder(t);
		break;
	case BST_POSTORDER:
		fprintf(stderr,"_bst_traverse_morris_postorder not implemented\n");
		break;
	default:
		fprintf(stderr,"_bst_traverse_morris: unknown mode %d\n",mode);
		break;
	}
}

static void _bst_traverse_debug_preorder(const struct bst *t)
{
	::std::debug<struct bst*> s;
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

static void _bst_traverse_debug_inorder(const struct bst *t)
{
	::std::debug<struct bst*> s;
	s.push(t);
	while (!s.empty()) {
		struct bst *p = s.top();
		s.pop();
		if (!p->left && !p->right) {
			_bst_print_node_debug(p);
			while (s.size() >= 1) {
				_bst_print_node_debug(s.top());
				s.pop();
			}
		}
		if (p->right)
			s.push(p->right);
		s.push(p);
		if (p->left)
			s.push(p->left);
	}
}

static void _bst_traverse_debug_postorder(const struct bst *t)
{
	::std::debug<struct bst*> s;
	s.push(t);
	while (!s.empty()) {
		struct bst *p = s.top();
		s.pop();
		if (!p->left && !p->right) {
			_bst_print_node_debug(p);
			while (s.size() >= 1) {
				_bst_print_node_debug(s.top());
				s.pop();
			}
		}
		s.push(p);
		if (p->right)
			s.push(p->right);
		if (p->left)
			s.push(p->left);
	}
}

static void _bst_traverse_debug(const struct bst *t, const int mode)
{
	switch (mode) {
	case BST_PREORDER:
		_bst_traverse_debug_preorder(t);
		break;
	case BST_INORDER:
		_bst_traverse_debug_inorder(t);
		break;
	case BST_POSTORDER:
		_bst_traverse_debug_postorder(t);
		break;
	default:
		fprintf(stderr,"_bst_traverse_debug: unknown mode %d\n",mode);
		break;
	}
}

void bst_traverse(struct bst *t, const int mode)
{
	fprintf(stderr,"bst_traverse(%p)\n",t);
	if (mode & BST_RECURSIVE)
		_bst_traverse_recursive(t,mode & BST_MASK);
	else if (mode & BST_STACK)
		_bst_traverse_stack(t,mod & BST_MASK);
	else if (mode & BST_MORRIS)
		_bst_traverse_morris(t,mode & BST_MASK);
	else if (mode & BST_DEBUG)
		_bst_traverse_debug(t,mode & BST_MASK);
	else {
		printf("bst_traverse(%p,%X): unknown mode, default to BST_STACK\n",t,mode);
		_bst_traverse_stack(t,mode & BST_MASK);
	}
}

void bst_destroy(struct bst *t);
