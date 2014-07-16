#include <unistd.h>
#include <getopt.h>
#include <limits.h>
#include <float.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "utils.h"
#include "mshio.h"
#include "solver_v1.h"
/*******************************************************************************/
// interface from solver_v1.h:
/*
 * ipar[0] = M
 * ipar[1] = Nd
 * ipar[2] = pad
 * ipar[3] = rule1
 * ipar[4] = rule2
 * ipar[5] = nu
 * ipar[6] = nv
 * ipar[7] = num_threads in omp
 */
/*
 * dpar[0] = g factor
 * dpar[1] = mua (absorption coefficient)
 * dpar[2] = mus (scattering coefficient)
 */
static enum {
	EXTRACTION_ORDER,
	SAVE_PARAMETERS,
	SET_M = 101, // spatial interpolation order, M
	SET_ND,
	SET_PAD,
	SET_RULE1,
	SET_RULE2,
	SET_NU,
	SET_NV,
	SET_NUM_THREADS,
	SET_GFACTOR,
	SET_MUA,
	SET_MUS
} long_option_flag;
static int ipar[128]; // ipar for solver_v1, not mkl_fgmres
static double dpar[128];  // dpar for solver_v1, not mkl_fgmres
static char fin_base[FILENAME_MAX]; // base name of input file 
static char output_dir[FILENAME_MAX]; // output directory, i.e., -o output_dir
static int format=2; // intput format 1=ACSII 2=BINARY(default)
static int verbose_flag=0; // message level 0=BRIEF(default) 1=VERBOSE
static int save_parameters_flag=0; // whether to save ipar dpar to file
static char fout_pars[FILENAME_MAX]="rte2dvisv1.pars"; // if yes, where to save ipar dpar
static int extraction_order=0; // 1=first order extraction, default 0
/******************************************************************************/
static void save_pars_to_file(const char *fname)
{
	FILE *fd=fopen(fname,"w");
	assert(fd);
	fprintf(fd,"M=%d\n",		ipar[0]);
	fprintf(fd,"Nd=%d\n",		ipar[1]);
	fprintf(fd,"pad=%d\n",		ipar[2]);
	fprintf(fd,"rule1=%d\n",	ipar[3]);
	fprintf(fd,"rule2=%d\n",	ipar[4]);
	fprintf(fd,"nu=%d\n",		ipar[5]);
	fprintf(fd,"nv=%d\n",		ipar[6]);
	fprintf(fd,"num_threads=%d\n",	ipar[7]);
	fprintf(fd,"gfactor=%.16E\n",	dpar[0]);
	fprintf(fd,"mua=%.16E\n",	dpar[1]);
	fprintf(fd,"mus=%.16E\n",	dpar[2]);
	fclose(fd);
}
static void read_pars_from_file(const char *fname)
{
	size_t bufsize=BUFSIZ;
	char *buf=(char*)malloc(sizeof(char)*BUFSIZ);
	FILE *fd=fopen(fname,"r");
	assert(buf);
	assert(fd);
	while (  (getline(&buf,&bufsize,fd))!=-1  ) { 
		if (strstr(buf,"M="))
			sscanf(buf,"M=%d",ipar);
		else if (strstr(buf,"Nd="))
			sscanf(buf,"Nd=%d",ipar+1);
		else if (strstr(buf,"pad="))
			sscanf(buf,"pad=%d",ipar+2);
		else if (strstr(buf,"rule1="))
			sscanf(buf,"rule1=%d",ipar+3);
		else if (strstr(buf,"rule2="))
			sscanf(buf,"rule2=%d",ipar+4);
		else if (strstr(buf,"nu="))
			sscanf(buf,"nu=%d",ipar+5);
		else if (strstr(buf,"nv="))
			sscanf(buf,"nv=%d",ipar+6);
		else if (strstr(buf,"num_threads="))
			sscanf(buf,"num_threads=%d",ipar+7);
		else if (strstr(buf,"gfactor="))
			sscanf(buf,"gfactor=%lf",dpar);
		else if (strstr(buf,"mua="))
			sscanf(buf,"mua=%lf",dpar+1);
		else if (strstr(buf,"mus="))
			sscanf(buf,"mus=%lf",dpar+2);
	}
	fclose(fd);
	free(buf);
}
void test_limits()
{
	// Read out system-dependent constants
	printf("	FILENAME_MAX	= %d\n",FILENAME_MAX);
	printf("	PIPE_BUF	= %d\n",PIPE_BUF);
	printf("	FOPEN_MAX	= %d\n",FOPEN_MAX);
	printf("	TMP_MAX		= %d\n",TMP_MAX);
	printf("	BUFSIZ		= %d\n",BUFSIZ);
	printf("	CHAR_MAX	= %d\n",CHAR_MAX);
	printf("	CHAR_MIN	= %d\n",CHAR_MIN);
	printf("	INT_MAX		= %d\n",INT_MAX);
	printf("	INT_MIN		= %d\n",INT_MIN);
	printf("	LONG_MAX	= %ld\n",LONG_MAX);
	printf("	LONG_MIN	= %ld\n",LONG_MIN);
	printf("	FLT_EPSILON	= %g\n",FLT_EPSILON);
	printf("	FLT_MIN		= %g\n",FLT_MIN);
	printf("	FLT_MAX		= %g\n",FLT_MAX);
	printf("	FLT_MIN_10_EXP	= %d\n",FLT_MIN_10_EXP);
	printf("	FLT_MAX_10_EXP	= %d\n",FLT_MAX_10_EXP);
	printf("	DBL_EPSILON	= %g\n",DBL_EPSILON);
	printf("	DBL_MIN		= %g\n",DBL_MIN);
	printf("	DBL_MAX		= %g\n",DBL_MAX);
	printf("	DBL_MIN_10_EXP	= %d\n",DBL_MIN_10_EXP);
	printf("	DBL_MAX_10_EXP	= %d\n",DBL_MAX_10_EXP);
}
void print_usage()
{
		/******************************************************************************/
	printf("\tPreprocess a Gmsh file into ascii/binary tables\n");
	printf("SYNOPSIS\n");
	printf("rte2dvisv1 [options] fbase\n");
	printf("\n");
	printf("where:\n");
	printf("* 'fbase' represents the base name of the input files\n");
	printf("* 'options' represents zero or more options. If a same parameter is set\n");
	printf("more than once, only the last one is effective. Note that one does not\n");
	printf("need to specify all the parameters to run.\n");
		/******************************************************************************/
	printf("EXAMPLES\n");
	printf("./rte2dvisv1 square162 --set-M 1 --set-Nd 20 --set-pad 16 --save-parameters\n");
		/******************************************************************************/
	printf("OPTIONS\n");
	printf("--ascii,-a	output format is text\n");
	printf("--binary,-b	output format is binary (default)\n");
	printf("--extraction-order	order of extractions [0]\n");
	printf("--file [file]	read parameters from 'file'\n");
	printf("--help,-h	print this usage message\n");
	printf("--save-parameters [optional,file] save parameters to 'file' [rte2dvisv1.pars]\n");
	printf("--set-M		spatial interpolation order\n");
	printf("--set-Nd	max harmonic order\n");
	printf("--set-pad	padding for better FFTW performance\n");
	printf("--set-rule1	quadrature rule number for testing integrals\n");
	printf("--set-rule2	quadrature rule number for non-singular source integrals\n");
	printf("--set-nu	\"angular\" number of points for singular source integrals\n");
	printf("--set-nv	\"radial\" number of points for singular source integrals\n"); 
	printf("--set-num_threads,-j	number of openmp threads to use\n");
	printf("--set-gfactor	g factor in Henyey-Greenstein phase function\n");
	printf("--set-mua	absorption coefficient\n");
	printf("--set-mus	scattering coefficient\n");
	printf("-o [dir]	save outputs to 'dir'\n");
	printf("--test,-t	print limits of the host architecture\n");
	printf("--verbose,-t	verbose mode (not default)\n");
		/******************************************************************************/
}
void opt_proc(int argc, char *argv[])
{
	// specify default values
	fin_base[0]=0;
	output_dir[0]=0;

	ipar[0] = 0;
	ipar[1] = 20;
	ipar[2] = 1;
	ipar[3] = 1;
	ipar[4] = 1;
	ipar[5] = 5;
	ipar[6] = 3;
	ipar[7] = 1;
	dpar[0] = 0.5;
	dpar[1] = 1.0;
	dpar[2] = 0.0;

	int *lofa=(int*)(&long_option_flag); // long option flag address
	static struct option long_options[] = {
		{"ascii",	no_argument,0,'a'},
		{"binary",	no_argument,0,'b'},
		{"extraction-order",required_argument,lofa,EXTRACTION_ORDER},
		{"file",	required_argument,0,'f'},
		{"help",	no_argument,0,'h'},
		{"save-parameters",optional_argument,lofa,SAVE_PARAMETERS},
		{"set-M",	required_argument,lofa,SET_M},
		{"set-Nd",	required_argument,lofa,SET_ND},
		{"set-pad",	required_argument,lofa,SET_PAD},
		{"set-rule1",	required_argument,lofa,SET_RULE1},
		{"set-rule2",	required_argument,lofa,SET_RULE2},
		{"set-nu",	required_argument,lofa,SET_NU},
		{"set-nv",	required_argument,lofa,SET_NV}, 
		{"set-num_threads",required_argument,0,'j'}, 
		{"set-gfactor",	required_argument,lofa,SET_GFACTOR},
		{"set-mua",	required_argument,lofa,SET_MUA},
		{"set-mus",	required_argument,lofa,SET_MUS},
		{"test",	no_argument,0,'t'},
		{"verbose",	no_argument,0,'v'},
		{0,0,0,0}
	};
	int option_index=0;
	int c;
	while ( (c=getopt_long_only(argc,argv,"abf:hj:o:tv",long_options,&option_index)) != -1) {
		switch (c) {
		case 0:
			//switch (long_options[option_index].flag)
			if (long_option_flag==EXTRACTION_ORDER) {
				sscanf(optarg,"%d",&extraction_order);
				//printf("extraction_order           = %d\n",ipar[0]);
			} else if (long_option_flag==SAVE_PARAMETERS) {
				save_parameters_flag=1;
				if (!optarg)
					strcpy(fout_pars,"solver_v1_pars");
				else
					strcpy(fout_pars,optarg);
			} else if (long_option_flag==SET_M)
				sscanf(optarg,"%d",ipar);
			else if (long_option_flag==SET_ND)
				sscanf(optarg,"%d",ipar+1); 
			else if (long_option_flag==SET_PAD) 
				sscanf(optarg,"%d",ipar+2); 
			else if (long_option_flag==SET_RULE1)
			      sscanf(optarg,"%d",ipar+3); 
			else if (long_option_flag==SET_RULE2)
			      sscanf(optarg,"%d",ipar+4); 
			else if (long_option_flag==SET_NU)
			      sscanf(optarg,"%d",ipar+5); 
			else if (long_option_flag==SET_NV)
			      sscanf(optarg,"%d",ipar+7); 
			// has been merged with short option -j
			//else if (long_option_flag==SET_NUM_THREADS)
			      //sscanf(optarg,"%d",ipar+8); 
			else if (long_option_flag==SET_GFACTOR)
			      sscanf(optarg,"%lf",dpar); 
			else if (long_option_flag==SET_MUA)
			      sscanf(optarg,"%lf",dpar+1); 
			else if (long_option_flag==SET_MUS)
			      sscanf(optarg,"%lf",dpar+2); 
			break;
		case 'a':
			format=1;
			break;
		case 'b':
			format=2;
			break;
		case 'f':
			read_pars_from_file(optarg);
			break;
		case 'h':
			print_usage();
			exit(0);
			break;
		case 'j':
			sscanf(optarg,"%d",ipar+8);
			break;
		case 't':
			test_limits();
			exit(0);
			break;
		case 'v':
			verbose_flag=1;
			break;
		case 'o':
			strcpy(output_dir,optarg);
			break;
		case '?':
			printf("%s\n",optarg);
			print_usage();
			exit(1);
			break;
		default:
			abort();
		}
	}
	// The remaining command line arguments is the input mesh files
	if (optind>=argc) {
		printf("Requires a base name for the input file as argument!\n");
		print_usage();
		exit(1);
	}
	strcpy(fin_base,argv[optind]);
	{// Delete int extension ".msh", if any.
		// might remove this function later
		char * temp= strstr(fin_base,".msh");
		if (temp) *temp=0;
	}
	if (save_parameters_flag) save_pars_to_file(fout_pars); 
	if (output_dir[0]==0) strcpy(output_dir,"SOL"); 
}
void solve0()
{
	struct st_mesh *q=mshio_create_mesh(fin_base);
	//mshio_print_mesh(q,PRINT_INFO_VERBOSE);

	struct st_solver_v1 *s=sv1_create_solver(q,ipar,dpar);
	sv1_print_solverinfo(s);

	double _Complex *b0=(double _Complex*)mkl_malloc(sizeof(double _Complex)*s->Ng,64);
	double _Complex *x0=(double _Complex*)mkl_malloc(sizeof(double _Complex)*s->Ng,64);
	//double _Complex *b1=(double _Complex*)mkl_malloc(sizeof(double _Complex)*s->Ng,64);
	//double _Complex *x1=(double _Complex*)mkl_malloc(sizeof(double _Complex)*s->Ng,64);
	assert(b0);
	assert(x0);
	//assert(b1);
	//assert(x1);
	int nitr;
	double eps;

	sv1_gen_b0(s,0.0,b0);
	sv1_solve(s,b0,x0,200,12,1.0E-13,&nitr,&eps); 
	sv1_save_solution(s,x0,output_dir); 

	//sv1_gen_b1x0(s,0.0,b1,x0);
	//sv1_solve(s,b1,x1,200,30,1.0E-13,&nitr,&eps); 
	//sv1_save_solution(s,x1,output_dir); 

	mkl_free(b0);
	mkl_free(x0);
	//mkl_free(b1);
	//mkl_free(x1);
	sv1_destroy_solver(s); 
	mshio_destroy_mesh(q);
}
void solve1()
{
	struct st_mesh *q=mshio_create_mesh(fin_base);
	//mshio_print_mesh(q,PRINT_INFO_VERBOSE);

	struct st_solver_v1 *s=sv1_create_solver(q,ipar,dpar);
	sv1_print_solverinfo(s);

	//double _Complex *b0=(double _Complex*)mkl_malloc(sizeof(double _Complex)*s->Ng,64);
	double _Complex *x0=(double _Complex*)mkl_malloc(sizeof(double _Complex)*s->Ng,64);
	double _Complex *b1=(double _Complex*)mkl_malloc(sizeof(double _Complex)*s->Ng,64);
	double _Complex *x1=(double _Complex*)mkl_malloc(sizeof(double _Complex)*s->Ng,64);
	//assert(b0);
	assert(x0);
	assert(b1);
	assert(x1);
	int nitr;
	double eps;

	//sv1_gen_b0(s,0.0,b0);
	//sv1_solve(s,b0,x0,200,12,1.0E-13,&nitr,&eps); 
	//sv1_save_solution(s,x0,output_dir); 

	sv1_gen_b1x0(s,0.0,b1,x0);
	sv1_solve(s,b1,x1,200,30,1.0E-13,&nitr,&eps); 
	sv1_save_solution(s,x1,output_dir); 

	//mkl_free(b0);
	mkl_free(x0);
	mkl_free(b1);
	mkl_free(x1);
	sv1_destroy_solver(s); 
	mshio_destroy_mesh(q);
}
/******************************************************************************/ 
int main(int argc, char *argv[])
{ 
	opt_proc(argc,argv);

	if (extraction_order==0)
		solve0();
	else if (extraction_order==1)
		solve1();
	else {
		fprintf(stderr,"extraction_order must be 0 or 1, abort\n");
		exit(-1);
	}

	return 0;
} 
