#include "convex_hull.h"

static void print_array(const int n, const double _Complex *restrict a)
{
	for (int i = 0; i < n; i++)
		printf("[%5d] %7.3f %23.16E %23.16E \n",i,
				creal(a[i]),cimag(a[i]),carg(a[i]));
}

static int status=0;

static void read_binary(FILE *restrict fd, size_t *restrict n, 
		double _Complex *restrict *a)
{
	assert(status==0);
	fread(n,sizeof(size_t),1,fd);
	int N=*n;
	*a = (double _Complex*)malloc(sizeof(double _Complex)*N);
	assert(*a);
	fread(*a,sizeof(double _Complex),N,fd);
	status=1;
}

static void read_ascii(FILE *restrict fd, size_t *restrict n, 
		double _Complex *restrict *a)
{
	assert(status==0);
	fscanf(fd,"%lu\n",n);
	int N=*n;
	double *tmp = (double*)malloc(sizeof(double _Complex)*N);
	assert(*a);
	for (int i = 0; i < N; i++)
		fscanf(fd,"%lf %lf\n",tmp+2*i,tmp+2*i+1);
	*a = (double _Complex*)tmp;
	status=1;
}


static double _Complex z0; // defined in find_min_y()
static void find_min_y(const int n, double _Complex *restrict a)
{
	assert(status==1);
	int    idx  = 0;
	double ymin = cimag(a[0]);
	for (int i = 1; i < n; i++)
		if (cimag(a[i])<ymin) {
			ymin = cimag(a[i]);
			idx  = i;
		} else 
			if (cimag(a[i])-ymin<1.0E-15 && creal(a[i])<creal(a[idx])) {
				ymin = cimag(a[i]);
				idx  = i;
			}
	double _Complex tmp;
	tmp    = a[0];
	a[0]   = a[idx];
	a[idx] = tmp;
	z0     = a[0];
	status=2;
}

/**************************************/
static int swp_count=0;
static void swp(double _Complex *a, double _Complex *b)
{
	swp_count++;
	//fprintf(stderr,"\t%.2f + %.2f*I <-> %.2f + %.2f*I\n",
			//creal(*a),cimag(*a),creal(*b),cimag(*b));
	double _Complex c = *a;
	*a = *b;
	*b = c;
}
static void swp(double *a, double *b)
{
	double c = *a;
	*a = *b;
	*b = c;
}
static double *__args;
static void sink3(const int size, double _Complex *restrict a, const int n)
{
	//fprintf(stderr,"sink3\n");
	//double _Complex *pt = a+n;
	//double _Complex *c1 = a+3*n+1;
	//double _Complex *c2 = a+3*n+2;
	//double _Complex *c3 = a+3*n+3;
	
	// have to compute args every time, less than optimal...
	//const double arg0 = carg(*pt-z0);
	//const double arg1 = carg(*c1-z0);
	//const double arg2 = carg(*c2-z0);
	//const double arg3 = carg(*c3-z0);
	
	// precomputed all args, stored in __args, FIXME
	//const double arg0 = __args[n];
	//const double arg1 = __args[3*n+1];
	//const double arg2 = __args[3*n+2];
	//const double arg3 = __args[3*n+3];
	if (3*n+1>=size)
		return;
	else if (3*n+2==size) {
		double _Complex *pt = a+n;
		double _Complex *c1 = a+3*n+1;
		const double arg0 = carg(*pt-z0);
		const double arg1 = carg(*c1-z0);
		/*
		 *const double arg0 = __args[n];
		 *const double arg1 = __args[3*n+1];
		 */
		if (arg1>arg0) {
			swp(pt,c1);
			//swp(__args+n,__args+3*n+1);
			sink3(size,a,3*n+1); 
		}
	} else if (3*n+3==size) {
		double _Complex *pt = a+n;
		double _Complex *c1 = a+3*n+1;
		double _Complex *c2 = a+3*n+2;
		const double arg0 = carg(*pt-z0);
		const double arg1 = carg(*c1-z0);
		const double arg2 = carg(*c2-z0);
		/*
		 *const double arg0 = __args[n];
		 *const double arg1 = __args[3*n+1];
		 *const double arg2 = __args[3*n+2];
		 */
		if (arg1>arg0 && arg1>arg2) {
			swp(pt,c1);
			//swp(__args+n,__args+3*n+1);
			sink3(size,a,3*n+1);
		} else if (arg2>arg0 && arg2>arg1) {
			swp(pt,c2);
			//swp(__args+n,__args+3*n+2);
			sink3(size,a,3*n+2);
		}
	} else { 
		double _Complex *pt = a+n;
		double _Complex *c1 = a+3*n+1;
		double _Complex *c2 = a+3*n+2;
		double _Complex *c3 = a+3*n+3;
		const double arg0 = carg(*pt-z0);
		const double arg1 = carg(*c1-z0);
		const double arg2 = carg(*c2-z0);
		const double arg3 = carg(*c3-z0);
		/*
		 *const double arg0 = __args[n];
		 *const double arg1 = __args[3*n+1];
		 *const double arg2 = __args[3*n+2];
		 *const double arg3 = __args[3*n+3];
		 */
		if (arg1>arg0 && arg1>arg2 && arg1>arg3) {
			swp(pt,c1);
			//swp(__args+n,__args+3*n+1);
			sink3(size,a,3*n+1);
		} else if (arg2>arg0 && arg2>arg1 && arg2>arg3) {
			swp(pt,c2);
			//swp(__args+n,__args+3*n+2);
			sink3(size,a,3*n+2);
		} else if (arg3>arg0 && arg3>arg1 && arg3>arg2) {
			swp(pt,c3);
			//swp(__args+n,__args+3*n+3);
			sink3(size,a,3*n+3);
		}
	}
}
static void crtheap3(const int size, double _Complex *restrict a)
{
	//fprintf(stderr,"crtheap3\n");
	if (size<2) return; 

	int n=size/3;
	while (--n>-1) {
		//fprintf(stderr,"n = %d\n",n);
		sink3(size,a,n);
	}

	return;
}
void heapsort3(const int size, double _Complex *restrict a)
{
	//fprintf(stderr,"heapsort3\n");

	if (!size) return;

	swp_count=0;

	crtheap3(size,a); 

	int n=size;
	do {
		//fprintf(stderr,"pop\n");
		swp(a,a+n-1);
		sink3(n-1,a,0);
	} while (--n>0);

	//fprintf(stderr,"swp_count=%d\n",swp_count);

	return;
}

// in-place tri-node heapsort
static void sort_raw_by_args(const int n, double _Complex *restrict a)
{
	assert(status==2);

	/*
	 *__args = (double*)malloc(sizeof(double)*n);
	 *fprintf(stderr,"__args = %p\n",__args);
	 *assert(__args);
	 *__args[0]=0.0;
	 *for (int i = 1; i < n; i++) {
	 *        __args[i] = carg(a[i]-z0);
	 *        fprintf(stderr,"[%3d] args[i] = %7.2f\n",i,__args[i]);
	 *}
	 */

	heapsort3(n-1,a+1);

	//free(__args);
	status=3;
}
/**************************************/

static inline double norm2(const double _Complex c)
{
	return creal(c)*creal(c) + cimag(c)*cimag(c);
}

static void delete_colinear_args(size_t *n, double _Complex *restrict *a)
{
	assert(status==3);
	size_t n_new = *n;
	//const double _Complex z0 = **a; // no need to shadow static global z0
	double arg_prev = carg(*(*a+1)-z0);
	for (int i = 2; i < *n; i++) {
		const double arg_curr = carg(*(*a+i)-z0);
		if (fabs(arg_curr-arg_prev)<1.0E-15) n_new--;
		arg_prev = arg_curr;
	}

	double _Complex *a_new = (double _Complex*)malloc(sizeof(double _Complex)*n_new);
	assert(a_new);
	arg_prev = carg(*(*a+1)-z0);
	for (int i = 0; i < 2; i++)
		a_new[i] = *(*a+i);
	for (int i = 2; i < *n; i++) {
		static int k=2;
		const double arg_curr = carg(*(*a+i)-z0);
		//fprintf(stderr,"[i=%-3d  k=%-3d] %7.3f > %7.3f\n",i,k,arg_curr,arg_prev);
		if (arg_curr-arg_prev<1.0E-15) {
			if ( norm2(*(*a+i)-z0) > norm2(a_new[k]-z0) )
				a_new[k++] = *(*a+i);
		} else
			a_new[k++] = *(*a+i);
		arg_prev = arg_curr;
	}

	free(*a);
	*a = a_new;
	*n = n_new;
	status==4;
}

/**************************************/
// det|(b-a)x(c-b)|, counter-clockwise = positive
static inline double det(const double _Complex *restrict a, 
		const double _Complex *restrict b, 
		const double _Complex *restrict c)
{
	const double _Complex z1 = *b - *a;
	const double _Complex z2 = *c - *b;
	return creal(z1)*cimag(z2) - creal(z2)*cimag(z1);
}

// in-place, use original array as the stack
// Ref: Cormen 3rd P1031
static void graham_scan0(size_t *n, double _Complex *restrict *a)
{
	assert(status==4);
	if (*n<4) return;

	int top=2;
	for (int i = 3; i < *n; i++) {
		//const double area = det(*a+top-1,*a+top,*a+i);
		//fprintf(stderr,"top = %2d    i = %2d   area = %7.2f\n",top,i,area);
		while (det(*a+top-1,*a+top,*a+i)<0.0) top--;
		*(*a+(++top)) = *(*a+i);
	}
	top++;

	double _Complex *a_new = (double _Complex*)malloc(sizeof(double _Complex)*top);
	assert(a_new);
	for (int i = 0; i < top; i++)
		a_new[i] = *(*a+i);
	free(*a);
	*a = a_new;
	*n = top;
	status=5;
}

/**************************************/
static void output_binary(FILE *fd, const size_t n, const double _Complex *restrict a)
{
	//assert(status==5);
	fwrite(&n,sizeof(size_t),1,fd);
	fwrite(a,sizeof(double _Complex),n,fd);
	//status=6;
}

static void output_ascii(FILE *fd, const size_t n, const double _Complex *restrict a)
{
	//assert(status==5);
	fprintf(fd,"%lu\n",n);
	for (int i = 0; i < n; i++)
		fprintf(fd,"%23.16E %23.16E\n",creal(a[i]),cimag(a[i]));
	//status=6;
}

void graham_scan(size_t *restrict size, double _Complex *restrict *data0)
{
	size_t N = *size;
	double _Complex *data = *data0;

	status=1;
	find_min_y(N,data);
	sort_raw_by_args(N,data);
	delete_colinear_args(&N,&data);

	graham_scan0(&N,&data);

	*size  = N;
	*data0 = data;
}

/*
 *int main(int argc, char const* argv[])
 *{
 *        size_t N;
 *        double _Complex *data;
 *
 *        //read_binary(stdin,&N,&data);
 *        read_ascii(stdin,&N,&data);
 *        //print_array(N,data);
 *        //fprintf(stderr,"read OK!\n");
 *
 *        find_min_y(N,data);
 *        //fprintf(stderr,"find_min_y OK!\n");
 *        sort_raw_by_args(N,data);
 *        //fprintf(stderr,"sort OK!\n");
 *        delete_colinear_args(&N,&data);
 *        //print_array(N,data);
 *        //fprintf(stderr,"delete colinear OK!\n");
 *
 *        graham_scan0(&N,&data);
 *        //print_array(N,data);
 *        //fprintf(stderr,"Graham scan OK!\n");
 *        
 *        //output_binary(stdout,N,data);
 *        output_ascii(stdout,N,data);
 *
 *        free(data);
 *
 *        return 0;
 *}
 */
