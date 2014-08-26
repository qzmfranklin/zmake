#ifndef _RMSM_H_
#define _RMSM_H_

/*
 * Row Major Sparse Matrix (RMSM)
 *
 * Optimized for matrix(real,sparse)-vector(real/complex,dense) multiplication.
 *
 * NOTE:
 * 	1. The user interface is opaque.
 * 	2. The user owns any vectors that are passed to RMSM functions.
 * 	3. RMSM only supports real matrix, though you can multiply this real 
 * 	matrix by either real or complex vectors.
 */


#include <complex.h>

#ifdef __cplusplus
extern "C" {
#endif

// internal use only
typedef struct {
	int col;
	double val;
} vessel_t;

struct st_rmsm {
	int     status; // internal status
	int     len; // number of columns/rows
	int    *rsz; // rsz[k] = number of non-zero elements in the k-th row
	int    *pos; // see rmsm_mul
	double *data;// internal representation of matrix
	void   *tmp; // internal use only
};

struct st_rmsm *rmsm_create(const int len);
void rmsm_add(struct st_rmsm *m, const double val, const int row, const int col);
void rmsm_pack(struct st_rmsm *m);
void rmsm_print_info(const struct st_rmsm *m);
void rmsm_destroy(struct st_rmsm *m);
void rmsm_mul(const struct st_rmsm *m,
		const double *restrict in,
		double *restrict out);
void rmsm_mul_complex(const struct st_rmsm *m,
		const double _Complex *restrict in,
		double _Complex *restrict out);
void rmsm_print_row(const struct st_rmsm *m, const int row);

#ifdef __cplusplus
}
#endif 



#endif /* end of include guard */
