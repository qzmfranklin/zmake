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
	const int size=13;
	struct st_rmsm *m=rmsm_create(size);
	//rmsm_print_info(m); 

	for (int i = 0; i < 500; i++) {
		int row=rand()%size;
		int col=rand()%size;
		double val=5.0*rand()/RAND_MAX;
		rmsm_add(m,val,row,col);
	}

	rmsm_pack(m); 
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
