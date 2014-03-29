#include <stdio.h>
#include "utils/utils.h"
/******************************************************************************/
int test01(void)
{
	int err=0; 
        printf("TEST01\n");
        printf("	|Test print_d\n");

	const int n=5;
	double a[n];

	for (int i = 0; i < n; i++)
		a[i] = i;

	print_d(n,a);
	
        printf("END OF TEST01\n");
        printf("\n");
	return err;
}
int main(int argc, char *argv[])
{ 
	test01();
	return 0;
}
