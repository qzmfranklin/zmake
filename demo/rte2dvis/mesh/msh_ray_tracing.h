#ifndef _MSH_RAY_TRACING_H_
#define _MSH_RAY_TRACING_H_
/******************************************************************************/
#include <vector>
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Each square is devided into four sub-squares:
 *		%2=1 %2=0
 * 	>2	| 3 | 4 |
 * 	<3	| 1 | 2 |
 */

struct st_meshnode {
	double x1,y1,x2,y2;	// boundary of the square:
	int n;			// number of triangles
	union {
		double *a;	// pointer to a C-style array
		std::vector<double> *v; // C++ STD container
	} trigs;
};

struct st_meshtree {
	struct st_meshnode *root; // pointer to the root of packed levels
	double *ptt;		// packed tree of triangles
	int status;		// 0=Uninit'd 1=Init'd <0=ERROR
	int npl;		// number of packed levels, starts from 0
	int ntot;		// total number of nodes, including leaves
};

/*
 * Triangles:
 * 	trigs		x1,y1,x2,y2,x3,y3
 * Initial Square:
 * 	limits[0]	xmin
 * 	limits[1]	ymin
 * 	limits[2]	xmax
 * 	limits[3]	ymax
 * Return value:
 * 	a pointer to the st_meshtree
 * Ownership:
 * 	the callee owns the st_meshtree object
 */
struct st_meshtree *create_meshtree(const int num_trigs, 
		const double *restrict trigs, const double *restrict limits,
		const int npl);
void destroy_meshtree(struct st_meshtree *t);


/*
 * TODO
 * Line parameters:  y = k x + b
 * 	ln[0]	k  slope
 * 	ln[1]	b  y-intercept
 * Ray parameters:   (x-x0)*cos(phi) + (y-y0)*sin(phi) = 0
 * 	p0[0]	x0
 * 	p0[1]	y0
 * Note:
 * 	ln[0] (slope) must not be inifity or 0.
 */ 
double meshtree_line_seg(struct st_mesh *restrict t, double ln[2]);
double meshtree_ray_seg(const struct st_mesh *restrict t, 
		const double p0[2], const double phi);
	
#ifdef __cplusplus
}
#endif
/******************************************************************************/
#endif
