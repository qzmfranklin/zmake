#include <stdio.h>
#include <complex.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <assert.h>
#include <mkl.h>
#include "rmsm.h"
/******************************************************************************/
// row-major sparse matrix, good for A.x operation
/*
 *struct st_rmsm {
 *        int status; // internal status
 *        int size; // dimension, i.e., size of this matrix
 *        int *pos; // i-th row starts from n[pos[i]] and a[pos[i]]
 *        int *rsz; // i-th row has rsz[i] non-zero elements, 'rsz' stands for row size
 *        int **col
 *        double **data;
 *        std::vector<intdbl_t> *tmp; // used only when unpacked
 *};
 */
struct st_rmsm *rmsm_create(const int size)
{
	//fprintf(stderr,"rmsm_create()\n"); 

	struct st_rmsm *m=(struct st_rmsm*)mkl_malloc(sizeof(struct st_rmsm),64);
	assert(m);

	m->status=0; 

	m->size  = size;
	m->pos   = (int*)mkl_malloc(sizeof(int)*size,64);
	m->rsz   = (int*)mkl_malloc(sizeof(int)*size,64);
	m->col   = NULL;
	m->data  = NULL;
	m->tmp   = new std::vector<intdbl_t>[size];
	assert(m->pos); 
	assert(m->rsz); 
	assert(m->tmp); 

	m->status=1;

	return m;
}
void rmsm_add(struct st_rmsm *m, const int row, const int col, const double val)
{
	assert(m->status==1);

	// turn off bound check in production release
	assert(row>=0 && row<m->size);
	assert(col>=0 && col<m->size);

	intdbl_t tmp={col,val};
	m->tmp[row].push_back(tmp);

	//fprintf(stderr,"[%5d,%5d] %f\n",row,
			//m->tmp[row].back().i,
			//m->tmp[row].back().d);
}
static bool cmp(const intdbl_t &a, const intdbl_t &b) { return (a.i < b.i); }
void rmsm_pack(struct st_rmsm *m)
{
	//fprintf(stderr,"rmsm_pack()\n");
	assert(m->status==1);

	//for (int i = 0; i < m->size; i++)
		//printf("[%5d] %lu\n",i,m->tmp[i].size());

	//const int row=73; // examine this row

	//printf("row %d (raw)\n",row);
	//for (int i = 0; i < m->tmp[row].size(); i++)
		//printf("[%5d] %f\n",m->tmp[row].at(i).i,m->tmp[row].at(i).d); 

	// sort each row
	for (int n = 0; n < m->size; n++)
		std::sort(m->tmp[n].begin(),m->tmp[n].end(),cmp);

	//printf("(sorted)\n");
	//for (int i = 0; i < m->tmp[row].size(); i++)
		//printf("[%5d] %f\n",m->tmp[row].at(i).i,m->tmp[row].at(i).d); 

	// merge
	std::vector<int>    *vi = new std::vector<int>[m->size];
	std::vector<double> *vd = new std::vector<double>[m->size];
	for (int n = 0; n < m->size; n++) {
		if (m->tmp[n].size()==0) continue;
		vi[n].push_back(m->tmp[n].at(0).i);
		vd[n].push_back(m->tmp[n].at(0).d);
		int curr_col=vi[n].at(0);
		for (int j = 1; j < m->tmp[n].size(); j++) {
			int    col=m->tmp[n].at(j).i;
			double val=m->tmp[n].at(j).d;
			if (col==curr_col)
				vd[n].back() += val;
			else {
				vi[n].push_back(col);
				vd[n].push_back(val); 
				curr_col = col;
			}
		}
	}
	delete [] m->tmp;

	//printf("(merged)\n");
	//for (int i = 0; i < vi[row].size(); i++)
		//printf("[%5d] %f\n",vi[row].at(i),vd[row].at(i));

	// pack
	{ int ptr=0;
	for (int i = 0; i < m->size; i++) {
		m->pos[i] = ptr;
		ptr      += vi[i].size();
		m->rsz[i] = vi[i].size();
	}
	m->length=ptr;
	}
	m->col =(int*)   mkl_malloc(sizeof(int)   *(m->length),64);
	m->data=(double*)mkl_malloc(sizeof(double)*(m->length),64);
	for (int i = 0; i < m->size; i++)
		for (int j = 0; j < vi[i].size(); j++) {
			m->col [j+m->pos[i]] = vi[i].at(j);
			m->data[j+m->pos[i]] = vd[i].at(j);
		}
	delete [] vi;
	delete [] vd;

	//printf("(packed)\n");
	//for (int i = 0; i < m->rsz[row]; i++)
		//printf("[%5d] %f\n",m->col[i+m->pos[row]],m->data[i+m->pos[row]]);

	m->status=2;
}
void rmsm_print_matrixinfo(const struct st_rmsm *m)
{
	//TODO
	assert(m->status>0);
	printf("    status = %d  ", m->status);
	printf("(0=uninit'd 1=init'd 2=packed)\n");
}
void rmsm_dmul(const struct st_rmsm *m, const double *restrict in, double *restrict out)
{
	assert(m->status==2);

	memset(out,0,sizeof(double)*(m->size));
	for (int i = 0; i < m->size; i++)
		for (int j = 0; j < m->rsz[i]; j++)
			out[i] += m->data[j+m->pos[i]] * in[m->col[j+m->pos[i]]];
}
void rmsm_zmul(const struct st_rmsm *m, const double _Complex *restrict in, double _Complex *restrict out)
{
	assert(m->status==2);

	memset(out,0,sizeof(double _Complex)*(m->size));
	for (int i = 0; i < m->size; i++)
		for (int j = 0; j < m->rsz[i]; j++)
			out[i] += m->data[j+m->pos[i]] * in[m->col[j+m->pos[i]]];
}
void rmsm_print_row(const struct st_rmsm *m, const int row)
{
	assert(m->status==2);

	printf("row %-5d ",row);
	for (int i = 0; i < m->rsz[row]; i++) {
		const int col = m->col[i+m->pos[row]];
		printf("%5d %+.2E ",col,m->data[col]);
	}
	printf("\n");
}
void rmsm_destroy(struct st_rmsm *m)
{
	//fprintf(stderr,"rmsm_destroy()\n");
	assert(m->status==2);
	mkl_free(m->pos);
	mkl_free(m->rsz);
	mkl_free(m->col);
	mkl_free(m->data);
	mkl_free(m);
}
