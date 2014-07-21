#include <stdio.h>
#include "heapsort.h"
#include "utils/utils.h"

static void print_i(const int n, const int *a)
{
	for (int i = 0; i < n; i++)
		printf("[%5d] %4d\n",i,a[i]);
	printf("\n");
}

int main(int argc, char const* argv[])
{ 
	const int n=15;
	int a[n]={11,7,3,9,14,90,12,5,15,10,2,8,1,4,6};

	printf("before sort:\n");
	print_i(n,a);

	heapsort3(n,a); 

	printf("after sort:\n");
	print_i(n,a);

	return 0;
}
