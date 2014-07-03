#ifndef _GEO_UTILS_H_
#define _GEO_UTILS_H_

#include <math.h>
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
 * Apple changed the default <complex.h> to C++ complex template class. Redefine
 * the most useful ones to maintain C style codes. Bitwise compatible.
 */
#ifdef __clang__
#define creal(c) __real__(c)
#define cimag(c) __imag__(c)
#define carg(c) atan2(__imag__(c),__real__(c))
#endif

#ifdef __cplusplus
extern "C" {
#endif

void print_array(const int n, const double _Complex *restrict a);

void read_binary(FILE *restrict fd, size_t *restrict n, double _Complex *restrict *a);

void read_ascii(FILE *restrict fd, size_t *restrict n, double _Complex *restrict *a);

void output_binary(FILE *fd, const size_t n, const double _Complex *restrict a);

void output_ascii(FILE *fd, const size_t n, const double _Complex *restrict a);

static inline double norm2(const double _Complex c)
{
	return creal(c)*creal(c) + cimag(c)*cimag(c);
}

static inline double norm(const double _Complex c)
{
	return sqrt(norm2(c));
}


#ifdef __cplusplus
}
#endif
#endif /* end of include guard _GEO_UTILS_H_ */
