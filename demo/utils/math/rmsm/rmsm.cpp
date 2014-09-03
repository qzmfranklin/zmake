#include "rmsm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <math.h>

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

static void delete_zero_absolute_error(const struct st_rmsm *m, std::vector<vessel_t> *tmp, const double eps)
{
	for (int i = 0; i < m->len; i++)
		for (int j = tmp[i].size() - 1; j >=0 ; j--)
			if (  fabs(tmp[i].at(j).val) < eps )
				tmp[i].erase(tmp[i].begin()+j);
}

static double vessel_norm(const struct st_rmsm *m, const std::vector<vessel_t> *tmp)
{
	double norm2 = 0.0;
	for (int i = 0; i < m->len; i++)
		for (int j = 0; j < tmp[i].size(); j++) {
			const double xx = tmp[i].at(j).val;
			norm2 += xx * xx;
		}
	return sqrt(norm2);
}

static void delete_zero_relative_error(const struct st_rmsm *m, std::vector<vessel_t> *tmp, const double eps)
{
	const double abs_err = vessel_norm(m,tmp) * eps;
	delete_zero_absolute_error(m,tmp,abs_err);
}

// total number of non-empty elements
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
		//printf("rsz[%d] = %d\n",i,m->rsz[i]);
		for (int j = 0; j < tmp[i].size(); j++) {
			m->col[k] = tmp[i].at(j).col;
			m->val[k] = tmp[i].at(j).val;
			//printf("%3d %3d   %.6E\n",i,m->col[k],m->val[k]);
			k++;
		}
	}
}

static bool cmp(const vessel_t &a, const vessel_t &b) { return (a.col < b.col); }
void rmsm_pack(struct st_rmsm *m, const int flag, const double eps)
{
	assert(m->status==1);

	for (int i = 0; i < m->len; i++)
		std::sort(m->tmp[i].begin(),m->tmp[i].end(),cmp);

	std::vector<vessel_t> *tmp = new std::vector<vessel_t>[m->len];

	merge_dup(m,tmp);
	delete [] m->tmp;

	if (flag==RMSM_ABSERR)
		delete_zero_absolute_error(m,tmp,eps);
	else if (flag==RMSM_RELERR)
		delete_zero_relative_error(m,tmp,eps);
	else {
		fprintf(stderr,"rmsm_pack: unknown flag %d\n",flag);
		exit(1);
	}

	const int size = ntot(m,tmp);
	if (size==0) goto empty;

	m->rsz = (int*)   malloc(sizeof(int) * m->len);
	m->val = (double*)malloc(sizeof(double)*size);
	m->col = (int*)   malloc(sizeof(int)   *size);
	assert(m->rsz);
	assert(m->val);
	assert(m->col);

	pack(m,tmp);

	delete [] tmp;
	m->status=2;
	return;

empty:
	fprintf(stderr,"rmsm_pack: empty matrix\n");
	delete [] tmp;
	m->status=3;
	return;
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
	assert(m->status >= 2);

	memset(out,0,sizeof(double) * m->len);

	if (m->status == 3) return;

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
	assert(m->status >= 2);

	memset(out,0,sizeof(double _Complex) * m->len);

	if (m->status == 3) return;

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
	printf("0=unalloc'd 1=alloc'd 2=packed 3=packed,empty\n");
	printf("                   len = %d\t",m->len);
	printf("number of columns/rows\n");
	printf("    number of elements = %d\n",rmsm_num_elements(m));

	if (m->status == 3) return;

	int k=0;
	for (int i = 0; i < m->len; i++) {
		printf("%5d  |",i);
		for (int j = 0; j < m->rsz[i]; j++) {
			printf("  %3d %.2E",m->col[k],m->val[k]);
			k++;
		}
		printf("\n");
	}
}

void rmsm_destroy(struct st_rmsm *m)
{
	//fprintf(stderr,"rmsm_destroy\n");
	assert(m->status > 0);

	if (m->status==1) {
		delete [] m->tmp;
	} else if (m->status==2) {
		free(m->rsz);
		free(m->col);
		free(m->val);
	} else if (m->status==3) {
		;
	}
	free(m);
}

int rmsm_num_elements(const struct st_rmsm *m)
{
	if (m->status == 3)
		return 0;
	else if (m->status < 2)
		return -1;

	int sum=0;
	for (int i = 0; i < m->len; i++)
		sum += m->rsz[i];
	return sum;
}

int rmsm_count_operation(const struct st_rmsm *m)
{
	if (m->status < 2) 
		return -1;
	else if (m->status == 3)
		return 0;

	int sum=0;
	for (int i = 0; i < m->len; i++) {
		const int tmp = m->rsz[i];
		if (tmp>0)
			sum += 2*tmp - 1;
	}
	return sum;
}
