#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <vector>

struct node {
	int key;
	node *left;
	node *right;
	node(int _key): key(_key), left(NULL), right(NULL) {};
};

/*
 * Max path sum of this tree = 20
 */
static node *prepare_tree()
{
#define __nn(x) new node((x))
	node *p  = __nn(4);
	p->left  = __nn(1);
	p->right = __nn(3);
	p->left->left  = __nn(8);
	p->left->right = __nn(7);
	p->right->left = __nn(4);
	p->right->right= __nn(1);
#undef __nn
	return p;
}

static void destroy_tree(node *p)
{
	if (p->right) {
		destroy_tree(p->right);
		p->right = NULL;
	}
	if (p->left) {
		destroy_tree(p->left);
		p->left = NULL;
	}
	free(p);
}

static int _max_path_sum(node *p, int &max)
{
	const int lmax = (p->left)  ? _max_path_sum(p->left, max) : 0;
	const int rmax = (p->right) ? _max_path_sum(p->right,max) : 0;
	max = ::std::max(max, p->key + lmax + rmax);
	return p->key + ::std::max(lmax, rmax);
}

static int max_path_sum(node *p)
{
	int max = -INT_MAX;
	_max_path_sum(p,max);
	return max;
}

int main(int argc, char const* argv[])
{
	node *tree = prepare_tree();
	printf("max path sum = %d\n",max_path_sum(tree));
	printf("expected value = 20\n");
	return 0;
}
