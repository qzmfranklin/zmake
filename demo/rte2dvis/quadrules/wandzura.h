#ifndef _WANDZURA_H_
#define _WANDZURA_H_
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif 
int wandzura_degree ( int rule );
int wandzura_order_num ( int rule );
void wandzura_rule ( int rule, int order_num, double xy[], double w[] );
int wandzura_rule_num ( void );
#ifdef __cplusplus
}
#endif
/******************************************************************************/
#endif /* end of include guard */
