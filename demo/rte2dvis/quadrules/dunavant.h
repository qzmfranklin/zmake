#ifndef _DUNAVANT_H_
#define _DUNAVANT_H_
/******************************************************************************/
#include "quadrules_internal.h"
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
int dunavant_degree ( int rule );
int dunavant_order_num ( int rule );
void dunavant_rule ( int rule, int order_num, double xy[], double w[] );
int dunavant_rule_num ( );
#endif
#ifdef __cplusplus
}
#endif
/******************************************************************************/
#endif /* end of include guard */ 
