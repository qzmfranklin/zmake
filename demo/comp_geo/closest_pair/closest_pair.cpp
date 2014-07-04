#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <float.h>
#include "comp_geo/geo_utils.h"

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

/*
 * Brute force O(n^2) method.
 * Returns the mininal distance. **pos points to two double _Complex addresses
 */
static double brute_force2(const int n, double _Complex *a, double _Complex **pos)
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

static double brute_force(const int n, double _Complex *a, double _Complex **pos)
{
	return sqrt(brute_force2(n,a,pos));
}


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
 * Runtime extra memory: 32*n bytes
 */
static const int B=6; // blocksize
static double nlgn_method2(const int n, double _Complex *a, double _Complex**pos)
{
	double _Complex *X=(double _Complex*)malloc(sizeof(double _Complex)*n);
	double _Complex *Y=(double _Complex*)malloc(sizeof(double _Complex)*n);
	assert(X);
	assert(Y);
	memcpy(X,a,sizeof(double _Complex)*n);
	memcpy(Y,a,sizeof(double _Complex)*n);
	qsort(X,n,sizeof(double _Complex),cmpx);
	qsort(Y,n,sizeof(double _Complex),cmpy);

	const int num_blk = (n%B==0)?(n/B):(n/B+1);

	double *dmin_list=(double*)malloc(sizeof(double)*num_blk);
	assert(dmin_list);

	double _Complex **pair_list=(double _Complex**)malloc(sizeof(void*)*num_blk*2);
	assert(pair_list);
	memset(pair_list,0,sizeof(void*)*num_blk*2);

	int *blk_size = (int*)malloc(sizeof(int)*num_blk);
	assert(blk_size);
	for (int i = 0; i < num_blk-1; i++)
		blk_size[i] = B;
	blk_size[num_blk-1] = n - num_blk*B;

	for (int i = 0; i < num_blk-1; i++)
		dmin_list[i] = brute_force2(B,X+i*B,pair_list+2*i);
	dmin_list[num_blk-1] = brute_force2(blk_size[num_blk-1],
			X+(num_blk-1)*B,pair_list+2*(num_blk-1));
}
/*
 * O(nlgn) method: implementing the pseudocode from Wikipedia:
 *
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
 *
 * The above method is recursive. Let's do a non-recursive, i.e., iterative
 * version here. Also, try not to allocate memory as long as we do not have to.
 */

int main(int argc, char const* argv[])
{
	size_t N;
	double _Complex *data;
	read_ascii(stdin,&N,&data);

	double _Complex *pos[2];
	double dmin = nlgn_method2(N,data,pos);

	free(data);
	return 0;
}
