#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <mkl.h>
#include "mshio.h" 
/******************************************************************************/ 
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
/******************************************************************************/ 
static struct st_mesh *init_mesh(const char *fbase)
{
	struct st_mesh *q;
	q=(struct st_mesh*)mkl_malloc(sizeof(struct st_mesh),64);
	assert(q);

	strcpy(q->fbase,fbase);
	q->status=0;

	return q;
}

static void read_info(struct st_mesh *q)
{
	//printf("read_info(stuct st_mesh &q)");
	assert(q->status==0); 

	char filename_info[FILENAME_MAX];
	strcpy(filename_info,q->fbase);
	strcat(filename_info,".info");

	// Open fin_info for read
	FILE *fin_info;
	fin_info = fopen(filename_info,"r");
	assert(fin_info);

	// Scan fin_info
	fscanf(fin_info,"%d\t\t# 1=ASCII 2=BINARY",&q->format);
	fscanf(fin_info,"%d\t\t# number of verts" ,&q->num_verts);
	fscanf(fin_info,"%d\t\t# number of trigs" ,&q->num_trigs);
	fscanf(fin_info,"%d\t\t# number of edges" ,&q->num_edges);
	fclose(fin_info);

	q->status=1; 
}

static void alloc_mesh(struct st_mesh *q)
{
	//fprintf(stderr,"alloc_mesh(struct st_mesh &q)\n");
	assert(q->status==1);

	q->v =(double*)mkl_malloc(sizeof(double)*2*(q->num_verts),64);
	//q->e =(int*)   mkl_malloc(sizeof(int)   *2*(q->num_edges),64);
	q->t =(int*)   mkl_malloc(sizeof(int)   *3*(q->num_trigs),64);
	q->te=(int*)   mkl_malloc(sizeof(int)   *2*(q->num_trigs),64);
	q->p =(double*)mkl_malloc(sizeof(double)*6*(q->num_trigs),64);
	q->a =(double*)mkl_malloc(sizeof(double)*1*(q->num_trigs),64);
	q->c =(double*)mkl_malloc(sizeof(double)*2*(q->num_trigs),64);
	assert(q->v);
	//assert(q->e);
	assert(q->t);
	assert(q->te);
	assert(q->p);
	assert(q->a);
	assert(q->c);

	q->status=2;
}

// return (a1-a2)x(b1-b2)
static inline double det2x2(const double *restrict a1, const double *restrict a2,
		const double *restrict b1, const double *restrict b2)
{
	const double a=a1[0]-a2[0];
	const double b=a1[1]-a2[1];
	const double c=b1[0]-b2[0];
	const double d=b1[1]-b2[1];
	return a*d-b*c;
}
// return area of triangle defined by p1,p2,p3
static double trigarea(const double *restrict p1, const double *restrict p2, const double *restrict p3)
{
	return 0.5*det2x2(p3,p1,p2,p1);
}
static void swp(int *a, int *b)
{
	int c=*a;
	*a=*b;
	*b=c;
}
static void swp(double *a, double *b)
{
	double c=*a;
	*a=*b;
	*b=c;
}
static void read_mesh(struct st_mesh *q)
{
	//fprintf(stderr,"load_mesh(struct st_mesh &q)\n");
	assert(q->status==2);

	char filename_verts[FILENAME_MAX];
	char filename_trigs[FILENAME_MAX];
	char filename_edges[FILENAME_MAX];
	strcpy(filename_verts,q->fbase);
	strcpy(filename_trigs,q->fbase);
	strcpy(filename_edges,q->fbase);
	strcat(filename_verts,".verts");
	strcat(filename_trigs,".trigs");
	strcat(filename_edges,".edges");

	// Open files
	FILE *fin_verts,*fin_trigs,*fin_edges;
	fin_verts=fopen(filename_verts,"r");
	fin_trigs=fopen(filename_trigs,"r");
	//fin_edges=fopen(filename_edges,"r");
	assert(fin_verts);
	assert(fin_trigs);
	//assert(fin_edges);

	switch (q->format) {
	case 1: // 1=ASCII
		for (int i = 0; i < q->num_verts; i++)
			fscanf(fin_verts,"%lf %lf\n",
					q->v+2*i,q->v+2*i+1);
		for (int i = 0; i < q->num_trigs; i++)
			fscanf(fin_trigs,"%d %d %d\n",
					q->t+3*i,q->t+3*i+1,q->t+3*i+2);
		//for (int i = 0; i < q->num_edges; i++)
			//fscanf(fin_edges,"%d %d\n",
					//q->e+2*i,q->e+2*i+1);
		break;
	case 2: // 2=BINARY
		fread(q->v,sizeof(double),2*q->num_verts,fin_verts);
		fread(q->t,sizeof(int)   ,3*q->num_trigs,fin_trigs);
		//fread(q->e,sizeof(int)   ,3*q->num_edges,fin_edges);
		break;
	}
	fclose(fin_verts);
	fclose(fin_trigs);

	// MSH indices starts from 1, C/C++ indices starts from 0
	for (int i = 0; i < 3*q->num_trigs; i++)
		q->t[i]--;

	// Assemble q->p, ensure right-handed, i.e., have positive area
	for (int i = 0; i < q->num_trigs; i++)  {
		q->p[6*i  ] = q->v[2*q->t[3*i  ]  ];  // x0
		q->p[6*i+1] = q->v[2*q->t[3*i  ]+1];  // y0
		q->p[6*i+2] = q->v[2*q->t[3*i+1]  ];  // x1
		q->p[6*i+3] = q->v[2*q->t[3*i+1]+1];  // y1
		q->p[6*i+4] = q->v[2*q->t[3*i+2]  ];  // x2
		q->p[6*i+5] = q->v[2*q->t[3*i+2]+1];  // y2
		// if left-handed, swap p1 and p2 (p3 untouched)
		if ((q->a[i]=trigarea(q->p+6*i,q->p+6*i+2,q->p+6*i+4)) < 0.0) {
			swp(q->t+3*i  ,q->t+3*i+1);
			swp(q->p+6*i  ,q->p+6*i+2);
			swp(q->p+6*i+1,q->p+6*i+3);
			q->a[i] = -q->a[i];
		}
	}

	// Compute the centers of triangles
	{ const double xx=1.0/3.0;
	for (int i = 0; i < q->num_trigs; i++) {
		q->c[2*i  ] = xx * (q->p[6*i  ]+q->p[6*i+2]+q->p[6*i+4]);
		q->c[2*i+1] = xx * (q->p[6*i+1]+q->p[6*i+3]+q->p[6*i+5]);
	} }

	q->status=3;
}

static void assemble_te(struct st_mesh *q)
{
	//TODO
}

/******************************************************************************/ 
struct st_mesh *mshio_create_mesh(const char *fbase)
{
	struct st_mesh *q;

	q=init_mesh(fbase);
	read_info(q);
	alloc_mesh(q);
	read_mesh(q);
	assemble_te(q);

	return q;
}

void mshio_destroy_mesh(struct st_mesh *q)
{
	//fprintf(stderr,"release_mesh()\n");
	assert(q->status>=2);

	mkl_free(q->v);
	//mkl_free(q->e);
	mkl_free(q->t);
	mkl_free(q->te);
	mkl_free(q->p);
	mkl_free(q->a);
	mkl_free(q->c);

	q->status=-1;
}

void mshio_print_mesh(struct st_mesh *q, int flag)
{
	//fprintf(stderr,"mshio_print_mesh()\n"); 

	printf("    format = %d  ", q->format);
	printf("(1=ASCII 2=BINARY)\n");
	printf("    status = %d  ", q->status);
	printf("(0=uninit'd 3=ready)\n");
	printf(" num_verts = %d\n", q->num_verts);
	printf(" num_trigs = %d\n", q->num_trigs);
	//printf(" num_edges = %d\n", q->num_edges);
	printf("V->COORD v = %p\n", q->v);
	printf("T->V     t = %p\n", q->t);
	printf("T->E    te = %p\n", q->te);
	printf("T->COORD p = %p\n", q->p);
	printf("T->AREA  a = %p\n", q->a);

	if (!flag) { printf("\n"); return; }

	assert(q->status==3); // after calling mshio_create_mesh()
	printf("verts:\n");
	for (int i = 0; i < MIN(20,q->num_verts); i++)
		printf("[%7d] (%8.3lf,%8.3lf)\n",
				i,
				q->v[2*i  ],q->v[2*i+1]);
	printf("edges:\n");
	//for (int i = 0; i < MIN(20,q->num_edges); i++)
		//printf("[%6d] (%6d,%6d)\n",
				//i,
				//q->e[2*i  ],q->e[2*i+1]);
	printf("trigs:\n");
	for (int i = 0; i < MIN(20,q->num_trigs); i++)
		printf("[%6d] %7.2E (%6d,%6d,%6d) (%7.2E,%7.2E) (%7.2E,%7.2E) (%7.2E,%7.2E)\n",
				i,
				q->a[i    ],
				q->t[3*i  ],q->t[3*i+1],q->t[3*i+2],
				q->p[6*i  ],q->p[6*i+1],
				q->p[6*i+2],q->p[6*i+3],
				q->p[6*i+4],q->p[6*i+5]);

	printf("\n");
}
