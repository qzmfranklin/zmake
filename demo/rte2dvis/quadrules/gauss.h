#ifndef _GAUSS_H_
#define _GAUSS_H_
/******************************************************************************/
#include "quadrules_internal.h"
#include <string.h>
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
void cgqf ( int nt, int kind, double alpha, double beta, double a, double b, 
  double t[], double wts[] );
#ifdef __cplusplus
}
#endif
/******************************************************************************/
#endif /* end of include guard */
