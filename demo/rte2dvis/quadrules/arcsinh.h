#ifndef _ARCSINH_RULE_H_
#define _ARCSINH_RULE_H_
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
	
void arcsinh_rule_xy(double *xy, double *w, 
		double const *p0, double const *p,
		const int nu, double const *xu, double const *wu,
		const int nv, double const *xv, double const *wv,
		double *work); 

void arcsinh_rule_atomic_xy(double *xy, double *w, 
		double const *p0, double const *p1, double const *p2,
		const int nu, double const *xu, double const *wu,
		const int nv, double const *xv, double const *wv,
		double *work); 

void arcsinh_rule(double *x, double *y, double *w, 
		double const *p0, double const *p,
		const int nu, double const *xu, double const *wu,
		const int nv, double const *xv, double const *wv,
		double *work); 

void arcsinh_rule_atomic(double *x, double *y, double *w, 
		double const *p0, double const *p1, double const *p2,
		const int nu, double const *xu, double const *wu,
		const int nv, double const *xv, double const *wv,
		double *work); 

#ifdef __cplusplus
}
#endif 
/******************************************************************************/
#endif
