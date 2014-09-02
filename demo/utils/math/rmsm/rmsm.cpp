#include "rmsm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <algorithm>

#include <iostream>

struct st_rmsm *rmsm_create(const int len)
{
	st_rmsm *m = (st_rmsm*)malloc(sizeof(st_rmsm));
	assert(m);

	m->status=0;
	m->len = len;
	m->rsz = NULL;
	m->col = NULL;
	m->val = NULL;
	m->tmp = new std::vector<vessel_t>[len];

	m->status=1;

	return m;
}

void rmsm_add(struct st_rmsm *m, const double val, const int row, const int col)
{
	assert(m->status==1);

	assert(row>=0 && row < m->len);
	assert(col>=0 && col < m->len);

	vessel_t tmp={col,val};
	m->tmp[row].push_back(tmp);

	//fprintf(stderr,"[%5d,%5d] %.5E %lu\n",row,col,val,m->tmp[row].size());
}

static bool cmp(const vessel_t &a, const vessel_t &b) { return (a.col < b.col); }
static void merge_dup(struct st_rmsm *m, std::vector<vessel_t> *tmp)
{
	//printf("merge_dup\n");
	std::vector<vessel_t> *tmp0 = (std::vector<vessel_t>*) m->tmp;
	for (int i = 0; i < m->len; i++) {
		if (tmp0[i].size()==0) continue;
		tmp[i].push_back(tmp0[i].at(0));
		//printf("%3d %3d %.6E\n",i,0,tmp0[i].at(0).val);
		int curr_col = tmp[i].at(0).col;
		for (int j = 1; j < tmp0[i].size(); j++) {
			int    col = tmp0[i].at(j).col;
			double val = tmp0[i].at(j).val;
			//printf("%3d %3d %.6E  ",i,col,val);
			if (col==curr_col) {
				//printf("+=\n");
				tmp[i].back().val += val;
			} else {
				//printf("push_back\n");
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
	//printf("pack\n");
	int k=0;
	for (int i = 0; i < m->len; i++) {
		m->rsz[i] = tmp[i].size();
		for (int j = 0; j < tmp[i].size(); j++) {
			m->col[k] = tmp[i].at(j).col;
			m->val[k] = tmp[i].at(j).val;
			//printf("%3d %3d   %.6E\n",i,m->col[k],m->val[k]);
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

	const int size = ntot(m,tmp);
	m->rsz = (int*)   malloc(sizeof(double)*size);
	m->val = (double*)malloc(sizeof(double)*size);
	m->col = (int*)   malloc(sizeof(int)   *size);
	assert(m->rsz);
	assert(m->val);
	assert(m->col);

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
			out[i] += m->val[k] * in[m->col[k]];
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
			out[i] += m->val[k] * in[m->col[k]];
			k++;
		}
}

void rmsm_print_info(const struct st_rmsm *m)
{
	assert(m->status > 0);
	printf("sizeof(struct st_rmsm) = %lu\n",sizeof(struct st_rmsm));
	printf("                status = %d\t",m->status);
	printf("0=uninit'd 1=init'd 2=packed(ready for use)\n");
	printf("                   len = %d\t",m->len);
	printf("number of columns/rows\n");

	{ int k=0;
	for (int i = 0; i < m->len; i++) {
		printf("%5d  |",i);
		for (int j = 0; j < m->rsz[i]; j++) {
			printf("  %3d %.2E",m->col[k],m->val[k]);
			k++;
		}
		printf("\n");
	}
	}
}

void rmsm_destroy(struct st_rmsm *m)
{
	assert(m->status > 0);

	if (m->status==1) {
		delete [] m->tmp;
	} else if (m->status==2) {
		free(m->rsz);
		free(m->col);
		free(m->val);
	}
	free(m);
}

int rmsm_count_operation(const struct st_rmsm *m)
{
	int sum=0;
	for (int i = 0; i < m->len; i++) {
		const int tmp = m->rsz[i];
		if (tmp>0)
			sum += 2*tmp - 1;
	}
	return sum;
}
