#include <hough_trans.h>
#include <assert.h>
#include <math.h>

//struct st_ht_tmp;
//struct st_hough_trans {
	//int status; // internal status
	//int num; // number of data points
	//double *xy; // data points
	//struct st_ht_tmp *tmp; // internal workspace
//};

/*
 * 	r-phi phase space box
 *
 *   r  ^
 *      |
 *      |
 *      |
 *   r2 |----------------
 *      |		|
 *      |		|
 *      |		|
 *      |		|
 *      |		|
 *   r1 -----------------------> phi
 *     phi1            phi2
 *
 */
//struct st_ht_phase_box {
	//double p1;
	//double p2;
	//double r1;
	//double r2;
//};


/*
 * Once the *xy is passed to ht_create, the user MUST NOT alter the content of
 * *xy until ht_destroy is called.
 */
struct st_hough_trans *ht_create(const int n, const double *xy)
{
	struct st_hough_trans *h = (struct st_hough_trans*)
		malloc(sizeof(struct st_hough_trans));
	assert(h);
	h->num    = n;
	h->xy     = xy;
	h->status = 0;
	return h;
}

/*
 * Some people might feel uneasy about the possible frequent alloc and free of
 * memories. Their concerns are definitely valid. But here goes the reasons for
 * doing it:
 * 	1. The overall amount of time spent on alloc&free is very small.
 * 	2. No more than a few MB of memory is involved.
 * 	3. The operating system should be smart enough to handle it efficiently.
 */
static void alloc_tmp(struct st_hough_trans *h,
		const struct st_ht_phase_box *box,
		const double delta_r, const double delta_phi)
{
	assert(h->status==0);

	h->tmp = (struct st_ht_tmp*) malloc(sizeof(struct st_ht_tmp));
	assert(h->tmp);

	const int numr = ceil( (box->r2 - box->r1) / delta_r   ) + 1E-6;
	const int nump = ceil( (box->r2 - box->r1) / delta_phi ) + 1E-6;

	struct st_ht_tmp *tmp = h->tmp;
 
	tmp->delr = delta_r;
	tmp->delp = delta_phi;
	tmp->numr = numr;
	tmp->nump = nump;
	tmp->aryr = (double*)malloc( sizeof(double) * (numr + 1) );
	tmp->aryp = (double*)malloc( sizeof(double) * (nump + 1) );
	tmp->rphi = (double*)malloc( sizeof(double) * (nump + 1) );
	tmp->rank = (int*)   malloc( sizeof(int)    * (nump + 1) );
	tmp->grid = (int*)   malloc( sizeof(int)    * numr * nump );
	assert(tmp->aryr);
	assert(tmp->aryp);
	assert(tmp->rphi);
	assert(tmp->rank);
	assert(tmp->grid);

	h->status=1;
}

// init fields in h->tmp that are not init'd in alloc_tmp, except h->tmp->rphi
static void init_tmp(struct st_hough_trans *h,
		const struct st_ht_phase_box *box,
		const double delta_r, const double delta_phi)
{
	assert(h->status==1);

	struct st_ht_tmp *tmp = h->tmp;

	// avoid RAW dependency to enable vectorization
	for (int i = 0; i < numr; i++)
		tmp->aryr[i] += box->r1 + i * delta_r;
	tmp->aryr[numr] = box->r2;

	for (int i = 0; i < nump; i++)
		tmp->aryp[i] += box->p1 + i * delta_phi;
	tmp->aryp[nump] = box->p2;

	h->status=2;
}

static void free_tmp(const struct st_hough_trans *h)
{
	assert(h->status==2);

	free(h->tmp->aryr);
	free(h->tmp->aryp);
	free(h->tmp->rphi);
	free(h->tmp->rank);
	free(h->tmp->grid);
	free(h->tmp);

	h->status=0;
}

/*
 *struct st_ht_tmp {
 *        int     numr; // number of grids in r
 *        int     nump; // number of grids in phi
 *        double  delr; // delta r
 *        double  delp; // delta phi
 *        double *aryr; // [numr+1] grid in r
 *        double *aryp; // [nump+1] grid in phi
 *        double *rphi; // [nump+1] workspace for holding r(phi) list
 *        int    *rank; // [nump+1] rank in r
 *        int    *grid; // [nump*numr] count in grid, phi-major, grid[i,j]=grid[j+i*numr]
 *};
 */
static inline double compute_rphi_from_point_and_phi(const double *xy, const double phi)
{
	return xy[0] * cos(phi) + xy[1] * sin(phi);
}

static int determine_rank(const int numr, const double *rlist, const double rval)
{
	int k=-1;
	while (k<numr)
		if (rval >= rlist[k+1])
			k++;
	return k;
}

static int max(const int a, const int b) { return (a>b)?a:b; }
static int min(const int a, const int b) { return (a<b)?a:b; }
static void add_point_to_grid_by_index(struct st_hough_trans *h, const int index)
{
	// TODO: eliminate some aliasing?
	const int numr = h->tmp->numr;
	const int nump = h->tmp->nump;
	const double *aryr = h->tmp->aryr;
	const double *aryp = h->tmp->aryp;
	double *rphi = h->tmp->rphi;
	int    *rank = h->tmp->rank;
	int    *grid = h->tmp->grid;

	for (int i = 0; i < nump + 1; i++)
		rphi[i] = compute_r_from_point_and_phi(h->xy+2*index,aryp[i]);

	memset(tmp->rank,0,sizeof(int) * (nump + 1) );
	for (int i = 0; i < nump + 1; i++)
		rank[i] = determine_rank(numr,aryr,rphi[i]);

	memset(tmp->grid,0,sizeof(int) * numr * nump);
	for (int i = 0; i < nump; i++)
		for (int j = max(0,rank[i]); j <= min(numr-1,rank[i]) ; j++)
			grid[j+i*numr]++;
}


// 0=succuss, 1=fail on boundary
static int find_max(const int m, const int n, const int *restrict a,
		int *restrict pos)
{
	int ii,jj;
	int curr_max = -1;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++) {
			const double val = a[j+i*n];
			if (val > curr_max) {
				curr_max = val;
				ii = i;
				jj = j;
			}
		}
	pos[0] = ii;
	pos[1] = jj;
	return ii==0 || ii==m || jj==0 || jj==n ;
}

int ht_refine_box(struct st_hough_trans *h,
		const struct st_ht_phase_box *restrict in,
		const double delta_r, const double delta_phi
		struct st_ht_phase_box *restrict out)
{
	if (h->status == 1) free_tmp(h);
	alloc_tmp(h,in,delta_r,delta_phi);
	init_tmp(h,in,delta_r,delta_phi);

	for (int i = 0; i < h->num; i++)
		add_point_to_grid_by_index(h,i);

	int pos[2];
	int res = find_max(h->tmp->nump,t->tmp->numr,h->tmp->grid,pos);
	if (res)
		fprintf(stderr,"ht_refine_box: boundary too close\n");

	free_tmp(h);

	return res;
}

void ht_refine_box_with_shift(const struct st_hough_trans *h,
		const struct st_ht_phase_box *restrict in,
		const double delta_r, const double delta_phi
		struct st_ht_phase_box *restrict out)
{
}

/*
 * Find the mask of points that correspond to a given box. Assumes that the *mask
 * is long enough but does not check that.
 */
void ht_find_points_for_box(const struct st_hough_trans *h,
		const struct st_ht_phase_box *box, char *mask);

void ht_print_info(const struct st_hough_trans *h);

void ht_destroy(const struct st_hough_trans *h);

