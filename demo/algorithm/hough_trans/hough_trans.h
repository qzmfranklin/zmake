#ifndef _HOUGH_TRANS_H_
#define _HOUGH_TRANS_H_
#ifdef __cplusplus
extern "C" {
#endif

/*
 * 			Hough Transformation
 * The Hough transformation converts the problem of finding collinear points to
 * the problem of finding concurrent curves. It can pick out collinear points as
 * well as human eyes can do.
 *
 * Below is a C-style implementation of 2D Hough transformation. Hough
 * transformation in higher dimensions requires the discretization of
 * high-dimensional phase space. This leads to computationally unacceptable
 * costs in CPU time and memory.
 *
 * Any attempt to do 3D Hough transformation must be divided into sub-problems
 * that only deal with 2D Hough transformations one at a time.
 *
 * 			Usage Model
 * Input:
 * 	The raw data
 * 		P_i(x_i,y_i)		i=1,2,...,N
 * 	is passed as an array of double precision floating point number pairs,
 * 	i.e., xy[2*N], where
 * 		xy[2*i]   = x_i
 * 		xy[2*i+1] = y_i
 * 	The number of points, N, is also passed as a separate variable.
 *
 * Output:
 * 	TODO
 * 	
 *
 * 	NOTE: Once the array xy[2*N] is passed to ht_create, the user MUST NOT
 * 	alter its content until having called ht_destroy. The array is kept
 * 	intact.
 */
struct st_hough_trans {
	int status; // internal status
	int num; // number of data points
	double *xy; // data points
};

/*
 * 		r-phi phase space
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
 * Once the *xy is passed to ht_create, the user MUST NOT alter the content of
 * *xy until ht_destroy is called.
 */
struct st_hough_trans *ht_create(const int n, const double *xy);

void ht_refine_box(const struct st_hough_trans *h,
		const struct st_ht_phase_box *restrict in,
		const double delta_r, const double delta_phi
		struct st_ht_phase_box *restrict out);

void ht_refine_box_with_shift(const struct st_hough_trans *h,
		const struct st_ht_phase_box *restrict in,
		const double delta_r, const double delta_phi
		struct st_ht_phase_box *restrict out);

void ht_find_points_for_box(const struct st_hough_trans *h,
		const struct st_ht_phase_box *box, char *mask);

void ht_print_info(const struct st_hough_trans *h);

void ht_destroy(const struct st_hough_trans *h);

#ifdef __cplusplus
}
#endif
#endif /* end of include guard _HOUGH_TRANS_H_ */
