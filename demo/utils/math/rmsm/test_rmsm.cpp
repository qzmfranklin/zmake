#include "utils/utils.h"
#include <string.h>
#include "rmsm.h"

static void print_vector(const char *banner, const int n, const double *a)
{

}

int main(int argc, char const* argv[])
{
	const int size=100;
	struct st_rmsm *m=rmsm_create(size);
	rmsm_print_info(m); 

	for (int i = 0; i < 5000; i++) {
		int row=rand()%size;
		int col=rand()%size;
		double val=1.0*rand()/RAND_MAX;
		//fprintf(stderr,"[%5d,%5d]*%f\n",row,col,val);
		rmsm_add(m,row,col,val);
	}

	rmsm_pack(m); 
	rmsm_print_info(m); 

	double x[size], y[size];
	const int row=3, col=3;
	memset(x,0,sizeof(double)*size);
	x[col]=1.0;
	rmsm_mul(m,x,y);
	printf("x is the unit vector at %d\n",col);
	print_vector("y=m.x=",10,y); 

	rmsm_destroy(m);

	return 0;
}
