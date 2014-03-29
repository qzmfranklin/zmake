#include <stdio.h>
#include "heapsort.h"
#include "utils/utils.h"
/******************************************************************************/
int main(int argc, char const* argv[])
{ 
	const int n=15;
	int a[n]={11,7,3,9,14,90,12,5,15,10,2,8,1,4,6};

	print_i(n,a);
	printf("\n");

	heapsort(n,a); 

	print_i(n,a);
	printf("\n");

	return 0;
}
