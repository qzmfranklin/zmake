#ifndef _HOUGH_TRANS_H_
#define _HOUGH_TRANS_H_
#ifdef __cplusplus
extern "C" {
#endif

/*
 * see README.md for more documentation
 */

struct st_ht_tmp;
struct st_hough_trans {
	int status; // internal status
	int num; // number of data points
	double *x;
	double *y;
	struct st_ht_tmp *tmp; // internal workspace
};

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
struct st_ht_phase_box {
	double p1;
	double p2;
	double r1;
	double r2;
};

/*
 * internal workspace, do not use me
 */
struct st_ht_tmp {
	int     nump; // number of grids in phi
	int     numr; // number of grids in r
	double  delp; // delta phi
	double  delr; // delta r
	double *aryp; // [nump+1] grid in phi
	double *aryr; // [numr+1] grid in r
	double *rphi; // [nump+1] workspace for holding r(phi) list
	int    *rank; // [nump+1] rank in r
	int    *grid; // [nump*numr] count in grid, phi-major, grid[i,j]=grid[j+i*numr]
};

/*
 * Once the *xy is passed to ht_create, the user MUST NOT alter the content of
 * *xy until ht_destroy is called.
 */
struct st_hough_trans *ht_create(const int n, const double *restrict x,
		const double *restrict y);

void ht_alloc_buffer(struct st_hough_trans *h,
		const struct st_ht_phase_box *restrict in,
		const double delta_r, const double delta_phi);

/*
 * Allocates new h->tmp after releasing old one (if alloc'd)
 * Return: 
 * 	0=success 
 * 	1=fail: too close to boundary, but still returns the proper index
 *
 * Output: *index, upon exit,
 * 	index[0] = peak p1 index in aryp
 * 	index[1] = peak r1 index in aryr
 */
int ht_find_peak_in_rphi_grid(struct st_hough_trans *h,
		const struct st_ht_phase_box *restrict in,
		const double delta_r, const double delta_phi,
		int *restrict index);

/*
 * Find the coordinates of the box corresponding to the given index
 */
void ht_find_box_coord(const struct st_hough_trans *h,
		const int *index, struct st_ht_phase_box *box);

/*
 * Same as ht_find_box_coord, but shift the box by the weight(count)
 */
void ht_find_box_coord_with_shift(const struct st_hough_trans *h,
		const int *index, struct st_ht_phase_box *box);

/*
 * Find the mask of points that correspond to a given box. Assumes that the *mask
 * is long enough but does not check that.
 */
void ht_find_points_for_box(const struct st_hough_trans *h,
		const struct st_ht_phase_box *box, char *mask);

void ht_free_buffer(const struct st_hough_trans *h);

void ht_print_info(const struct st_hough_trans *h);

void ht_destroy(const struct st_hough_trans *h);

#ifdef __cplusplus
}
#endif
#endif /* end of include guard _HOUGH_TRANS_H_ */
