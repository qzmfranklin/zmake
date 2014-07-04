#include "geo_utils.h"

void print_array(const int n, const double _Complex *restrict a)
{
	for (int i = 0; i < n; i++)
		printf("[%5d] %7.3f %23.16E %23.16E \n",i,
				carg(a[i]),creal(a[i]),cimag(a[i]));
}

void read_binary(FILE *restrict fd, size_t *restrict n, double _Complex *restrict *a)
{
	fread(n,sizeof(size_t),1,fd);
	int N=*n;
	*a = (double _Complex*)malloc(sizeof(double _Complex)*N);
	assert(*a);
	fread(*a,sizeof(double _Complex),N,fd);
}

void read_ascii(FILE *restrict fd, size_t *restrict n, double _Complex *restrict *a)
{
	fscanf(fd,"%lu\n",n);
	int N=*n;
	double *tmp = (double*)malloc(sizeof(double _Complex)*N);
	assert(*a);
	for (int i = 0; i < N; i++)
		fscanf(fd,"%lf %lf\n",tmp+2*i,tmp+2*i+1);
	*a = (double _Complex*)tmp;
}

void output_binary(FILE *fd, const size_t n, const double _Complex *restrict a)
{
	fwrite(&n,sizeof(size_t),1,fd);
	fwrite(a,sizeof(double _Complex),n,fd);
}

void output_ascii(FILE *fd, const size_t n, const double _Complex *restrict a)
{
	fprintf(fd,"%lu\n",n);
	for (int i = 0; i < n; i++)
		fprintf(fd,"%23.16E %23.16E\n",creal(a[i]),cimag(a[i]));
}
