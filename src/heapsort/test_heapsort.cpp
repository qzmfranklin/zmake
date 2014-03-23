#include <stdio.h>
#include "heapsort.h"
/******************************************************************************/
int main(int argc, char const* argv[])
{ 
	const int n=15;
	int a[n]={11,7,3,9,14,0,12,5,15,10,2,8,1,4,6};

	for (int i = 0; i < n; i++)
		printf("%2d ",a[i]);
	printf("\n");

	heapsort(n,a); 

	for (int i = 0; i < n; i++)
		printf("%2d ",a[i]);
	printf("\n");

	return 0;
}
