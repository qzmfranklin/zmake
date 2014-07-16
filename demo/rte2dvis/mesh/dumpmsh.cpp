#include <unistd.h>
#include <getopt.h>
#include <limits.h>
#include <float.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mshio.h"
#include "utils.h"
/******************************************************************************/
static void print_usage();
static void test_limits();
static void opt_proc(int argc, char *argv[]);
static int dump_msh(const char * fname_in, const char * fname_out, int format);
/*******************************************************************************/
static char fname_in[FILENAME_MAX];
static char fname_out[FILENAME_MAX];
static int format=2; // 1=ACSII 2=BINARY(default)
static int verbose_flag=0; // 0=UNSET(default) 1=SET
/******************************************************************************/ 
int main(int argc, char *argv[])
{ 
	//test_limits();
	opt_proc(argc,argv);
	int err=dump_msh(fname_in,fname_out,format); 

	return err;
} 
void print_usage()
{
		/******************************************************************************/
	printf("\tPreprocess a Gmsh file into ascii/binary tables\n");
	printf("SYNOPSIS\n");
	printf("	dump [options] file\n");
	printf("\n");
	printf("	where:\n");
	printf("	* 'options' represents zero or more dump options.\n");
	printf("	* 'file' represents a Gmsh format file.\n");
		/******************************************************************************/
	printf("OPTIONS\n");
	printf("	--ascii/-a	output format is text\n");
	printf("	--binary/-b	output format is binary(default)\n");
	printf("	--help/-h	print this usage message\n");
	printf("	-o [target]	direct the output to 'target'\n");
	printf("	--test/-t	print limits of the host architecture\n");
	printf("	--verbose/-t	verbose mode(not default)\n");
		/******************************************************************************/
}

void opt_proc(int argc, char *argv[])
{
	opterr=0; // ignore any getopt error

	fname_out[0]='\0';

	while (1) {
		static struct option long_options[] = {
			{"ascii",no_argument,0,'a'},
			{"binary",no_argument,0,'b'},
			{"help",no_argument,0,'h'},
			{"test",no_argument,0,'t'},
			{"verbose",no_argument,0,'v'},
			{0,0,0,0}
		};
		// getopt_long stores the option index here.
		int option_index = 0;

		int c = getopt_long (argc, argv, "abho:tv",
				long_options, &option_index);

		// Detect the end of the options.
		if (c == -1) break;

		switch (c) {
		case 0:
			// If this option set a flag, do nothing else now. 
			if (long_options[option_index].flag != 0)
				break;
			else {
				//printf("%s\n",long_options[option_index].name);
			}
			break;
		case 'a':
			format=1;
			break;
		case 'b':
			format=2;
			break;
		case 'h':
			print_usage();
			exit(0);
			break;
		case 't':
			test_limits();
			exit(0);
			break;
		case 'v':
			verbose_flag=1;
			break;
		case 'o':
			strcpy(fname_out,optarg);
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

	// The remaining command line arguments is the GSM file. 
	if (optind>=argc) {
		printf("Requires a MSH file as argument!\n");
		print_usage();
		exit(1);
	}
	strcpy(fname_in,argv[optind]);

	// If not specified -o [target], use the same file name as the input 
	if (fname_out[0]=='\0')
		strcpy(fname_out,fname_in);

	{// Delete extension ".msh", if any.
		char * temp= strstr(fname_out,".msh");
		if (temp) *temp=0;
	} 

	if (verbose_flag==1) {
		printf("From   : %s\n",fname_in);
		printf("To     : %s\n",fname_out);
		if (format==1)
			printf("Format : ASCII\n");
		else if (format==2) 
			printf("Format : BINARY\n");
	}
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

/*
 * Read MSH file, dump the verts and trigs into ASCII/BINARY files.
 *   |output fnames are:
 *   |	fname_in.verts
 *   |	fname_in.trigs
 *   |	fname_in.info
 *   |Inputs:
 *   |	const char * fname_in:		name of the MSH file to read
 *   |	const char * fname_out:		base name of files to write
 *   |					NULL to enable default behavior
 *   |	int format:			1=ASCII 2=BINARY
 *   |Outputs: 
 *   |	fname_out.verts:		verts coordinates
 *   |	fname_out.trigs:		triangles integer pairs
 *   |	fname_out.info:			the number of verts and triangles
 *   ------------------------------
 */ 
int dump_msh(const char * fname_in, const char * fname_out, int format)
{
	int err=0;
	// Default fname_out
	if (  !fname_out  ) {
		printf("	fname_out == NULL, proceed with default fname_out:\n");
		printf("		%s\n",fname_in);
		fname_out = fname_in;
	}
	assert( fname_out);
	// Construct ouuput file names
	char *fname_out_info;
	fname_out_info 	= (char*) malloc( FILENAME_MAX * sizeof(char) );
	assert(fname_out_info );
	strcpy(fname_out_info, fname_out);
	strcat(fname_out_info, ".info" );

	char *fname_out_verts;
	fname_out_verts = (char*) malloc( FILENAME_MAX * sizeof(char) );
	assert(fname_out_verts);
	strcpy(fname_out_verts,fname_out);
	strcat(fname_out_verts,".verts");

	char *fname_out_trigs;
	fname_out_trigs = (char*) malloc( FILENAME_MAX * sizeof(char) );
	assert(fname_out_trigs);
	strcpy(fname_out_trigs,fname_out);
	strcat(fname_out_trigs,".trigs");

	char *fname_out_edges;
	fname_out_edges = (char*) malloc( FILENAME_MAX * sizeof(char) );
	assert(fname_out_edges);
	strcpy(fname_out_edges,fname_out);
	strcat(fname_out_edges,".edges");

	// Open 4 files:
	// 	fin:		input MSH file
	// 	fout_info:	output ASCII info file
	// 	fout_verts:	output ASCII verts file
	// 	fout_trigs:	output ASCII trigs file
	// 	fout_edges:	output ASCII edges file
	/*printf("	------------------------------\n");*/
	FILE *fin       =fopen(fname_in       ,"r");
	FILE *fout_info =fopen(fname_out_info ,"w");
	FILE *fout_verts=fopen(fname_out_verts,"w");
	FILE *fout_trigs=fopen(fname_out_trigs,"w");
	FILE *fout_edges=fopen(fname_out_edges,"w");
	assert(fin);
	assert(fout_info);
	assert(fout_verts);
	assert(fout_trigs);
	assert(fout_edges);
	//printf("	------------------------------\n");

	int verts_num;
	int num_elements;
	int trigs_num;
	int edges_num;
	char str_Nodes[]="Nodes";
	char str_EndNodes[]="EndNodes";
	char str_Elements[]="Elements";
	char str_EndElements[]="EndElements";
	char buff[BUFSIZ];
	int i=0;
	// Find verts
	//printf("	Search for \"%s\"...\n",str_Nodes);
	while (  (fgets(buff,BUFSIZ,fin))  ) {
		i++;
		if ( strstr(buff,str_Nodes)  ) {
			//printf("		Found \"%s\" at line %d\n",str_Nodes,i);
			break;
		}
	}
	// Scan verts and write to fout_verts
	fscanf(fin,"%d",&verts_num);
	//printf("	Number of verts is\n");
	//printf("		%d\n",verts_num);
	//printf("	Start scanning verts and writing to fname_out_verts...\n");
	for (i = 0; i < verts_num; i++) {
		int ip,z_zero;
		double x,y;
		fscanf(fin,"%d %lf %lf %d\n",&ip,&x,&y,&z_zero);
		//printf("\t\t\t%3d\t%12.8E\t%12.8E\n",ip,x,y);
		switch (format) {
		case 1:  // ASCII
			fprintf(fout_verts,"%+25.19E %+25.19E\n",x,y);
			break;
		case 2:  // BINARY
			fwrite(&x,sizeof(double),1,fout_verts);
			fwrite(&y,sizeof(double),1,fout_verts);
			break;
		}
	}
	//printf("	Finished scanning verts and writing.\n");
	//printf("	------------------------------\n");
	// Find trigs
	rewind(fin);
	i=0;
	//printf("	Search for \"%s\"...\n",str_Elements);
	while (  (fgets(buff,BUFSIZ,fin))  ) {
		i++;
		if ( strstr(buff,str_Elements)  ) {
			//printf("		Found \"%s\" at line %d\n.",str_Elements,i);
			break;
		}
	}
	// Scan trigs and write to fout_trigs
	fscanf(fin,"%d",&num_elements);
	i++;
	//printf("	Number of elements is\n");
	//printf("		%d\n",num_elements);
	trigs_num=0;
	edges_num=0;
	//printf("	Start Scanning elements and writing trigs to fname_out_trigs...\n");
	int j;
	for ( j = 0; j < num_elements; j++) {
		int tri[5],n1,n2,n3;
		fscanf(fin,"%d %d %d %d %d",&tri[0],&tri[1],&tri[2],&tri[3],&tri[4]);
		fgets(buff,BUFSIZ,fin);
		i++;
		if ( tri[1]==1 ) {
			edges_num++;
			sscanf(buff," %d %d\n",&n1,&n2);
			switch (format) {
			case 1:  // ASCII
				fprintf(fout_edges,"%12d %12d\n",n1,n2);
				break;
			case 2:  // BINARY
				fwrite(&n1,sizeof(int),1,fout_edges);
				fwrite(&n2,sizeof(int),1,fout_edges);
				break;
			}
		} else if ( tri[1]==2 ) {
			trigs_num++;
			sscanf(buff," %d %d %d\n",&n1,&n2,&n3);
			switch (format) {
			case 1:  // ASCII
				fprintf(fout_trigs,"%12d %12d %12d\n",n1,n2,n3);
				break;
			case 2:  // BINARY
				fwrite(&n1,sizeof(int),1,fout_trigs);
				fwrite(&n2,sizeof(int),1,fout_trigs);
				fwrite(&n3,sizeof(int),1,fout_trigs);
				break;
			}
		}
	}
	//printf("	Finished scanning elements and writing trigs.\n");
	//printf("	Found %d trigs.\n",trigs_num);
	// Write to fout_info
	//printf("	------------------------------\n"); 
	//printf("	Start writing into fname_out_info...\n");
	fprintf(fout_info,"%d\t\t# 1=ASCII 2=BINARY\n",format);
	fprintf(fout_info,"%d\t\t# number of verts\n",verts_num);
	fprintf(fout_info,"%d\t\t# number of trigs\n",trigs_num);
	fprintf(fout_info,"%d\t\t# number of edges\n",edges_num);
	//printf("	Finished writing into fname_out_info.\n");

	// Free memory
	//printf("	------------------------------\n");
	//printf("	Freeing memory...\n");
	fclose(fin);
	fclose(fout_verts);
	fclose(fout_trigs);
	fclose(fout_info);
	free(fname_out_verts);
	free(fname_out_trigs);
	free(fname_out_info);
	//printf("	------------------------------\n");

	//printf("END:	FILEIO_DUMP_MSH_FILE\n");
	//printf("--------------------------------------------------\n");

	return err;
}
