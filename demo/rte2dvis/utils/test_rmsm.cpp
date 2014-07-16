#include "utils.h"
#include <string.h>
#include "rmsm.h"
/******************************************************************************/
int test01(void);
/******************************************************************************/
int main(int argc, char const* argv[])
{
	if (argc>1) link_stdout(argv[1]);
	test01(); 
	if (argc>1) unlink_stdout(); 

	return 0;
}

int test01(void)
{
	int err=0; 
        printf("TEST01\n");
        printf("	|Test rmsm (passed)\n");

	const int size=100;
	struct st_rmsm *m=rmsm_create(size);
	rmsm_print_matrixinfo(m); 

	for (int i = 0; i < 5000; i++) {
		int row=rand()%size;
		int col=rand()%size;
		double val=1.0*rand()/RAND_MAX;
		//fprintf(stderr,"[%5d,%5d]*%f\n",row,col,val);
		rmsm_add(m,row,col,val);
	}

	rmsm_pack(m); 
	rmsm_print_matrixinfo(m); 

	double x[size], y[size];
	const int row=3, col=3;
	memset(x,0,sizeof(double)*size);
	x[col]=1.0;
	rmsm_dmul(m,x,y);
	printf("x is the unit vector at %d\n",col);
	print_vector("y=m.x=",10,y); 

	printf("row %d\n",row);
	//for (int i = 0; i < m->rsz[row]; i++)
	for (int i = 0; i < 10; i++)
		printf("[%5d] %+.16E\n",m->col[i+m->pos[row]],m->data[i+m->pos[row]]);

	rmsm_destroy(m);

        printf("END OF TEST01\n");
        printf("\n");
	return err;
}
