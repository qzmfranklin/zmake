#ifndef _HOUGH_TRANS_H_
#define _HOUGH_TRANS_H_
#ifdef __cplusplus
extern "C" {
#endif

/*
 * see README.md for documentation
 */

struct st_ht_tmp;
struct st_hough_trans {
	int status; // internal status
	int num; // number of data points
	double *xy; // data points
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
	int     numr; // number of grids in r
	int     nump; // number of grids in phi
	double  delr; // delta r
	double  delp; // delta phi
	double *aryr; // grid in r
	double *aryp; // grid in phi
	int    *grid; // count in grid
};

/*
 * Once the *xy is passed to ht_create, the user MUST NOT alter the content of
 * *xy until ht_destroy is called.
 */
struct st_hough_trans *ht_create(const int n, const double *xy);

/*
 * Allocates new h->tmp after releasing old one (if alloc'd)
 */
void ht_refine_box(const struct st_hough_trans *h,
		const struct st_ht_phase_box *restrict in,
		const double delta_r, const double delta_phi
		struct st_ht_phase_box *restrict out);

void ht_refine_box_with_shift(const struct st_hough_trans *h,
		const struct st_ht_phase_box *restrict in,
		const double delta_r, const double delta_phi
		struct st_ht_phase_box *restrict out);

/*
 * Find the mask of points that correspond to a given box. Assumes that the *mask
 * is long enough but does not check that.
 */
void ht_find_points_for_box(const struct st_hough_trans *h,
		const struct st_ht_phase_box *box, char *mask);

void ht_print_info(const struct st_hough_trans *h);

void ht_destroy(const struct st_hough_trans *h);

#ifdef __cplusplus
}
#endif
#endif /* end of include guard _HOUGH_TRANS_H_ */
