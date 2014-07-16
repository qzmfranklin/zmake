#include "msh_ray_tracing.h"
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <mkl.h>
/******************************************************************************/
static int ipow(int m, int n); // returns m^n
static int meshnode_isin(struct st_meshnode *node, const double x0, const double y0);
static void print_node(struct st_meshnode *node);

// si=sin(phi) ic=1.0/cos(phi) x0=p[0] y0=p[1]
static int square_ray_isint(const double x1, const double y1, const double x2, const double y2, 
		const double x0, const double y0, const double si, const double ic);
static double trig_ray_seg(const double *restrict trig, 
		const double x0, const double y0, const double si, const double ic);
/******************************************************************************/
double meshtree_ray_seg(const struct st_mesh *restrict t, 
		const double p[2], const double phi)
{
	//TODO
	//double si,ic;
	//sincos(phi,&si,&ic);
	//ic = 1.0/ic;


	return 0.0;
}

struct st_meshtree *create_meshtree(const int num_trigs, 
		const double *restrict trigs, const double *restrict limits,
		const int npl)
{
	//fprintf(stderr,"create_meshtree()\n");

	if (npl<1) {
		fprintf(stderr,"t->npl=%d, should be at least 1. Abort.\n",npl);
		abort();
	}

	struct st_meshtree *t;
	t = (struct st_meshtree*)mkl_malloc(sizeof(st_meshtree),64);
	assert(t); 

	// allocate memory for the packed nodes
	t->status=0;
	t->npl=npl;
	t->ntot = (ipow(4,npl+1)-1) / 3;
	printf("#packed levels = %d\n",npl);
	printf("#nodes         = %d\n",t->ntot);
	printf("#leaves        = %d\n",(3*t->ntot+1)/4);
	printf("#triangles     = %d\n",num_trigs);
	t->root=(struct st_meshnode*)mkl_malloc(sizeof(struct st_meshnode)*t->ntot,64);
	assert(t->root);
	//fprintf(stderr,"t->root allocated\n");

	struct st_meshnode *p = t->root; // for easy reference

	// construct the packed levels
	{// level=0: root node
		p->x1   = limits[0];
		p->y1   = limits[1];
		p->x2   = limits[2];
		p->y2   = limits[3];
		p->n    = 0;
		p->trigs.a=NULL;
	}
	// level>0
	for (int i = 1; i < t->ntot; i++) {
		int j=(i-1)>>2;	// parent = [(child-1)/4]
		int k=(i-1)%4+1;// remainder
		//fprintf(stderr,"[%6d = 4 * %-6d + %-6d]\n",i,j,k);
		switch (k) {
		case 1:
			p[i].x1 = p[j].x1;
			p[i].y1 = p[j].y1;
			p[i].x2 = 0.5*(p[j].x1 + p[j].x2);
			p[i].y2 = 0.5*(p[j].y1 + p[j].y2);
			break;
		case 2:
			p[i].x1 = 0.5*(p[j].x1 + p[j].x2);
			p[i].y1 = p[j].y1;
			p[i].x2 = p[j].x2;
			p[i].y2 = 0.5*(p[j].y1 + p[j].y2);
			break;
		case 3:
			p[i].x1 = p[j].x1;
			p[i].y1 = 0.5*(p[j].y1 + p[j].y2);
			p[i].x2 = 0.5*(p[j].x1 + p[j].x2);
			p[i].y2 = p[j].y2;
			break;
		case 4:
			p[i].x1 = 0.5*(p[j].x1 + p[j].x2);
			p[i].y1 = 0.5*(p[j].y1 + p[j].y2);
			p[i].x2 = p[j].x2;
			p[i].y2 = p[j].y2;
			break;
		}
		p[i].n=0;
		p[i].trigs.a=NULL;
	}
	// init leaves, expect average filling rate < 4 triangles
	for (int i = (ipow(4,npl)-1)/3; i < t->ntot; i++) {
		p[i].trigs.v = new std::vector<double> (24);
		p[i].trigs.v->clear();
	}

	{ // Insert mesh into the quad-tree
	const double xx=1.0/3.0;
	for (int i = 0; i < num_trigs; i++) {
		const double x0=xx*(trigs[6*i+0]+trigs[6*i+2]+trigs[6*i+4]);
		const double y0=xx*(trigs[6*i+1]+trigs[6*i+3]+trigs[6*i+5]);
		// check that the center lies in the square
		if (!meshnode_isin(p,x0,y0)) {
			fprintf(stderr,"trigs[%5d] (%f,%f) is out of (%f,%f) - (%f,%f). ",
					i,x0,y0,p->x1,p->y1,p->x2,p->y2);
			fprintf(stderr,"Abort.\n");
			abort();
		}
		int k=0; // p[k] is the current node
		// Upon exit of the for loop, p[k] is the leaf that contains trigs[i].
		for (int j = 0; j < npl; j++) {
			int child=1;
			if ( x0>0.5*(p[k].x1+p[k].x2) ) child+=1;
			if ( y0>0.5*(p[k].y1+p[k].y2) ) child+=2;
			k = 4*k + child;
		}
		//printf("trigs[%3d](%f,%f) -> square[%3d](%f,%f)-(%f,%f)\n",
				//i,x0,y0,k,p[k].x1,p[k].y1,p[k].x2,p[k].y2);
		for (int index = 0; index < 6; index++)
			p[k].trigs.v->push_back(trigs[6*i+index]);
	}
	}

	{ // check that all triangles are in the quad-tree
		int tmp=0;
		for (int i = (ipow(4,npl)-1)/3; i < t->ntot; i++)
			tmp += p[i].trigs.v->size()/6;
		if (tmp!=num_trigs) {
			fprintf(stderr,"Quad-tree only has %d triangles, ",tmp);
			fprintf(stderr,"less than the expected value of %d. ",num_trigs);
			fprintf(stderr,"Abort.\n");
			abort();
		}
		printf("Successfully constructed the quad-tree.\n");
	}

	{ // release all std::vector's, pack data into t->data
	t->ptt=(double*)mkl_malloc(sizeof(double)*6*num_trigs,64);
	assert(t->ptt);
	double *ptt=t->ptt;
	for (int i = (ipow(4,npl)-1)/3; i < t->ntot; i++) {
		p[i].n = (int) p[i].trigs.v->size();
		double *tmp = ptt;
		for (int j = 0; j < p[i].n; j++)
			*ptt++ = p[i].trigs.v->at(j);
		delete p[i].trigs.v;
		p[i].trigs.a = tmp;
	}
	printf("Successfully packed %d triangles.\n",(int) (ptt - t->ptt)/6);
	}

	return t;
}

void destroy_meshtree(struct st_meshtree *t)
{
	//fprintf(stderr,"destroy_meshtree()\n");

	mkl_free(t->ptt);
	mkl_free(t->root);
	mkl_free(t);
}
/******************************************************************************/
// si = sin(phi)	ic = 1.0.cos(phi)
static int square_ray_isint(const double x1, const double y1, const double x2, const double y2, 
		const double x0, const double y0, const double si, const double ic)
{
	double t1, t2;
	t1 = (x1-x0)*ic;
	t2 = (x2-x0)*ic;
	{ // the line does not touch the square at all
		int n1=0, n2=0;
		const double f1=y0+t1*si;
		const double f2=y0+t2*si;
		if (f1<y1) n1--;
		if (f1>y2) n1++;
		if (f2<y1) n2--;
		if (f2>y2) n2++;
		if (n1*n2>0) return 0;
	}
	if (t1>0.0 | t2>0.0) return 1;
	return 0;
}

static int meshnode_isin(struct st_meshnode *node, const double x0, const double y0)
{
	return ((x0-node->x1)*(x0-node->x2) < 0) && 
		((y0-node->y1)*(y0-node->y2) < 0);
}

static int ipow(int m, int n)
{
	int temp=1;
	for (int i = 0; i < n; i++) 
		temp *= m;
	return temp;
}

static void print_node(struct st_meshnode *node)
{
	printf("(%f,%f)-(%f,%f)\n",node->x1,node->y1,node->x2,node->y2);
}
