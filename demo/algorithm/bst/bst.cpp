#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include "bst.h"
#include <vector>
#include <stdlib.h>
#include <string.h>

/*
 *struct bst {
 *        int key;
 *        int bf; // balance factor
 *        struct bst *left;
 *        struct bst *right;
 *};
 */

/*
 * Any runtime error would exit(1).
 * TODO: error handler to avoid memory leak
 */
static struct bst *_bst_from_string(const char *in)
{
	if (in[0] != '{') {
		fprintf(stderr,"bst_create: "
		        "create_bst_from_string: "
		        "Invalid input string:"
		        "\n%s",in);
		exit(1);
	}

	/* Frist pass, scans the input string into a std::vector. */
	std::vector<struct bst*> *mask = new std::vector<struct bst*>;
	std::vector<int>         *keys = new std::vector<int>;

	int k=1;
	while (in[k] != '}') {
		while (in[k] == ' ' || in[k] == ',' || in[k] == '\n')
			k++;
		if ('0' <= in[k] && in[k] <= '9') {
			/* valid node */
			int key;
			sscanf(in+k,"%d",&key);
			/*
			 * 0xbeaf = valid, but not alloc'd
			 * NULL   = invalid, empty node
			 * other  = valid, alloc'd
			 * ISO C++ forbid comparison between pointers and
			 * integers, have to add -fpermissive.
			 */
			mask->push_back(0xbeaf);
			keys->push_back(key);
			while ( '0' <= in[k] && in[k] <= '9' )
				k++;
		} else if (in[k] == '#') {
			/* empty node */
			mask->push_back(NULL);
			keys->push_back(0);
		} else {
			fprintf(stderr,"bst_create: "
			        "create_bst_from_string: "
			        "Invalid input string:"
			        "\n%s",in);
			exit(1);
		}
	}

	/* Second pass, create the binary search tree from the std::vector  */
	const int n = mask->size();
	for (int i = 0; i < n; i++) {
#define left(x) (2*(x)+1)
#define right(x) (2*(x)+2)
#define parent(x) (((x)-1)/2)
#define is_valid(x) ( (x) < n && (x) >=0 && (*mask)[(x)] )
#define is_alloc(x) ( (*mask)[(x)] == 0xbeaf )
#define alloc_node(x) { \
	struct bst *p = (struct bst*) calloc(sizeof(struct bst),1); \
	assert(p); \
	(*mask)[(x)] = p; }
		if (  !is_valid(i)  )
			continue;
		if (  is_alloc(i)  )
			alloc_node(i);
		if (  is_valid(left(i))  &&  is_alloc(left(i))  ) {
			alloc_node(left(i));
			(*mask)[i]->left = (*mask)[left(i)];
		}
		if (  is_valid(right(i))  &&  is_alloc(right(i))  ) {
			alloc_node(right(i));
			(*mask)[i]->right = (*mask)[right(i)];
		}
#undef is_valid
#undef is_allc
#undef left
#undef right
#undef parent
#undef alloc_node
	}

	struct bst *res = (*mask)[0];

	delete mask;
	delete keys;

	return res;
}

static void _bst_update_balanc_factor(struct bst *t)
{
}

struct bst *bst_from_string(const char *str)
{
	if (str==NULL)
		return NULL;
	struct bst * res;
	res = _bst_from_string(str);
	_bst_update_balanc_factor(res);
	return res;
}

int bst_height(const struct bst *t)
{
}

typedef struct {
	int key;
	int right;
} linear_node_do_not_use_me;

void bst_to_string(struct bst *in, char *out)
{
	if (in == NULL) {
		strcpy(out,"{}");
		return;
	}

	/* First pass, convert to integral array */
	std::vector<char> mask;
	std::vector<linear_node_do_not_use_me> tree;

	/* Preorder Morris Traversal */
	struct bst *curr = in;
	int jcur = 0;
	while (curr) {
#define left(x) (2*(x)+1)
#define right(x) (2*(x)+2)
#define output(x) { \
	mask.reserve(jcur+1); \
	tree.reserve(jcur+1); \
	mask[jcur] = 1; \
	tree[jcur].key = curr->key; }
		if (curr->left == NULL) {
			output(curr);
			curr = curr->right;
			jcur = right(jcur);
			continue;
		}
		struct bst *prev = curr->left;
		int jpre = left(jcur);
		while (prev->right  &&  prev->right != curr) {
			prev = prev->right;
			jpre = right(jpre);
		}
		if (prev->right == NULL) {
			output(curr);
			prev->right = curr;
			tree[jpre].right = jcur;
			curr = curr->left;
			jcur = left(jcur);
		} else {
			curr = prev->right;
			jcur = tree[jpre].right;
			prev->right = NULL;
		}
#undef left
#undef right
#undef output
	}

	/* Second pass, convert integral array to string */
	int k = 0; /* current position in out */
	out[k++] = '{';
	const int n = mask.size();
	char buf[100];
	for (int i = 0; i < n; i++) {
		if (!mask[i]) continue;
		sprintf(buf,"%d",tree[i].key);
		strcpy(out+k,buf);
		k += strlen(buf) + 1;
	}
	out[k++] = '}';

	return;
}

static void _bst_copy_recursive(struct bst **to, const struct bst *from)
{
	*to = (struct bst*) calloc(sizeof(struct bst),1);
	assert(*to);
	(*to)->key = from->key;

	if (from->left)
		_bst_copy_recursive( &((*to)->left), from->left );
	if (from->right)
		_bst_copy_recursive( &((*to)->right), from->right );
}

struct bst *bst_copy(const struct bst *t)
{
	if (t == NULL)
		return NULL;
	struct bst *res;
	_bst_copy_recursive(&res,t);
	return res;
}

/* TODO: Possibly incorrect yet, but close. Need to test it. */
static int _bst_depth_diff(const struct bst *t)
{
#define FAIL -INT_MAX
	if (t==NULL)
		return 0;

	const int l = _bst_depth_diff(t->left);
	const int r = _bst_depth_diff(t->right);
	if (l==FAIL || r==FAIL)
		return FAIL;

	const int d = abs(l-r);
	if (d<2)
		return d+1;
	else
		return FAIL;
#undef FAIL
}

int bst_is_balanced(const struct bst *t)
{
	return _bst_depth_diff(t);
}

void bst_insert(struct bst *t, const int val);

struct bst *bst_seek(const bst *t, const int val);

int bst_delete(struct bst *t, struct bst *node);

static void _bst_destroy(struct bst *t)
{
	if (t->left)
		_bst_destroy(t->left);
	if (t->right)
		_bst_destroy(t->right);
	free(t);
}

void bst_destroy(struct bst *t)
{
	if (t==NULL)
		return;
	_bst_destroy(t);
}
