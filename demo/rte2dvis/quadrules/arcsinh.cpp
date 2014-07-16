#include "arcsinh.h"
#include <math.h>
/******************************************************************************/
static double det(const double *a1, const double *a2, 
		const double *b1, const double *b2)
{
	return (a1[0]-a2[0])*(b1[1]-b2[1]) - (a1[1]-a2[1])*(b1[0]-b2[0]);
}


void arcsinh_rule_xy(double *xy, double *w, 
		double const *p0, double const *p,
		const int nu, double const *xu, double const *wu,
		const int nv, double const *xv, double const *wv,
		double *work)
{
	const int N=nu*nv;

	arcsinh_rule_atomic_xy(xy    ,w    ,p0,p  ,p+2,nu,xu,wu,nv,xv,wv,work);
	arcsinh_rule_atomic_xy(xy+2*N,w+N  ,p0,p+2,p+4,nu,xu,wu,nv,xv,wv,work);
	arcsinh_rule_atomic_xy(xy+4*N,w+2*N,p0,p+4,p  ,nu,xu,wu,nv,xv,wv,work);

	double tmp[4];
	tmp[0] = det(p+2,p ,p+4,p  );		// A
	tmp[1] = det(p,  p0,p+2,p  )*tmp[0];	// s1: p0 p1 p2
	tmp[2] = det(p+2,p0,p+4,p+2)*tmp[0];	// s2: p0 p2 p3
	tmp[3] = det(p+4,p0,p,  p+4)*tmp[0];	// s3: p0 p3 p1

	for (int j = 0; j < 3; j++)
		if (tmp[j+1]<0.0)
			for (int i = 0; i < N; i++)
				w[j*N+i] *= -1.0;
} 

void arcsinh_rule_atomic_xy(double *xy, double *w, 
		double const *p0, double const *p1, double const *p2,
		const int nu, double const *xu, double const *wu,
		const int nv, double const *xv, double const *wv,
		double *work)
{
	double *u = work;

	double p0p1[2],p0p2[2],p1p2[2],invp1p2,Ah[4],xp[2],h,u0[2];
	p0p1[0] = p1[0] - p0[0]; // x1-x0
	p0p1[1] = p1[1] - p0[1]; // y1-y0
	p0p2[0] = p2[0] - p0[0]; // x2-x0
	p0p2[1] = p2[1] - p0[1]; // y2-y0
	p1p2[0]	= p2[0] - p1[0]; // x2-x1
	p1p2[1]	= p2[1] - p1[1]; // y2-y1
	invp1p2 = 1.0/sqrt(p1p2[0]*p1p2[0] + p1p2[1]*p1p2[1] ); // inverse p1p2
	p1p2[0]*= invp1p2;
	p1p2[1]*= invp1p2;
	h	= -p1p2[0]*p0[1] + invp1p2*(p0p2[0]*p1[1] - p0p1[0]*p2[1]);
	Ah[0]	= p1p2[0]*h;
	Ah[1]	= -p1p2[1]*h;
	Ah[2]	= p1p2[1]*h;
	Ah[3]	= p1p2[0]*h;
	xp[0]	= p1p2[0]*p0p1[0] + p1p2[1]*p0p1[1]; // x1p
	xp[1]	= p1p2[0]*p0p2[0] + p1p2[1]*p0p2[1]; // x2p
	u0[0]	= asinh(xp[0]/h); // u1
	u0[1]	= asinh(xp[1]/h)-u0[0]; // u2-u1

	for (int i = 0; i < nu; i++)
		u[i] = u0[0] + u0[1]*xu[i]; 

	for (int j = 0; j < nv; j++)
		for (int i = 0; i < nu; i++)
			// h(u1-u2) is absorbed into weights.
			w[i+nu*j] = fabs(h*u0[1])*wu[i]*wv[j]; 

	for (int j = 0; j < nv; j++)
		for (int i = 0; i < nu; i++) { 
			xy[2*(i+nu*j)  ] = p0[0] + xv[j]*( Ah[0]*sinh(u[i]) + Ah[1] );
			xy[2*(i+nu*j)+1] = p0[1] + xv[j]*( Ah[2]*sinh(u[i]) + Ah[3] );
		} 
}
/******************************************************************************/

void arcsinh_rule(double *x, double *y, double *w, 
		double const *p0, double const *p,
		const int nu, double const *xu, double const *wu,
		const int nv, double const *xv, double const *wv,
		double *work)
{
	const int N=nu*nv;

	arcsinh_rule_atomic(x    ,y    ,w    ,p0,p  ,p+2,nu,xu,wu,nv,xv,wv,work);
	arcsinh_rule_atomic(x+N  ,y+N  ,w+N  ,p0,p+2,p+4,nu,xu,wu,nv,xv,wv,work);
	arcsinh_rule_atomic(x+2*N,y+2*N,w+2*N,p0,p+4,p  ,nu,xu,wu,nv,xv,wv,work);

	double tmp[4];
	tmp[0] = det(p+2,p ,p+4,p  );		// A
	tmp[1] = det(p,  p0,p+2,p  )*tmp[0];	// s1: p0 p1 p2
	tmp[2] = det(p+2,p0,p+4,p+2)*tmp[0];	// s2: p0 p2 p3
	tmp[3] = det(p+4,p0,p,  p+4)*tmp[0];	// s3: p0 p3 p1

	for (int j = 0; j < 3; j++)
		if (tmp[j+1]<0.0)
			for (int i = 0; i < N; i++)
				w[j*N+i] *= -1.0;
} 

void arcsinh_rule_atomic(double *x, double *y, double *w, 
		double const *p0, double const *p1, double const *p2,
		const int nu, double const *xu, double const *wu,
		const int nv, double const *xv, double const *wv,
		double *work)
{
	double *u = work;

	double p0p1[2],p0p2[2],p1p2[2],invp1p2,Ah[4],xp[2],h,u0[2];
	p0p1[0] = p1[0] - p0[0]; // x1-x0
	p0p1[1] = p1[1] - p0[1]; // y1-y0
	p0p2[0] = p2[0] - p0[0]; // x2-x0
	p0p2[1] = p2[1] - p0[1]; // y2-y0
	p1p2[0]	= p2[0] - p1[0]; // x2-x1
	p1p2[1]	= p2[1] - p1[1]; // y2-y1
	invp1p2 = 1.0/sqrt(p1p2[0]*p1p2[0] + p1p2[1]*p1p2[1] ); // inverse p1p2
	p1p2[0]*= invp1p2;
	p1p2[1]*= invp1p2;
	h	= -p1p2[0]*p0[1] + invp1p2*(p0p2[0]*p1[1] - p0p1[0]*p2[1]);
	Ah[0]	= p1p2[0]*h;
	Ah[1]	= -p1p2[1]*h;
	Ah[2]	= p1p2[1]*h;
	Ah[3]	= p1p2[0]*h;
	xp[0]	= p1p2[0]*p0p1[0] + p1p2[1]*p0p1[1]; // x1p
	xp[1]	= p1p2[0]*p0p2[0] + p1p2[1]*p0p2[1]; // x2p
	u0[0]	= asinh(xp[0]/h); // u1
	u0[1]	= asinh(xp[1]/h)-u0[0]; // u2-u1

	for (int i = 0; i < nu; i++)
		u[i] = u0[0] + u0[1]*xu[i]; 

	for (int j = 0; j < nv; j++)
		for (int i = 0; i < nu; i++)
			//// h(u1-u2) is absorbed into weights.
			w[i+nu*j] = fabs(h*u0[1])*wu[i]*wv[j]; 

	for (int j = 0; j < nv; j++)
		for (int i = 0; i < nu; i++) { 
			x[i+nu*j] = p0[0] + xv[j]*( Ah[0]*sinh(u[i]) + Ah[1] );
			y[i+nu*j] = p0[1] + xv[j]*( Ah[2]*sinh(u[i]) + Ah[3] );
		} 
}
