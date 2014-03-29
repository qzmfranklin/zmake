#include "utils.h"
#include <stdio.h>
/******************************************************************************/
void print_d(const int n, const double *a)
{
	for (int i = 0; i < n; i++)
		printf("[%5d] %.16E\n",i,a[i]);
}
void print_i(const int n, const int *a)
{
	for (int i = 0; i < n; i++)
		printf("[%5d] %.d\n",i,a[i]);
}
