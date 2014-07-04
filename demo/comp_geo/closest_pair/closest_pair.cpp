#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <float.h>
#include "comp_geo/geo_utils.h"

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

/*
 * Brute force O(n^2) method.
 * Returns the mininal distance _SQAURED_. 
 * **pos points to two double _Complex addresses.
 */
static double brute_force2(const int n, 
		double _Complex *restrict a,
		double _Complex *restrict pos)
{
	assert(n>1);
	double d_min = DBL_MAX;
	double _Complex pts[2];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < i; j++) {
			const double d_curr = norm2(a[j]-a[i]);
			if (d_curr<d_min) {
				d_min  = d_curr;
				pts[0] = a[i];
				pts[1] = a[j];
			}
		}
	pos[0] = pts[0];
	pos[1] = pts[1];
	return d_min;
}

static double brute_force(const int n, 
		double _Complex *restrict a,
		double _Complex *restrict pos)
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

static void merge(double *restrict d_min, double _Complex *restrict pair_min,
		const double _Complex *restrict a, const int n1, const int n2,
		int *restrict work_size, double _Complex *restrict *work)
{
	const double x0 = 0.5*( a[n1-1] + a[n1] );
	const int     n = n1 + n2;

	// Determine y_size, realloc to enlarge *work if needed
	int start=-1;
	while ( creal(a[++start]) < x0-*d_min );
	int end=start-1;
	while ( ++end<n && creal(a[end]) <= x0+*d_min );
	const int y_size = end - start;
	if (*work_size<y_size) {
		*work_size = y_size;
		free(*work);
		*work = (double _Complex*)
			malloc(sizeof(double _Complex)*y_size);
	}

	double _Complex *y = *work;
	for (int i = 0; i < y_size; i++)
		y[i] = a[i+start];
	qsort(y,y_size,sizeof(double _Complex),cmpy);

	for (int i = 0; i < y_size; i++)
		for (int j = i; j < i+5; j++) {
			const double d_curr = norm2(a[j]-a[i]);
			if (d_curr<*d_min) {
				*d_min = d_curr;
				pair_min[0] = a[i];
				pair_min[1] = a[j];
			}
		}
}

/*
 * O(nlgn) method: *a is sorted in-place. Extra memory >= 3*16*sqrt(n) bytes
 *
 * pseudocode from Wikipedia:
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
static double nlgn_method2(const int n, 
		double _Complex *restrict a,
		double _Complex *restrict pos)
{
	qsort(a,n,sizeof(double _Complex),cmpx);

	const int B=6;
	int num_blk = (n%B==0)?(n/B):(n/B+1);

	int *blk_size = (int*)malloc(sizeof(int)*num_blk);
	assert(blk_size);
	for (int i = 0; i < num_blk-1; i++)
		blk_size[i] = B;
	blk_size[num_blk-1] = n - num_blk*B;

	double d_min = DBL_MAX;
	double _Complex pair_min[2];
	for (int i = 0; i < num_blk; i++) {
		double _Complex pair_curr[2];
		const double d_curr = brute_force2(
				blk_size[i],a+i*B,pair_curr);
		if (d_curr<d_min) {
			d_min = d_curr;
			pair_min[0] = pair_curr[0];
			pair_min[1] = pair_curr[1];
		}
	}

	// workspace
	int work_size = 2*sqrt(n);
	double _Complex *work = (double _Complex*)
		malloc(sizeof(double _Complex)*work_size);

	while(num_blk>1) {
		int j=0;
		while (j<num_blk-1) {
			// merge j and j+1 to k
			static int offset=0;
			static int k=0;
			merge(  &d_min,pair_min,
				a+offset,blk_size[j],blk_size[j+1],
				&work_size,&work);
			blk_size[k] = blk_size[j] + blk_size[j+1];
			offset += blk_size[k];
			k++;
			j += 2;
		}
		num_blk -= j/2;
	}

	free(blk_size);
	free(work);

	return d_min;
}


int main(int argc, char const* argv[])
{
	size_t N;
	double _Complex *data;
	read_ascii(stdin,&N,&data);

	double _Complex pos[2];
	double dmin = nlgn_method2(N,data,pos);
	free(data);

	print_array(2,pos);
	printf("d_min = %7.2f\n",dmin);

	return 0;
}
