#include "convex_hull.h"
#include <complex.h>

static void print_array(const int n, const double _Complex *restrict a)
{
	for (int i = 0; i < n; i++)
		printf("[%5d] %7.3f %23.16E %23.16E \n",i,
				creal(a[i]),cimag(a[i]),carg(a[i]));
}

static int status=0;

static void read_binary(FILE *restrict fd, size_t *restrict n, 
		double _Complex *restrict *a)
{
	fread(n,sizeof(size_t),1,fd);
	int N=*n;
	*a = (double _Complex*)malloc(sizeof(double _Complex)*N);
	assert(*a);
	fread(*a,sizeof(double _Complex),N,fd);
}

static void read_ascii(FILE *restrict fd, size_t *restrict n, 
		double _Complex *restrict *a)
{
	fscanf(fd,"%lu\n",n);
	int N=*n;
	double *tmp = (double*)malloc(sizeof(double _Complex)*N);
	assert(*a);
	for (int i = 0; i < N; i++)
		fscanf(fd,"%lf %lf\n",tmp+2*i,tmp+2*i+1);
	*a = (double _Complex*)tmp;
}

static void output_binary(FILE *fd, const size_t n, const double _Complex *restrict a)
{
	//assert(status==5);
	fwrite(&n,sizeof(size_t),1,fd);
	fwrite(a,sizeof(double _Complex),n,fd);
	//status=6;
}

static void output_ascii(FILE *fd, const size_t n, const double _Complex *restrict a)
{
	//assert(status==5);
	fprintf(fd,"%lu\n",n);
	for (int i = 0; i < n; i++)
		fprintf(fd,"%23.16E %23.16E\n",creal(a[i]),cimag(a[i]));
	//status=6;
}

int main(int argc, char const* argv[])
{
	size_t N;
	double _Complex *data;

	//read_binary(stdin,&N,&data);
	read_ascii(stdin,&N,&data);
	//print_array(N,data);

	graham_scan(&N,&data);
	
	//output_binary(stdout,N,data);
	output_ascii(stdout,N,data);

	free(data);

	return 0;
}
