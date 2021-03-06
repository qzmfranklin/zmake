#include "geo_utils.h"

void print_array(const int n, const void *restrict a0)
{
	const double _Complex *a = (double _Complex*)a0;
	for (int i = 0; i < n; i++)
		printf("[%5d] %7.3f %23.16E %23.16E \n",i,
				carg(a[i]),creal(a[i]),cimag(a[i]));
}

void read_binary(FILE *restrict fd, size_t *restrict n, void *restrict *a0)
{
	fread(n,sizeof(size_t),1,fd);
	int N=*n;
	double _Complex *a = (double _Complex*)
		malloc(sizeof(double _Complex)*N);
	assert(a);
	fread(a,sizeof(double _Complex),N,fd);
	*a0 = a;
}

void read_ascii(FILE *restrict fd, size_t *restrict n, void *restrict *a)
{
	fscanf(fd,"%lu\n",n);
	int N=*n;
	double *tmp = (double*)malloc(sizeof(double _Complex)*N);
	assert(tmp);
	for (int i = 0; i < N; i++)
		fscanf(fd,"%lf %lf\n",tmp+2*i,tmp+2*i+1);
	*a = tmp;
}

void output_binary(FILE *fd, const size_t n, const void *restrict a)
{
	fwrite(&n,sizeof(size_t),1,fd);
	fwrite(a,sizeof(double _Complex),n,fd);
}

void output_ascii(FILE *fd, const size_t n, const void *restrict a0)
{
	const double _Complex *a = (double _Complex*)a0;
	fprintf(fd,"%lu\n",n);
	for (int i = 0; i < n; i++)
		fprintf(fd,"%23.16E %23.16E\n",creal(a[i]),cimag(a[i]));
}
