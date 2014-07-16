#ifndef _SOLVER_V1_H_
#define _SOLVER_V1_H_
/*
 * Version 1
 * 	solve (I+K).X=V for X
 * 	pulse spatial basis function
 * 	I = purely diagonal, only store diagonal elements
 * 	K = blockwise Toeplitz, store the DFT of the first col+row
 */
/******************************************************************************/
#include <fftw3.h>
#include <unistd.h>
#include <stdio.h>
#include <complex.h>
#include "mesh.h"
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif 
struct st_solver_v1 {
	int status; // internal status

	int M; // spatial interpolation order
	int pad; // padding for Nd Nm

	int Ns; // #triangles
	int Nv; // #vertices
	int Ne; // #edeges
	int Nt; // #total spatial d.o.f.

	int Nd; // #highest angular harmonic
	int Nm;	// #padded angular number >=2*Nd+1

	int Ng; // #total d.o.f.

	int num_threads; // number of threads in any omp session

	struct st_mesh *mesh; // raw mesh

	double *E; // I, avoid _Complex_I
	double *K; // K, DFT, row-major, [Ns,Ns,2*Nm]
	
	double g_factor; // g factor in HG phase function
	double *g; // g^|m|

	double _Complex *work[8]; // complex workspaces
	double *tmp[8]; // real workspace

	fftw_plan plans[8]; // fftw_plan

	int ipar[128]; // integer parameters
	double dpar[128]; // double precision parameters 
};
/**************************************/
/*
 * plans[0]: work[0]
 * plans[1]: work[1]
 */
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
/*
 * dpar[0] = g factor
 * dpar[1] = mua (absorption coefficient)
 * dpar[2] = mus (scattering coefficient)
 */
struct st_solver_v1 *sv1_create_solver(struct st_mesh *q, const int *ipar, const double *dpar);
// A.x0=b0
void sv1_gen_b0(struct st_solver_v1 *s, const double phis, double _Complex *restrict b0);
// A.x1=b1, x=x0+x1
void sv1_gen_b1x0(struct st_solver_v1 *s, const double phis, 
		double _Complex *restrict b1, double _Complex *restrict x0);
void sv1_solve(struct st_solver_v1 *s, double _Complex *rhs, double _Complex *sol, 
		const int max_nitr, const int max_nonrestart_nitr, const double retol, 
		int *restrict nitr, double *restrict eps);
void sv1_mul(struct st_solver_v1 *s, const double _Complex *restrict in, double _Complex *restrict out);
void sv1_save_solution(struct st_solver_v1 *s, const double _Complex *v, const char *dir);
void sv1_print_solverinfo(struct st_solver_v1 *s);
void sv1_destroy_solver(struct st_solver_v1 *s);
#ifdef __cplusplus
}
#endif
/******************************************************************************/
#endif /* end of include guard */
