#include <stdio.h>
#include <assert.h>
#include "mshio.h"
#include "rmsm.h"
#include "solver_v2.h"
#include "utils.h"
/*****************************************************************************/
int main(int argc, char const* argv[]);
int test01(void);
/*****************************************************************************/
int main(int argc, char const* argv[])
{ 
	if (argc>1) link_stdout(argv[1]); 
	if (argc>1) unlink_stdout(); 
	test01(); 

	return 0;
}

int test01(void)
{
	int err=0; 
        printf("TEST01\n");
        printf("	|Test solver_v2 workflow\n");

	//struct st_mesh *q=mshio_create_mesh("msh/0344/0344");
	struct st_mesh *q=mshio_create_mesh("msh/0616/0616");
	//mshio_print_mesh(q,PRINT_INFO_VERBOSE);

	/*
	 * ipar[0] = M
	 * ipar[1] = Nd
	 * ipar[2] = pad
	 * ipar[3] = rule1
	 * ipar[4] = rule2
	 * ipar[5] = nu
	 * ipar[6] = nv
	 * ipar[7] = num_threads in omp
	 */
	const int ipar[128]={1,3,1, 1,1,5,3, 1};
	/*
	 * dpar[0] = g factor
	 * dpar[1] = mua (absorption coefficient)
	 * dpar[2] = mus (scattering coefficient)
	 */
	const double dpar[128]={0.7,1.0,2.0};

	struct st_solver_v2 *s=sv2_create_solver(q,ipar,dpar);
	sv2_print_solverinfo(s);
	//for (int i = 0; i < s->Nt; i++)
		//rmsm_print_row(s->E,i);

	double _Complex *b0=(double _Complex*)mkl_malloc(sizeof(double _Complex)*s->Ng,64);
	double _Complex *x0=(double _Complex*)mkl_malloc(sizeof(double _Complex)*s->Ng,64);
	double _Complex *b1=(double _Complex*)mkl_malloc(sizeof(double _Complex)*s->Ng,64);
	double _Complex *x1=(double _Complex*)mkl_malloc(sizeof(double _Complex)*s->Ng,64);
	assert(b0);
	assert(x0);
	assert(b1);
	assert(x1);
	int nitr;
	double eps;

	//sv2_gen_b0(s,0.0,b0);
	for (int i = 0; i < s->Ng; i++)
		x0[i] = 1.0;
	print_vector("x=",5,x0);
	sv2_mul(s,x0,b0);
	print_vector("b=A.x=",5,b0);
	sv2_solve(s,b0,x0,200,12,1.0E-13,&nitr,&eps); 

	//sv2_gen_b1x0(s,0.0,b1,x0);
	//sv2_solve(s,b1,x1,200,12,1.0E-13,&nitr,&eps); 

	sv2_destroy_solver(s); 
	mshio_destroy_mesh(q);

        printf("END OF TEST01\n");
        printf("\n");
	return err;
}
