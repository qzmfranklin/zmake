#include <fftw3.h>
#include <mkl.h>
#include <emmintrin.h>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h> 
#include "utils.h"
/******************************************************************************/
/*
 * based on http://stackoverflow.com/questions/5252375/custom-c-assert-macro
 * works outside debug mode too
 */
#define assrt(cond) \
	do \
{ \
	if (!(cond)) \
	{ \
		fprintf(stderr,"ASSRT FAILED\n");		\
		fprintf(stderr,"file: %s\n",__FILE__);		\
		fprintf(stderr,"line: %d\n",__LINE__);		\
		__builtin_trap();				\
	} \
} while(0)  
/******************************************************************************/ 
using namespace std;
double *address64(double *in)
{
	unsigned long long int a = (unsigned long long int)in;
	unsigned long long int b = a%64;
	if (b==0)
		return in;
	else
		return (double*)(a+64-b);
}

double ddotd(const int n, const double *restrict w, const double *restrict f)
{
	double val=0.0;
	for (int i = 0; i < n; i++)
		val += f[i]*w[i];
	return val;
}

double _Complex ddotz(const int n, const double *restrict w, 
		const double _Complex *restrict f)
{
	double _Complex val=0.0 + 0.0*I;
	for (int i = 0; i < n; i++)
		val += f[i]*w[i];
	return val;
}


void cscale(const double _Complex scale, 
		const int n, double _Complex *restrict v)
{
	for (int i = 0; i < n; i++)
		v[i] *= scale;
}

void cdot(const int n, double _Complex *a,  
		double _Complex *b, double _Complex *dest)
{
	for (int i = 0; i < n; i++) 
		dest[i] = a[i]*b[i];
} 

void array_abs(const int n, double *v)
{
	for(int i=0; i < n; i++)
		v[i] = fabs(v[i]);
}

double array_max(const  int n, double *v)
{
	double ans = 0;
	for(int i=0; i < n; i++)
		if(fabs(v[i])> ans)
			ans = fabs(v[i]);
	return ans;
}

void array_diff(const int n, double *restrict v, double *restrict w)
{
	for(int i=0; i < n; i++)
		v[i] -= w[i];
}

void array_copy(const int n, double *restrict v, double *restrict w)
{
	for(int i=0; i < n; i++)
		w[i] = v[i];
}

void array_out(const char* fname, const int m, const int n, double *restrict a,
		int lda)
{
	if (lda<m) lda=m;

	FILE *fp;
	if(fname == NULL) {
		fp = stdout;
		fprintf(stderr, "Export array to STDOUT\n");
	}
	else {
		fp = fopen(fname, "w");
		fprintf(stderr, "Export array to %s\n",fname);
	}

	for(int i=0; i < m; i++){
		for(int j=0; j < n; j++)
			fprintf(fp, "%f  ", a[i+j*lda]);
		fprintf(fp, "\n");
	}

	if(fname != NULL){
		int rval = fclose(fp);
		assrt(rval !=  -1);
	}
}

void array_in(const char* fname, const int n, double *v)
{
	std::ifstream ifile(fname);
	assrt(ifile.is_open());
	for(int i=0; i < n; i++)
		ifile>>v[i];
	ifile.close();
}

void array_flush(const int n, double *v)
{
	for(long int i=0; i < n; i++)
		_mm_clflush(v + i);
}

void solve2x2(double *restrict A, double *restrict rhs, double *restrict x){
	double a = A[0];
	double b = A[2];
	double c = A[1];
	double d = A[3];
	double det = a*d-b*c;
	x[0] = (rhs[0]*d-rhs[1]*b)/det;
	x[1] = (-rhs[0]*c+rhs[1]*a)/det;

}

void verify_dir(const char *dir)
{
	struct stat sb;
	int rval = stat(dir, &sb);
	if((rval == -1) && (errno == ENOENT)){
		mkdir(dir, 0700);
		stat(dir, &sb);
	}
	assrt(S_ISDIR(sb.st_mode));
}

/*
 * link_cout and unlink_cout are deprecated. They are no longer in the header
 * file, i.e., utils.h. They are just provided here as a historical trait.
 */
static std::streambuf *sbuf_backup;
static std::ofstream ofile;
static int linkcout_state = 0;

void link_cout(const char *fname)
{
	assrt(linkcout_state == 0);
	linkcout_state = 1;

	ofile.open(fname);
	assrt(ofile.good());
	sbuf_backup = cout.rdbuf();
	std::cout.rdbuf(ofile.rdbuf());
	assrt(cout.good());
}

void unlink_cout()
{
	assrt(linkcout_state == 1);
	linkcout_state = 0;

	std::cout.rdbuf(sbuf_backup);
	ofile.close();
}

static int ___linkstd_state___ = 0; 
static FILE *___stdout_backup___;

/*
 *void link_stdout(const char *fname)
 *{
 *        if (fname==NULL) {
 *                fprintf(stderr,"link_stdout(const char *fname): fname=NULL\n");
 *                fprintf(stderr, "Use the current stdout.\n");
 *                return;
 *        }
 *
 *        assrt(___linkstd_state___==0);
 *
 *        FILE *fp = fopen(fname,"w");
 *        assrt(fp);
 *        fflush(stdout);	// clear stdout buffer
 *        ___stdout_backup___ = stdout;
 *        stdout = fp;
 *
 *        ___linkstd_state___ = 1; 
 *
 *        fprintf(stderr, "STDOUT => %s\n",fname);
 *} 
 *
 *void unlink_stdout()
 *{
 *        assrt(___linkstd_state___==1);
 *
 *        fflush(stdout);
 *        fclose(stdout);
 *        stdout = ___stdout_backup___;
 *
 *        ___linkstd_state___ = 0; 
 *}
 */
void link_stdout(const char *fname)
{
	assrt(___linkstd_state___==0);
	if (fname==NULL) {
		fprintf(stderr,"link_stdout(const char *fname): fname=NULL\n");
		fprintf(stderr, "Use the current stdout.\n");
		return;
	}
	freopen(fname,"a",stdout);
	___linkstd_state___=1;
}

void unlink_stdout()
{
	assrt(___linkstd_state___==1);
	freopen("/dev/tty","a",stdout);
	___linkstd_state___ = 0; 
}

void box_file(const char* fname, const char*mesg){
	FILE *file;
	file = fopen(fname, "r");
	char c;
	cout<<setw(40)<<mesg<<":"<<fname<<endl;
	cout<<"\t"
		<<"---------------------------------------------------------------------"
		<<endl;
	int line = 0;
	while((c=getc(file))!=EOF){
		if(line==0)
			cout<<'\t'<<'|';
		if((c=='\n')||(line==67)){
			for(int i=line; i < 67; i++)
				cout<<' ';
			line = 0;
			cout<<'|'<<'\n';
		}
		else if(c=='\b'){
			if(line > 0)
				line--;
			cout<<c;
		}
		else if(c=='\t'){
			int i;
			for(i=line; (i < 67)&&(i<line+8); i++)
				cout<<' ';
			line = i;
		}
		else if((32<=c)&&(c<127)){
			cout<<c;
			line++;
		}
	}
	cout<<"\t"
		<<"---------------------------------------------------------------------"
		<<endl;
}

void print_proc_status(const char* mesg){
	int pid = getpid();
	char fname[200];
	sprintf(fname, "/proc/%d/status", pid);
	box_file(fname, mesg);
}

void init_vector(const int n, double* v, int incx)
{
	assrt(incx>0);
	for (int i = 0; i < n; i++)
		v[i*incx] = 10.0 * rand()/RAND_MAX;
}

void init_matrix(const int m, const int n, double* a, int lda)
{
	if (lda<m) lda=m;
	for (int j = 0; j < n; j++)
		for (int i = 0; i < m; i++)
			a[i+j*lda] = 10.0 * rand()/RAND_MAX;
}

void print_vector(const char* title, const int n, const double _Complex *a)
{
	printf("%s\n",title);
	for (int i = 0; i < n; i++)
		printf("[%5d] %+.16E %+.16E*I\n",i,creal(a[i]),cimag(a[i]));
}
void print_vector(const char* title, const int n, const double *a)
{
	printf("%s\n",title);
	for (int i = 0; i < n; i++)
		printf("[%5d] %+.16E\n",i,a[i]);
}

void print_matrix(const char* desc, const int m, const int n, 
		double *a, int lda)
{
	if (lda<m) lda=m;
	printf("%s = \n",desc);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			printf("%7.3f ",a[i+j*lda]);
		printf("\n");
	}
}


void print_cvector(const char* desc, const int n, 
		double _Complex *restrict v, int incx)
{
	assrt(incx>0);
	printf("%s = \n",desc);
	for (int i = 0; i < n; i++)
		printf("%20.15f+%20.15fI\n",creal(v[i*incx]),cimag(v[i*incx]));
	printf("\n");
}

void print_cmatrix(const char* desc, const int m, const int n, 
		double _Complex *restrict a, int lda)
{
	if (lda<m) lda=m;
	printf("%s = \n",desc);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			printf("%7.3f+%7.3fI ",creal(a[i+j*lda]),cimag(a[i+j*lda]));
		printf("\n");
	}
}

void create_fftw_plans(const int n, fftw_plan *plans, const int flag)
{
	//fprintf(stderr, "create_fftw_plans(n=%d,plans,flag)\n",n);
	fftw_complex *work1,*work2;
	//work1        = fftw_alloc_complex(n);
	//work2        = fftw_alloc_complex(n);
	work1=(fftw_complex*)mkl_malloc(sizeof(double)*2*n,64);
	work2=(fftw_complex*)mkl_malloc(sizeof(double)*2*n,64);

	plans[IFWD] = fftw_plan_dft_1d(n,
			(fftw_complex*)work1,
			(fftw_complex*)work1,
			FFTW_FORWARD,flag);
	plans[IBWD] = fftw_plan_dft_1d(n,
			(fftw_complex*)work1,
			(fftw_complex*)work1,
			FFTW_BACKWARD,flag);
	plans[OFWD] = fftw_plan_dft_1d(n,
			(fftw_complex*)work1,
			(fftw_complex*)work2,
			FFTW_FORWARD,flag);
	plans[OBWD] = fftw_plan_dft_1d(n,
			(fftw_complex*)work1,
			(fftw_complex*)work2,
			FFTW_BACKWARD,flag);

	//fftw_free(work1);
	//fftw_free(work2);
	mkl_free(work1);
	mkl_free(work2);
}

void destroy_fftw_plans(fftw_plan *restrict plans)
{
	//fprintf(stderr, "destroy_fftw_plans(plans)\n");
	for (int i = 0; i < 4; i++)
		fftw_destroy_plan(plans[i]);
}

/******************************************************************************/
