#ifndef _CLOSEST_PAIR_H_
#define _CLOSEST_PAIR_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Input:
 * 	n	number of points
 * 	*a	double precision (x,y) pairs stored in a sequence
 * Output:
 * 	return	the mininal distance
 * 	*pos	upon returning, stores the desired pair of points
 * Coder's note:
 * 	The usual divide&conquer strategy is recursive. Naive implementations
 * 	found through googling use too much extra memory and runtime. This
 * 	implementation is non-recursive and only uses O(sqrt(n)) extra memory.
 */

/*
 * Non-recursive nlgn method. *a is altered. Extra memory O(sqrt(n)).
 * You should use this one.
 */
double closest_pair(const int n, void *restrict a, void *restrict pos);

/*
 * Brute force n^2 method. *a is not altered. No extra memory.
 * You should _NOT_ use this one. This is only for debugging.
 */
double closest_pair0(const int n, const void *restrict a, void *restrict pos);
	
#ifdef __cplusplus
}
#endif

#endif /* end of include guard */
