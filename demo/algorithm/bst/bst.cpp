#include <stdio.h>
#include <assert.h>
#include "bst.h"
#include <vector>
#include <stdlib.h>

/*
 *struct bst {
 *        int val;
 *        int bf; // balance factor
 *        struct bst *left;
 *        struct bst *right;
 *};
 */

/* 0 = success */
// TODO: split into two functions
static int create_bst_from_string(const char *in, struct bst *out)
{
	if (in[0] != '{') {
		fprintf(stderr,"bst_create: "
				"create_bst_from_string: "
				"Invalid input string:"
				"\n%s",in);
		return -1;
	}

	/* Frist pass, scans the input string into a std::vector. */
	std::vector<bool> *mask = new std::vector<bool>;
	std::vector<int>  *vals = new std::vector<int>;

	int k=1;
	while (in[k] != '}') {
		while (in[k] == ' ' || in[k] == ',')
			k++;
		if ('0' <= in[k] && in[k] <= '9') {
			/* valid node */
			int val;
			sscanf(in+k,"%d",&val);
			mask.push_back(true);
			vals.push_back(val);
			while ( '0' <= in[k] && in[k] <= '9' )
				k++;
		} else if (in[k] == '#') {
			/* empty node */
			mask.push_back(false);
			vals.push_back(-INT_MAX);
		} else {
			fprintf(stderr,"bst_create: "
					"create_bst_from_string: "
					"Invalid input string:"
					"\n%s",in);
			return -1;
		}
	}

	/*
	 * Second pass, create the binary search tree from the std::vector.
	 * Morris preorder traversal. *ptr is an auxilliary array.
	 */
	const int n = mask.size();
	int *ptrs = (int*) malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++)
		ptrs[i] = -INT_MAX;
	assert(ptrs);
	int curr = 0;
	while (curr < n) {
#define left(x) (2*(x)+1)
#define right(x) (2*(x)+2)
#define parent(x) (((x)-1)/2)
#define is_valid(x) ( (x) < n || mask[(x)]==true )

		if ( !is_valid(left(curr)) ) {
			curr = right(curr);
			continue;
		}
		int prev = left(curr);
		while (  is_valid(right(prev)) && ptrs[right(prev)] != curr )
			prev = right(prev);
		if ( !is_valid(right(curr)) ) {
			//curr = (struct bst*) calloc(sizeof(struct bst));
			//assert(curr);
			ptrs[right(curr)] = curr;
			curr = left(curr);
		} else {
			curr = right(prev);
			ptrs[right(curr)] = -INT_MAX;
		}

#undef is_valid
#undef left
#undef right
#undef parent
	}

	return 0;
}

struct bst *bst_create(const char *str)
{
	struct bst *t = (struct bst*) malloc(sizeof(struct bst));
	if (str==NULL || t==NULL)
		return t;
}

void bst_to_str(const struct bst *t, int *len, char *out);

struct bst *bst_copy(const struct bst *t);

int bst_is_balanced(const struct bst *t);

void bst_insert(struct bst *t, const int val);

struct bst *bst_seek(const bst *t, const int val);

int bst_delete(struct bst *t, struct bst *node);

void bst_destroy(const struct bst *t);
