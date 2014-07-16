#ifndef _RMSM_H_
#define _RMSM_H_
/******************************************************************************/
/*
 * Row Major Spase Matrix (RMSM)
 * Only be included indirectly through including spmat. through including
 * spmat.h. Not in the user space, i.e., not for direct use.
 */
#include <complex.h>
#include <vector>
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
// intdbl_t is solely intended for internal use
typedef struct {
	int i;
	double d;
} intdbl_t;
// row-major sparse square matrix, good for A.x operation
struct st_rmsm {
	int status; // internal status
	int size; // dimension, i.e., size of this matrix
	int length; // num of non-zero elements, = length of col,data
	int *pos; // i-th row starts from col[pos[i]] and data[pos[i]]
	int *rsz; // i-th row has rsz[i] non-zero elements, 'rsz' stands for row size
	int *col;
	double *data;
	std::vector<intdbl_t> *tmp; // used only when unpacked
};
struct st_rmsm *rmsm_create(const int size);
void rmsm_add(struct st_rmsm *m, const int row, const int col, const double val);
void rmsm_pack(struct st_rmsm *m);
void rmsm_print_matrixinfo(const struct st_rmsm *m);
void rmsm_destroy(struct st_rmsm *m);
void rmsm_dmul(const struct st_rmsm *m, const double *restrict in, double *restrict out);
void rmsm_zmul(const struct st_rmsm *m, const double _Complex *restrict in, double _Complex *restrict out);
void rmsm_print_row(const struct st_rmsm *m, const int row);
#ifdef __cplusplus
}
#endif 
/******************************************************************************/
#endif /* end of include guard */
