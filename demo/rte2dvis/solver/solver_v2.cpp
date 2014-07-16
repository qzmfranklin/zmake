#include <stdio.h>
#include <omp.h>
#include <mkl.h>
#include <complex.h>
#include <math.h>
#include <assert.h>
#include "solver_v2.h"
#include "mesh.h" 
#include "quadrules.h"
#include "rmsm.h"
/**************************************/
#include "utils.h"
/******************************************************************************/
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
/******************************************************************************/
static void init(struct st_solver_v2 *s, const int *ipar, const double *dpar)
{
	//fprintf(stderr,"init()\n");
	assert(s->status==0);

	for (int i = 0; i < 128; i++) {
		s->ipar[i] = ipar[i];
		s->dpar[i] = dpar[i];
	}

	s->M      = 1;
	s->Nd     = s->ipar[1];
	s->pad    = s->ipar[2];
	s->num_threads = s->ipar[7];

	struct st_mesh *q=s->mesh;

	s->Ns     = q->num_trigs;
	s->Nv     = q->num_verts;
	s->Ne     = q->num_edges;
	s->Nt     = s->Nv + (s->M-1)*s->Ne + (s->M-1)*(s->M-2)/2*s->Ns;

	{
	int tmp=2*s->Nd+1;
	int res=tmp%s->pad;
	if (res==0) res=s->pad;
	s->Nm     = tmp + s->pad - res;
	}

	s->Ng     = s->Nt * (2*s->Nd+1);

	s->g_factor = dpar[0]; 

	s->status = 1;
}
static void alloc(struct st_solver_v2 *s)
{
	//fprintf(stderr,"alloc()\n");
	assert(s->status==1);

	s->K=(double*)mkl_malloc(sizeof(double)*(s->Nt*s->Nt*s->Nm*2),64);
	//assert(s->E);
	assert(s->K);

	// g table [2*Nd+1]
	s->g=(double*)mkl_malloc(sizeof(double)*(2*s->Nd+1),64);
	assert(s->g);
	s->g[s->Nd] = 1.0;
	for (int i = s->Nd+1; i < 2*s->Nd+1; i++) {
		s->g[i] = s->g[i-1] * s->g_factor;
		s->g[2*s->Nd-i] = s->g[i];
	}

	// for matrix-vector multiplication
	s->work[0]=(double _Complex*)mkl_malloc(sizeof(double _Complex)*(s->Nt*s->Nm*2),64);
	s->work[1]=(double _Complex*)mkl_malloc(sizeof(double _Complex)*(s->Nt*s->Nm*2),64);
	assert(s->work[0]);
	assert(s->work[1]);
	const int Nm_times_2 = 2*s->Nm;
	fftw_init_threads();
	fftw_plan_with_nthreads(s->num_threads);
	s->plans[0] = fftw_plan_many_dft(1,&Nm_times_2,s->Nt,
			(fftw_complex*)(s->work[0]),NULL,1,Nm_times_2,
			(fftw_complex*)(s->work[0]),NULL,1,Nm_times_2,
			FFTW_FORWARD ,FFTW_MEASURE|FFTW_PATIENT);
	s->plans[1] = fftw_plan_many_dft(1,&Nm_times_2,s->Nt,
			(fftw_complex*)(s->work[1]),NULL,1,Nm_times_2,
			(fftw_complex*)(s->work[1]),NULL,1,Nm_times_2,
			FFTW_BACKWARD,FFTW_MEASURE|FFTW_PATIENT);

	s->status=2;
}
static void fillE(struct st_solver_v2 *s)
{
	//fprintf(stderr,"fillE()\n");
	assert(s->status==2);

	s->E = rmsm_create(s->Nt);

	const double A[]={	// row-major, times 2 already
		1.0/12*2,	1.0/24*2,	1.0/24*2,
		1.0/24*2,	1.0/12*2,	1.0/24*2,
		1.0/24*2,	1.0/24*2,	1.0/12*2};
	for (int n = 0; n < s->Ns; n++)
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++) {
				const int row = s->mesh->t[i+3*n];
				const int col = s->mesh->t[j+3*n];
				const double val = A[j+3*i] * s->mesh->a[n];
				//printf("[%5d,%5d] %+.5E\n",row,col,val);
				rmsm_add(s->E,row,col,val); 
			}

	rmsm_pack(s->E);

	s->status=3;
}
/**************************************/
static double r2rd(const double *p1, const double *p2)
{
	const double a=p1[0]-p2[0];
	const double b=p1[1]-p2[1];
	return sqrt(a*a+b*b);
}
// t[6]=triangle(x1,y1,x2,y2,x3,y3)	p[2]=point inside t 	l[3]=simplex coordinate
static void xy2l(const double *restrict t, const double *restrict p, double *restrict l)
{
	const double m = 1.0/((t[2]-t[0])*(t[5]-t[1])-(t[4]-t[0])*(t[3]-t[1])); // 1/(2*area), positive
	const double x1x = t[0]-p[0]; // x1-x
	const double x2x = t[2]-p[0];
	const double x3x = t[4]-p[0];
	const double y1y = t[1]-p[1];
	const double y2y = t[3]-p[1];
	const double y3y = t[5]-p[1];
	l[0] = m*(x2x*y3y-x3x*y2y);
	l[1] = m*(x3x*y1y-x1x*y3y);
	l[2] = m*(x1x*y2y-x2x*y1y);
}
static void fillK(struct st_solver_v2 *s)
{
	//fprintf(stderr,"fillK()\n");
	assert(s->status==3);
	const int Nt = s->Nt;
	const int Ng = s->Ng;
	const int Nd = s->Nd;
	const int Nm = s->Nm;
	const double *p = s->mesh->p;
	const int rule1 = s->ipar[3];
	const int rule2 = s->ipar[4];
	const int nu = s->ipar[5]; // source
	const int nv = s->ipar[6]; // source
	const int nn1 = wandzura_order_num(rule1);
	const int nn2 = dunavant_order_num(rule2);
	const int  ns = 3*nu*nv;
	const double *area = s->mesh->a;
	const double *cntr = s->mesh->c; 
	double *xy01=(double*)mkl_malloc(sizeof(double)*2*nn1,64);
	double  *wn1=(double*)mkl_malloc(sizeof(double)*nn1,64);
	double *xy02=(double*)mkl_malloc(sizeof(double)*2*nn2,64);
	double  *wn2=(double*)mkl_malloc(sizeof(double)*nn2,64);
	double *xu =(double*)mkl_malloc(sizeof(double)*nu,64);
	double *wu =(double*)mkl_malloc(sizeof(double)*nu,64);
	double *xv =(double*)mkl_malloc(sizeof(double)*nv,64);
	double *wv =(double*)mkl_malloc(sizeof(double)*nv,64);
	double _Complex *Kwork=(double _Complex*)mkl_malloc(sizeof(double _Complex)*Nt*Nt*2*Nm,64);

	// init non-sigular quad rules
	wandzura_rule(rule1,nn1,xy01,wn1);
	dunavant_rule(rule2,nn2,xy02,wn2);
	// init 1D quad rules
	//                     xmin,  xmax
	cgqf(nu,1.0,0.0,0.0,    0.0,  1.0,   xu,wu);
	cgqf(nv,1.0,0.0,0.0,    0.0,  1.0,   xv,wv);

	const int Nm_times_2 = 2*Nm;
	fftw_plan plan = fftw_plan_many_dft(1,&Nm_times_2,Nt*Nt,
			(fftw_complex*)Kwork,NULL,1,Nm_times_2,
			(fftw_complex*)Kwork,NULL,1,Nm_times_2,
			FFTW_FORWARD, FFTW_ESTIMATE);

	// Note: s->K [Nt,Nt,2Nm] is real row-major rank-3 tensor
	memset(Kwork,0,sizeof(double)*Nt*Nt*2*Nm);

	#pragma omp parallel		\
	num_threads(s->num_threads)	\
	default(none)			\
	shared(nu,nv,Nd,Nt,Ng,Nm,rule1, \
			rule2,nn1,nn2,	\
			p,ns,area,cntr, \
			xy01,wn1,xy02,	\
			wn2,xu,wu,xv,wv,\
			Kwork,		\
			stdout,stdin,stderr)
	{
	// testing
	double *xyn1=(double*)mkl_malloc(sizeof(double)*2*nn1,64);

	// source
	double *xyn2=(double*)mkl_malloc(sizeof(double)*2*nn2,64);

	double *xys=(double*)mkl_malloc(sizeof(double)*2*ns,64);
	double *ws =(double*)mkl_malloc(sizeof(double)*ns,64); 

	double _Complex *b  =(double _Complex*)mkl_malloc(sizeof(double)*2*2*Nm,64);

	double _Complex *e  =(double _Complex*)mkl_malloc(sizeof(double)*2*MAX(nn1,MAX(nn2,ns)),64);
	double _Complex *wer=(double _Complex*)mkl_malloc(sizeof(double)*2*MAX(nn1,MAX(nn2,ns)),64); 

	double *work=(double*)mkl_malloc(sizeof(double)*3*nu*nv,64);

	const int num_threads=omp_get_num_threads();
	const int tid=omp_get_thread_num();
	const int res=Nt%num_threads;
	const int blk=(res==0)?(Nt/num_threads):(Nt/num_threads+1);
	const int start=tid*blk;
	const int end=(tid!=num_threads-1)?((tid+1)*blk):(Nt);
	//fprintf(stderr,"[tid %-3d] starts...\n",tid);
	for (int n = start; n < end; n++) {
		reference_to_physical_t3(nn1,p+6*n,xy01,xyn1);
		for (int np = 0; np < Nt; np++) {
			reference_to_physical_t3(nn2,p+6*np,xy02,xyn2);
			// Compute one block of s->K with (n,np)
			for (int l = 0; l < 3; l++) {
				for (int lp = 0; lp < 3; lp++) {
					const int row = s->mesh->t[l +3*n ];
					const int col = s->mesh->t[lp+3*np];
					for (int j = 0; j < nn1; j++) {
						const double *p0=xyn1+2*j;
						int MM;
						memset(b,0,sizeof(double _Complex)*2*Nm);
						if (r2rd(p0,cntr+2*np)>6*0.8774*sqrt(area[np])) {
							// Non-singular
							MM = nn2;
							for (int i = 0; i < MM; i++) {
								double sc[3]; // simplex coordinates
								xy2l(p+6*np,xyn2+2*i,sc);
								const double dx = p0[0] - xyn2[2*i  ];
								const double dy = p0[1] - xyn2[2*i+1];
								const double inv= 1.0/sqrt(dx*dx+dy*dy);
								e[i]   = inv*(dx-dy*I);
								wer[i] = inv*wn2[i]*area[np]
									*sc[lp] + 0.0*I;
							}
						} else {
							// Singular, near-singular
							MM = 3*nu*nv;
							arcsinh_rule_xy(xys,ws, p0,p+6*np,
									nu,xu,wu, nv,xv,wv,
									work);
							for (int i = 0; i < MM; i++) {
								double sc[3]; // simplex coordinates
								xy2l(p+6*np,xyn2+2*i,sc);
								const double dx = p0[0] - xys[2*i  ];
								const double dy = p0[1] - xys[2*i+1];
								const double inv= 1.0/sqrt(dx*dx+dy*dy);
								e[i]   = inv*(dx-dy*I);
								wer[i] = ws[i]
									*sc[lp] + 0.0*I;
							}
						}
						// Fill b[0]
						for (int i = 0; i < MM; i++)
							b[0] += wer[i];
						// Fill b[1] -> b[2Nd]
						for (int dm = 1; dm <= 2*Nd; dm++) {
							for (int i = 0; i < MM; i++)
								wer[i] *= e[i];
							for (int i = 0; i < MM; i++)
								b[dm]  += wer[i];
						}
						// Pad b[2Nd+1] -> b[2(Nm-Nd)-1] with zeros
						// Fill b[2(Nm-Nd)] -> b[2Nm-1] with complex conjugates
						for (int i = 2*(Nm-Nd); i <= 2*Nm-1; i++)
							b[i] = conj(b[2*Nm-i]);
						// copy b to Kwork, wn1=testing weights
						for (int i = 0; i < 2*Nm; i++)
							//Kwork[i+(np+n*Nt)*2*Nm] += b[i] * wn1[j];
							Kwork[i+(col+row*Nt)*2*Nm] += b[i] * wn1[j];
					}
				}
			}
		}
	}
	// Free thread-specific memories
	mkl_free(xyn1);
	mkl_free(xyn2);
	mkl_free(xys);
	mkl_free(ws );
	mkl_free(b  );
	mkl_free(e  );
	mkl_free(wer);

	mkl_free(work);
	} // end of #pragma omp parallel

	// FFTW s->work to s->K
	fftw_execute(plan); // in-place s->K
	for (int n = 0; n < Nt; n++) {
		const double xx=area[n]*0.5/Nm;
		for (int np = 0; np < Nt; np++)
			for (int i = 0; i < 2*Nm; i++)
				s->K[i+(np+n*Nt)*2*Nm] += xx * creal(Kwork[i+(np+n*Nt)*2*Nm]);
	}

	// Release Memory
	mkl_free(xy01);
	mkl_free(wn1 );
	mkl_free(xy02);
	mkl_free(wn2 );

	mkl_free(xu );
	mkl_free(wu );
	mkl_free(xv );
	mkl_free(wv );

	mkl_free(Kwork);

	// Destroy FFTW plan
	fftw_destroy_plan(plan);

	s->status=4;
}
/******************************************************************************/
struct st_solver_v2 *sv2_create_solver(struct st_mesh *q, const int *ipar, const double *dpar)
{
	//fprintf(stderr,"sv2_create_solver()\n");

	assert(q);
	assert(q->status==3); // st_mesh is already loaded

	struct st_solver_v2 *s = (struct st_solver_v2*)
		mkl_malloc(sizeof(struct st_solver_v2),64);
	assert(s);
	s->status = 0; 
	s->mesh   = q;

	init(s,ipar,dpar);
	alloc(s);
	fillE(s);
	fillK(s);

	return s;
}

void sv2_destroy_solver(struct st_solver_v2 *s)
{
	//fprintf(stderr,"sv2_destroy_solver()\n");
	assert(s->status>=2); // check alloc'd

	mkl_free(s->K);
	mkl_free(s->g);
	mkl_free(s->work[0]);
	mkl_free(s->work[1]); 

	fftw_destroy_plan(s->plans[0]);
	fftw_destroy_plan(s->plans[1]);

	rmsm_destroy(s->E);
}

void sv2_mul(struct st_solver_v2 *s, const double _Complex *restrict in, double _Complex *restrict out)
{
	//fprintf(stderr,"sv2_mul()\n");
	assert(s->status==4);
	/*
	 * dpar[0] = g factor
	 * dpar[1] = mua (absorption coefficient)
	 * dpar[2] = mus (scattering coefficient)
	 * dpar[3] = phis (planewave incident)
	 */
	// convenience variables
	const double mua = s->dpar[1];
	const double mus = s->dpar[2];
	const double mut = mua + mus;
	const int     Nt = s->Nt;
	const int     Nm = s->Nm;
	const int     Nd = s->Nd;
	const int     Ng = s->Ng;

	double _Complex **work = s->work;
	memset(out,0,sizeof(double _Complex)*(s->Ng));

	//contribution from identity term I
	//#pragma omp parallel for 	\
	//num_threads(s->num_threads)	\
	//default(none)			\
	//shared(in,out,s,Nt)
	for (int n = 0; n < Nt; n++)
		for (int np = 0; np < s->E->rsz[n]; np++)
			for (int i = 0; i < 2*Nd+1; i++)
				out[i+n*(2*Nd+1)] += s->E->data[np+s->E->pos[n]] 
					* in[i+s->E->col[np+s->E->pos[n]]*(2*Nd+1)];
	/*
	 *#pragma omp parallel for 	\
	 *num_threads(s->num_threads)	\
	 *default(none)			\
	 *shared(work,Nm,Nt,Nd,mut,mus,in,s)
	 *for (int n = 0; n < Nt; n++) {
	 *        for (int i = 0; i < 2*Nd+1; i++)
	 *                work[0][i+n*2*Nm] = mut*in[i+n*(2*Nd+1)] - mus*s->g[i]*in[i+n*(2*Nd+1)];
	 *        memset(work[0]+n*2*Nm+2*Nd+1,0,sizeof(double _Complex)*(2*Nm-2*Nd-1));
	 *}
	 *fftw_execute(s->plans[0]); // in-place omp FWD FFTW work[0]
	 *memset(work[1],0,sizeof(double _Complex)*(Nt*2*Nm));
	 *#pragma omp parallel for 	\
	 *num_threads(s->num_threads)	\
	 *default(none)			\
	 *shared(work,s,Nm,Nt)
	 *for (int n = 0; n < Nt; n++)
	 *        for (int np = 0; np < Nt; np++)
	 *                for (int i = 0; i < 2*Nm; i++)
	 *                        work[1][i+n*2*Nm] += s->K[i+(np+n*Nt)*2*Nm] * work[0][i+np*2*Nm];
	 *fftw_execute(s->plans[1]); // in-place omp BWD FFTW work[1]
	 *memset(out,0,sizeof(double _Complex)*Ng);
	 *#pragma omp parallel for 	\
	 *num_threads(s->num_threads)	\
	 *default(none)			\
	 *shared(out,work,Nm,Nt,Nd)
	 *for (int n = 0; n < Nt; n++)
	 *        for (int i = 0; i < 2*Nd+1; i++)
	 *                out[i+n*(2*Nd+1)] = work[1][i+n*2*Nm];
	 */
}

static size_t tmp_size(const size_t size, const size_t n)
{
	size_t temp = (2*n+1)*size+n*(n+9)/2+1;
	printf("allocate %lu bytes for mkl_fgmres solver\n",sizeof(double)*temp);
	return temp;
}
static void print_mkl_dfgmres_par_info(const int *ipar, const double *dpar)
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
void sv2_solve(struct st_solver_v2 *s, double _Complex *rhs, double _Complex *sol, 
		const int max_nitr, const int max_nonrestart_nitr, const double retol, 
		int *restrict nitr, double *restrict eps)
{
	fprintf(stderr,"sv2_solve()\n");
	double *b=(double*)rhs;
	double *x=(double*)sol;
	
	int size=2*s->Ng;
	double *tmp=(double*)mkl_malloc(sizeof(double)*tmp_size(size,max_nonrestart_nitr),64);
	int ipar[128],RCI_request;
	double dpar[128];
	assert(tmp); 

	//fprintf(stderr,"solve!\n");
	dfgmres_init(&size,x,b,&RCI_request,ipar,dpar,tmp);
	assert(!RCI_request);

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
	ipar[4]  = max_nitr;
	ipar[7]  = 1;
	ipar[8]	 = 1;
	ipar[9]	 = 0;
	ipar[11] = 1;
	ipar[14] = max_nonrestart_nitr;
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
	dpar[0]	 = retol;

	// Check consistency of parameters
	dfgmres_check(&size,x,b,&RCI_request,ipar,dpar,tmp);
	assert(!RCI_request);

	print_mkl_dfgmres_par_info(ipar,dpar);

	//printf("helleoworl = %d\n",293);
	print_vector("right hand side b=",10,(double _Complex*)b);
	printf("...\n");

	for (int i = 0; i < 2*s->Ng; i++)
		x[i] = 10.0 * rand()/RAND_MAX;
	print_vector("initial guess x=",10,(double _Complex*)x);
	printf("...\n");

	printf("start iterating!\n");
	do {
		dfgmres(&size,x,b,&RCI_request,ipar,dpar,tmp); 
		switch (RCI_request) {
		case 1: // *(tmp+ipar[22]-1) <= mul(A,*(tmp+ipar[21]-1))
			sv2_mul(s,(double _Complex*)(tmp+ipar[21]-1),
					(double _Complex*)(tmp+ipar[22]-1));
			break;
		case 2: // ipar[9]  requests user-defined stopping check
			break;
		case 3: // ipar[10] requests preconditioning
			break;
		case 4: // ipar[11] requests zero norm check
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
	dfgmres_get(&size,x,b,&RCI_request,ipar,dpar,tmp,nitr);
	*eps = dpar[4];
	printf("solver finished after %d iterations!\n",*nitr);
	printf("relative residual = %.3E\n",*eps);

	MKL_Free_Buffers(); 
	mkl_free(tmp);

	print_vector("approximate solution x=",10,(double _Complex*)x);
	printf("...\n");
}
void sv2_print_solverinfo(struct st_solver_v2 *s)
{
	printf("    status = %d  ", s->status);
	printf("(0=uninit'd 4=ready)\n");

	printf("TRIGS   Ns = %d\n",s->Ns);
	printf("VERTS   Nv = %d\n",s->Nv);
	printf("EDGES   Ne = %d\n",s->Ne);
	printf("SPAT    Nt = %d\n",s->Nt);
	printf("ANG     Nm = %d\n",s->Nm);
	printf("TOTAL   Ng = %d\n",s->Ng);

	printf("SPA INT  M = %d\n",s->M);
	printf("HARM    Nd = %d\n",s->Nd);
	printf("ANG    pad = %d\n",s->pad);
	printf("SRC  rule1 = %d\n",s->ipar[3]);
	printf("TEST rule2 = %d\n",s->ipar[4]);
	printf("TEST    nu = %d\n",s->ipar[5]);
	printf("TEST    nv = %d\n",s->ipar[6]);

	printf("num_threads= %d\n",s->num_threads);

	printf("  g_factor = %.3f\n",s->g_factor);
	printf("  mua      = %.3f\n",s->dpar[1]);
	printf("  mus      = %.3f\n",s->dpar[2]);

	//printf("RAW   mesh = %p\n",s->mesh);
	//printf("IDENTITY E = %p\n",s->E);
	//printf("INTERACT K = %p\n",s->K);
}

