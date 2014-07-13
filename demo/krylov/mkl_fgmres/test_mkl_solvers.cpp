#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <mkl.h>
#include <string.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

static void m_mul_v(const int n, double *restrict A, 
		double *restrict in, double *restrict out)
{
	for (int i = 0; i < n; i++)
		out[i] = 0.0;
	for (int j = 0; j < n; j++)
		for (int i = 0; i < n; i++)
			out[i] += A[i+j*n] * in[j];
}

static void print_mkl_dfgmres_pars(const int *restrict ipar, 
		const double *restrict dpar)
{
	printf("mkl fgmres solver parameters list (C indexing)\n");
	/*
	 * LOGICAL parameters:
	 * ipar[3]:  current iteration count
	 * ipar[4]:  max numbuer of iterations
	 * ipar[7]:  !0 = max iteration check [1]
	 * ipar[8]:  !0 = residual stopping check dpar[3]<dpar[4] [0]
	 * ipar[9]:  !0 = user-defined stopping check by setting RCI_request=2 [1]
	 * ipar[10]: !0 = requests precondition by setting RCI_request=3 [0]
	 * ipar[11]: !0 = check zero norm of current vector dpar[6]<=dpar[7] [0]
	 *                 0 = requests user-defined check by setting RCI_request=4
	 *                 (must set to 1 for some unknown reasons)
	 * ipar[12]: xution vector storage flag [0]
	 * ipar[13]: internal iteration counts before restart
	 * ipar[14]: max number of non-restarted version
	 */
	printf("ipar[  4] = %-8d  ",ipar[4]);
	printf("maximal number of iterations\n");
	printf("ipar[  7] = %-8d  ",ipar[7]);
	printf("!0 = automatic stopping check on max iteration [1]\n");
	printf("ipar[  8] = %-8d  ",ipar[8]);
	printf("!0 = automatic stopping check on dpar[3]<dpar[4] [0]\n");
	printf("ipar[  9] = %-8d  ",ipar[9]);
	printf("!0 = request user-defined stopping check by setting\n");
	printf("\t\t\t   RCI_request=2 [1]\n");
	printf("ipar[ 10] = %-8d  ",ipar[10]);
	printf("!0 = request precondition by setting RCI_request=3 [0]\n");
	printf("ipar[ 11] = %-8d  ",ipar[11]);
	printf("!0 = automatic check on zero norm of current vector \n");
	printf("\t\t\t   dpar[6]<=dpar[7]; if 0, request for user-defined\n");
	printf("\t\t\t   zero-norm check by setting RCI_request=4 [0]\n");
	printf("ipar[ 12] = %-8d  ",ipar[12]);
	printf("vector storage flag [0]\n");
	//printf("ipar[ 13] = %-8d  ",ipar[13]);
	//printf("internal iteratoin count before restart\n");
	printf("ipar[ 14] = %-8d  ",ipar[14]);
	printf("maximal number of non-restarted iterations\n");
	/*
	 * DOUBLE parameters:
	 * dpar[0]:  relative tolerance [1.0D-6]
	 * dpar[1]:  abxute tolerance [0.0D-0]
	 * dpar[2]:  L2 norm of initial residual
	 * dpar[3]:  service variable, dpar[0]*dpar[2]+dpar[1]
	 * dpar[4]:  L2 norm of current residual
	 * dpar[5]:  L2 norm of previous residual, if available
	 * dpar[6]:  norm of generated vector
	 * dpar[7]:  tolerance for zero norm of current vector [1.0D-12]
	 */
	printf("dpar[  0] = %.2E  ",dpar[0]);
	printf("relative tolerance [1.0D-6]\n");
	printf("dpar[  1] = %.2E  ",dpar[1]);
	printf("absolute tolerance [0.0D-0]\n");
	printf("dpar[  2] = %.2E  ",dpar[2]);
	printf("L2 norm of initial residual\n");
	printf("dpar[  3] = %.2E  ",dpar[3]);
	printf("service variable, dpar[0]*dpar[2]+dpar[1]\n");
	printf("dpar[  4] = %.2E  ",dpar[4]);
	printf("L2 norm of current residual\n");
	printf("dpar[  5] = %.2E  ",dpar[5]);
	printf("L2 norm of previous residual, if applicable\n");
	printf("dpar[  6] = %.2E  ",dpar[6]);
	printf("norm of generated vector\n");
	printf("dpar[  7] = %.2E  ",dpar[7]);
	printf("tolerance for zero norm of current vector [1.0D-12]\n");
}

static void init_vector(const int n, double *restrict a)
{
	for (int i = 0; i < n; i++)
		a[i] = 10.0*rand()/RAND_MAX;
}

static void print_vector(const char *msg, const int n, double *restrict a)
{
	printf("%s\n",msg);
	for (int i = 0; i < n; i++)
		printf("[%5d] %23.16f\n",i,a[i]);
	printf("\n");
}

int main(int argc, char const* argv[])
{
	const int N=100;
	const int max_num_non_restart=MAX(150,N);


	// Prepare the inputs.
	double A[N*N],expected_sol[N],rhs[N];
	init_vector(N*N,A);
	for (int i = 0; i < N; i++)
		expected_sol[i] = i;
	m_mul_v(N,A,expected_sol,rhs);

	fprintf(stderr,"hello\n");

	printf("N=%d\n",N);
	print_vector("expected_sol=",10,expected_sol);
	
	// Allocate storage for the iterative solver.
	double sol[N];
	init_vector(N,sol); 
	print_vector("initial guess=",10,sol); 

	int ipar[128],RCI_request,ivar=N,itercount;
	double dpar[128];
	// non-preconditioned gmres solver
	double tmp[(2*max_num_non_restart+1)*N + max_num_non_restart*(max_num_non_restart+9)/2 + 1];

	dfgmres_init(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp);
	assert(!RCI_request);

	/*
	 * LOGICAL parameters:
	 *   ipar[3]:  current iteration count
	 *   ipar[4]:  max numbuer of iterations
	 *   ipar[7]:  !0 = max iteration check [1]
	 *   ipar[8]:  !0 = residual stopping check dpar[3]<dpar[4] [0]
	 *   ipar[9]:  !0 = user-defined stopping check by setting RCI_request=2 [1]
	 *   ipar[10]: !0 = requests precondition by setting RCI_request=3 [0]
	 *   ipar[11]: !0 = check zero norm of current vector dpar[6]<=dpar[7] [0]
	 *              0 = requests user-defined check by setting RCI_request=4
	 *              (must set to 1 for some unknown reasons)
	 *   ipar[12]: solution vector storage flag [0]
	 *   ipar[13]: internal iteration counts before restart
	 *   ipar[14]: max number of non-restarted version
	 */
	ipar[4]  = 100;
	ipar[7]  = 1;
	ipar[8]	 = 1;
	ipar[9]	 = 0;
	ipar[11] = 1;
	ipar[14] = max_num_non_restart;

	/*
	 * DOUBLE parameters:
	 *   dpar[0]:  relative tolerance [1.0D-6]
	 *   dpar[1]:  absolute tolerance [0.0D-0]
	 *   dpar[2]:  L2 norm of initial residual
	 *   dpar[3]:  service variable, dpar[0]*dpar[2]+dpar[1]
	 *   dpar[4]:  L2 norm of current residual
	 *   dpar[5]:  L2 norm of previous residual, if available
	 *   dpar[6]:  norm of generated vector
	 *   dpar[7]:  tolerance for zero norm of current vector [1.0D-12]
	 */
	dpar[0]	 = 1.0E-10;
	// Check consistency of parameters
	dfgmres_check(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp);
	assert(!RCI_request);

	print_mkl_dfgmres_pars(ipar,dpar);

	printf("start iterating!\n");
	/*
	 * RCI_request:
	 * 	0	completed
	 * 	1	requests user-defined matrix-vector multiplication
	 * 	2	requests user-defined stopping check, ipar[9]
	 * 	3	requests user-defined preconditoning, ipar[10]
	 * 	4	requests user-defined zero norm check, ipar[11]
	 */
	do {
		dfgmres(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp); 
		switch (RCI_request) {
		case 1: // *(tmp+ipar[22]-1) <= mul(A,*(tmp+ipar[21]-1))
			m_mul_v(N,A,tmp+ipar[21]-1,tmp+ipar[22]-1);
			break;
		case 2: // requests user-defined stopping check
			break;
		case 3: // requests preconditioning
			break;
		case 4: // requests zero norm check
			break;
		default:// no idea how to get here...
			break;
		}
		//if (ipar[3]%5==0)
			//printf("[  %5d,  %5d,  %.2E]\n",
					//ipar[3],ipar[13],dpar[4]);
	} while(RCI_request>0);
	if (RCI_request<0)
		fprintf(stderr,"\n\nWANRING: solver broke down with RCI_request = %d\n\n\n",RCI_request);

	// Extract solution, print, clear buffers.
	dfgmres_get(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp,&itercount);
	printf("\n");
	printf("solver finished after %d iterations!\n",itercount);
	printf("relative residual = %.3E\n",dpar[4]);
	print_vector("approximate solution=",10,sol);
	printf("...\n");
	MKL_Free_Buffers(); 
}
