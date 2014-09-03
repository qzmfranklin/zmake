#include "utils/utils.h"
#include <string.h>
#include <stdlib.h>
#include "rmsm.h"

static void print_vector(const char *banner, const int n, const double *a)
{
	printf("%s\n",banner);
	for (int i = 0; i < n; i++)
		printf("[%5d] %.6E\n",i,a[i]);
}

int main(int argc, char const* argv[])
{
	if (argc<3) {
		fprintf(stderr,"Usage: test_rmsm.exe [size] [num_elements]\n");
		exit(1);
	}
	int size, num_elements;
	sscanf(argv[1],"%d",&size);
	sscanf(argv[2],"%d",&num_elements);

	struct st_rmsm *m=rmsm_create(size);
	//rmsm_print_info(m); 

	for (int i = 0; i < num_elements; i++) {
		int row = rand() % size;
		int col = rand() % size;
		double val = 10.0 * rand()/RAND_MAX/num_elements*size;
		rmsm_add(m,val,row,col);
	}

	rmsm_pack(m,RMSM_ABSERR,0.0); 
	printf("m=\n");
	rmsm_print_info(m); 

	double x[size], y[size];
	const int row=3, col=3;
	memset(x,0,sizeof(double)*size);
	x[col]=1.0;
	printf("x is the unit vector at %d\n",col);

	rmsm_mul(m,x,y);
	print_vector("y=m.x=",10,y); 

	rmsm_destroy(m);

	return 0;
}
