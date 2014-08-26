#include "rmsm.h"
#include <stdio.h>
#include <assert.h>
#include <vector.h>


/*
 *typedef struct {
 *        int col;
 *        double val;
 *} vessel_t;
 *
 *struct st_rmsm {
 *        int     status; // internal status
 *        int     len; // number of columns/rows
 *        int    *rsz; // rsz[k] = number of non-zero elements in the k-th row
 *        int    *pos; // see rmsm_mul for description and utility
 *        double *data;// internal representation of matrix
 *        void   *tmp; // internal use only
 *};
 */

struct st_rmsm *rmsm_create(const int len)
{
	st_rmsm *m = (st_rmsm*)malloc(sizeof(st_rmsm));
	assert(m);

	m->status=0;
	m->len = len;
	m->rsz = (int*)malloc(sizeof(int)*len);
	assert(rsz);
	m->pos = NULL;
	m->data= NULL;
	m->tmp = (void*) new std::vector<vessel_t>[4];

	m->status=1;
}

void rmsm_add(struct st_rmsm *m, const double val, const int row, const int col)
{
	assert(m->status==1);

#ifndef RELEASE
	assert(row>=0 && row < m->len);
	assert(col>=0 && col < m->len);
#endif

	vessel_t tmp={col,val};
	m->tmp[row].push_back(tmp);
}

static bool cmp(const vessel_t &a, const vessel_t &b) { return (a.col < b.col); }
static void merge_dup(struct st_rmsm *m, std::vector<vessel_t> *tmp)
{
	std::vector<vessel_t> *tmp0 = (std::vector<vessel_t>*) m->tmp;
	for (int i = 0; i < m->len; i++) {
		if (tmp0[i].size()==0) continue;
		tmp[i].push_back(tmp0[i].at(0));
		int curr_col = tmp[i].at(0).col;
		for (int j = 1; j < tmp0[i].size(); j++) {
			int    col = tmp0[i].at(j).col;
			double val = tmp0[i].at(j).val;
			if (col==curr_col)
				tmp[i].back().val += val;
			else {
				tmp[i].push_back(tmp0[i].at(j));
				curr_col = col;
			}
		}
	}
}

// total number of non-empty and distinct elements
static int ntot(struct st_rmsm *m, std::vector<vessel_t> *tmp)
{
	int ntot=0;
	for (int i = 0; i < m->len; i++)
		ntot += tmp[i].size();
	return ntot;
}

static void pack(struct st_rmsm *m, std::vector<vessel_t> *tmp)
{
	int k=0;
	for (int i = 0; i < m->len; i++) {
		for (int j = 0; j < tmp[i].size(); j++) {
			m->pos[k]  = tmp[i].at(j).pos;
			m->data[k] = tmp[i].at(j).val;
			k++;
		}
	}
}

void rmsm_pack(struct st_rmsm *m)
{
	assert(m->status==1);

	for (int i = 0; i < m->len; i++)
		std::sort(m->tmp[i].begin(),m->tmp[i].end(),cmp);

	std::vector<vessel_t> *tmp = new std::vector<vessel_t>[m->len];

	merge_dup(m,tmp);
	delete [] m->tmp;

	const int ntot = ntot(m,tmp);
	m->data = (double*)malloc(sizeof(double)*ntot);
	m->pos  = (double*)malloc(sizeof(int)*ntot);
	assert(m->data);
	assert(m->pos);

	pack(m,tmp);

	delete [] tmp;

	m->status=2;
}

/*
 * The user is responsible for the alloc and free of *in and *out. rmsm_mul
 * assumes that *in and *out does not overlap but does not check it for the
 * reason of performance.
 *
 * This function, together with its complex version (rmsm_mul_complex),
 * optimized memory access pattern and is the core of RMSM.
 */
void rmsm_mul(const struct st_rmsm *m,
		const double *restrict in,
		double *restrict out)
{
	assert(m->status==2);

	memset(out,0,sizeof(double) * m->len);
	int k=0;
	for (int i = 0; i < m->len; i++)
		for (int j = 0; j < m->rsz[i]; j++) {
			out[i] += m->data[k] * in[m->pos[k]];
			k++;
		}
}

void rmsm_mul_complex(const struct st_rmsm *m,
		const double _Complex *restrict in,
		double _Complex *restrict out)
{
	assert(m->status==2);

	memset(out,0,sizeof(double _Complex) * m->len);
	int k=0;
	for (int i = 0; i < m->len; i++)
		for (int j = 0; j < m->rsz[i]; j++) {
			out[i] += m->data[k] * in[m->pos[k]];
			k++;
		}
}

void rmsm_destroy(struct st_rmsm *m)
{
	assert(m->status==2);
	free(m->rsz);
	free(m->pos);
	free(m->data);
	free(m);
}
