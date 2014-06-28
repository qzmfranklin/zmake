#include <stdio.h>
#include "mergesort.h"

static void print_array(const int n, const int *a)
{
	for (int i = 0; i < n; i++)
		printf("[%3d] %3d\n",i,a[i]);
	printf("\n");
}

int main(int argc, char const* argv[])
{
	const int n=15;
	int a[n]={11,7,3,9,14,0,12,5,15,10,2,8,1,4,6};

	printf("before sort:\n");
	print_array(n,a);
	mergesort(n,a); 
	printf("after sort:\n");
	print_array(n,a);

	return 0;
}
