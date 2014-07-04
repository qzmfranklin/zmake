#ifndef _CLOSEST_PAIR_H_
#define _CLOSEST_PAIR_H_

#ifdef __cplusplus
extern "C" {
#endif


/*
 * non-recursive nlgn method. *a is altered. Extra memory constant
 */
double closest_pair(const int n, 
		double _Complex *restrict a,
		double _Complex *restrict pos);

/*
 * brute force n^2 method. *a is not altered. No extra memory
 */
double closest_pair0(const int n, 
		const double _Complex *restrict a,
		double _Complex *restrict pos);
	
#ifdef __cplusplus
}
#endif

#endif /* end of include guard */
