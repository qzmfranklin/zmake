#include <stdio.h>
#include <stdlib.h>

static int GLOBAL;

int main(int argc, char const* argv[])
{
	static int STATIC;
	int STACK;
	int *HEAP = (int*) malloc(sizeof(int));

	printf("GLOBAL  = %p\n",&GLOBAL);
	printf("STATIC  = %p\n",&STATIC);
	printf("STACK   = %p\n",&STACK);
	printf("HEAP    = %p\n",HEAP);

	free(HEAP);

	return 0;
}
