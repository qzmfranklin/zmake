#include <stdio.h>
#include <assert.h>
#include <float.h>
#include "comp_geo/geo_utils.h"


/*
 * recommended usage:
 * 	read_ascii(fd,&N,&data);
 */
//qsort(data,num,sizeof(int),compare);

static int cmpx (const void * a, const void * b) 
{ 
	return ( creal(*(double _Complex*)a) - creal(*(double _Complex*)b) ); 
}
static int cmpy (const void * a, const void * b) 
{ 
	return ( cimag(*(double _Complex*)a) - cimag(*(double _Complex*)b) ); 
}

/*
 * Implementation of the pseudocode from Wikipedia
 * closestPair of (xP, yP)
 * where xP is P(1) .. P(N) sorted by x coordinate, and
 *       yP is P(1) .. P(N) sorted by y coordinate (ascending order)
 * 	if N ≤ 3 then
 * 	  return closest points of xP using brute-force algorithm
 * 	else
 * 	  xL ← points of xP from 1 to ⌈N/2⌉
 * 	  xR ← points of xP from ⌈N/2⌉+1 to N
 * 	  xm ← xP(⌈N/2⌉)x
 * 	  yL ← { p ∈ yP : px ≤ xm }
 * 	  yR ← { p ∈ yP : px > xm }
 * 	  (dL, pairL) ← closestPair of (xL, yL)
 * 	  (dR, pairR) ← closestPair of (xR, yR)
 * 	  (dmin, pairMin) ← (dR, pairR)
 * 	  if dL < dR then
 * 	    (dmin, pairMin) ← (dL, pairL)
 * 	  endif
 * 	  yS ← { p ∈ yP : |xm - px| < dmin }
 * 	  nS ← number of points in yS
 * 	  (closest, closestPair) ← (dmin, pairMin)
 * 	  for i from 1 to nS - 1
 * 	    k ← i + 1
 * 	    while k ≤ nS and yS(k)y - yS(i)y < dmin
 * 	      if |yS(k) - yS(i)| < closest then
 * 	        (closest, closestPair) ← (|yS(k) - yS(i)|, {yS(k), yS(i)})
 * 	      endif
 * 	      k ← k + 1
 * 	    endwhile
 * 	  endfor
 * 	  return closest, closestPair
 * 	endif
 */

/*
 * Brute force O(n^2) method.
 * Returns the mininal distance. **pos points to two double _Complex addresses
 */
static double brute_force(const int n, double _Complex *a, double _Complex **pos)
{
	assert(n>1);
	double d_min = DBL_MAX;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < i; j++) {
			const double d_curr = norm2(a[j]-a[i]);
			if (d_curr<d_min) {
				d_min  = d_curr;
				pos[0] = a+i;
				pos[1] = a+j;
			}
		}
	return d_min;
}

int main(int argc, char const* argv[])
{
	size_t N;
	double _Complex *data;
	read_ascii(stdin,&N,&data);



	free(data);
	return 0;
}
