#include "bst.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const* argv[])
{
	const int keys[] = {0,1,2,3,4,5,6,7,8,9,10};
	const int N = sizeof(keys)/sizeof(int);
	struct bst *t = bst_create(keys[0]);

	bst_insert(&t,keys[1]);
	bst_insert(&t,keys[2]);
	bst_insert(&t,keys[3]);

	return 0;
}
