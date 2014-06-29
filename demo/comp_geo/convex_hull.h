#ifndef _CONVEX_HULL_H_
#define _CONVEX_HULL_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <complex.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 	In-place Graham scan to find the convex hull.
 * Input:
 * 	size       	number of points
 * 	creal(data[i])	x of the i-th point
 * 	cimag(data[i])	y of the i-th point
 * NOTE: 
 * 	*data and *size are altered.
 * Return:
 * 	*data 		the set of points that form the convex hull in the 
 * 			counter-clockwise order
 * 	*size 		the number of points in *data
 * Usage:
 * 	See graham_scan.cpp
 */
void graham_scan(size_t *restrict size, double _Complex *restrict *data);

#ifdef __cplusplus
}
#endif
#endif /* end of include guard _CONVEX_HULL_H_ */
