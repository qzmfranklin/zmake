#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <algorithm>

static double max_diff(const double *a, const int m,
		const double *b, const int n)
{
	return 0.0;
}

static void prepare_array(double **a, const int n)
{
	*a = (double *) malloc(sizeof(double)*n);
	assert(*a);
	for (int i = 0; i < n; i++)
		*(*a+i) = 10.0 * rand() / RAND_MAX;
	//::std::sort(*a,*a+n);
}

static void print_array(const double *a, const int n)
{
	for (int i = 0; i < n; i++)
		printf("%5.2f  ",a[i]);
	printf("\n");
}

static void print_diff_matrix(const double *a, const int m,
		const double *b, const int n)
{
	printf("\n");
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			printf("%5.2f  ",a[i] - b[j]);
		printf("\n");
	}
}

int main(int argc, char const* argv[])
{
	srand(10);
	double *a;
	const int m = 10;
	prepare_array(&a,m);

	double *b;
	const int n = 10;
	prepare_array(&b,n);

	print_array(a,m);
	print_array(b,n);

	print_diff_matrix(a,m,b,n);

	free(a);
	free(b);

	return 0;
}
