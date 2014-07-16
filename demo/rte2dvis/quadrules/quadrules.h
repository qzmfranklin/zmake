#ifndef _QUADRULES_H_
#define _QUADRULES_H_
/******************************************************************************/
/*
 * This header file is in the user space.
 */
/******************************************************************************/
#include "quadrules/arcsinh.h"
#include "quadrules/dunavant.h"
#include "quadrules/gauss.h"
#include "quadrules/wandzura.h"
/******************************************************************************/

/*
 *   REFERENCE_TO_PHYSICAL_T3 maps T3 reference points to physical points.
 *
 * Discussion:
 *
 *   Given the vertices of an order 3 physical triangle and a point
 *   (XSI,ETA) in the reference triangle, the routine computes the value
 *   of the corresponding image point (X,Y) in physical space.
 *
 *   Note that this routine may also be appropriate for an order 6
 *   triangle, if the mapping between reference and physical space
 *   is linear.  This implies, in particular, that the sides of the
 *   image triangle are straight and that the "midside" nodes in the
 *   physical triangle are literally halfway along the sides of
 *   the physical triangle.
 *
 * Reference Element T3:
 *
 *   |
 *   1  3
 *   |  |\
 *   |  | \
 *   S  |  \
 *   |  |   \
 *   |  |    \
 *   0  1-----2
 *   |
 *   +--0--R--1-->
 *
 * Licensing:
 *
 *   This code is distributed under the GNU LGPL license.
 *
 * Modified:
 *
 *   24 June 2005
 *
 * Author:
 *
 *   John Burkardt
 *
 * Parameters:
 *
 *   Input, double T[2*3], the coordinates of the vertices.
 *   The vertices are assumed to be the images of (0,0), (1,0) and
 *   (0,1) respectively.
 *
 *   Input, int N, the number of objects to transform.
 *
 *   Input, double REF[2*N], points in the reference triangle.
 *
 *   Output, double PHY[2*N], corresponding points in the
 *   physical triangle.
 *
 * Last Modified: 2014-03-05T10:52:07 by
 * 	Zhongming Qu
 */
void reference_to_physical_t3 ( const int n, const double *restrict t,
		const double *restrict ref, double *restrict phy ) 
{
	for ( int i = 0; i < 2; i++ ) {
		for ( int j = 0; j < n; j++ ) {
			phy[i+j*2] = t[i+0*2] * ( 1.0 - ref[0+j*2] - ref[1+j*2] )
				   + t[i+1*2] *       + ref[0+j*2]
				   + t[i+2*2] *                    + ref[1+j*2];
		}
	} 
	return;
}
#endif /* end of include guard */
