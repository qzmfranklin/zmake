#include "rmsm.h"
#include "utils/TimeStat.h"
#include "utils/Table.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>

enum matrix_type {
	ROWMAJOR = 0,
	COLMAJOR,
	RMSM
};

class time_rmsm {
private:
	int status;
	int len; // size of matrix/vector
	double weight; // [0,1] fraction of matrix to be filled
	struct st_rmsm *m;  // row-major-sparse-matrix
	double *in;
	double *out;
public:
	time_rmsm() { status = 0; }

	~time_rmsm() { release(); }

	void set(const int n, const double w)
	{
		if (status>0)
			release();
		assert(status==0);

		len = n;
		weight = w;

		alloc();
		init();

		status = 3;
	}

	double run(const int type, const int numtrials)
	{
		assert(status==3);
		TimeStat clk(numtrials);
		for (int i = 0; i < numtrials; i++) {
			switch ((enum matrix_type)type) {
			case ROWMAJOR:
				clk.tic();
				mul_rowmajor();
				clk.toc();
				break;
			case COLMAJOR:
				clk.tic();
				mul_colmajor();
				clk.toc();
				break;
			case RMSM:
				clk.tic();
				mul_rmsm();
				clk.toc();
				break;
			default:
				fprintf(stderr,"test::run unknown matrix_type %d\n",type);
				fprintf(stderr, "\t0 = ROWMAJOR\n"
						"\t1 = COLMAJOR\n"
						"\t2 = RMSM\n");
				abort();
				break;
			}
		}
		return clk.median();
	}

	double num_operations()
	{
		return rmsm_count_operation(m);
	}

private:
	void alloc()
	{
		assert(status==0);

		m = rmsm_create(len);
		assert(m);
		in = (double*)malloc(sizeof(double)*len);
		assert(in);
		out = (double*)malloc(sizeof(double)*len);
		assert(out);

		status = 1;
	}

	void init()
	{
		assert(status==1);

		srand(time(NULL));
		for (int i = 0; i < len; i++)
			for (int j = 0; j < len; j++)
				rmsm_add(m,10.0*rand()/RAND_MAX,i,j);

		rmsm_pack(m);

		status = 2;
	}

	void release()
	{
		assert(status>0);

		free(in);
		free(out);
		rmsm_destroy(m);

		status = 0;
	}
	void mul_rowmajor()
	{
		for (int i = 0; i < len; i++)
			for (int j = 0; j < len; j++)
				out[i] += rowm[j+i*len] * in[j];
	}

	void mul_colmajor()
	{
		for (int j = 0; j < len; j++)
			for (int i = 0; i < len; i++)
				out[i] += colm[i+j*len] * in[j];
	}

	void mul_rmsm()
	{
		rmsm_mul(m,in,out);
	}
};

int main(int argc, char const* argv[])
{
	if (argc<3) {
		fprintf(stderr,"Usage: time_rmsm.exe [numtrials] [weight]\n");
		exit(1);
	}
	int numtrials;
	double weight;
	sscanf(argv[1],"%d",&numtrials);
	sscanf(argv[2],"%lf",&weight);

	const int m = 9;
	const int n = 3;
	const char* rows[m] = {"64","200","256","1000","1024",
		"4096","15000","100*1000","1000*1000"};
	const char* cols[n] = {"max","median","min"};
	double data[m*n];

	const int lenlist[m] = {64,200,256,1000,1024,
		4096,15000,100*1000,1000*1000};

	time_rmsm t;
	for(int i=0; i < m; i++) {
		printf("%d\n",i);
		t.set(lenlist[i],weight);
		const double cycles = t.run((int)RMSM,numtrials);
		data[2*i  ] = cycles;
		data[2*i+1] = cycles/t.num_operations();
	}

	Table table;
	table.dim(m,n);
	table.rows(rows);
	table.cols(cols);
	table.data(data);
	char banner[BUFSIZ];
	sprintf(banner, "\n"
			"\tBenchmark the performance of the RMSM\n"
			"\t(Row Major Sparse Matrix) against row-\n"
			"\tmajor and col-major matrix.\n\n"
			"\tTime is in CPU cycles.\n\n"
			"\tnrl'd = normalized, i.e., devided the\n"
			"\traw data by the number of mul and add\n"
			"\toperations in a single M-V mul.\n\n"
			"\t\tnumtrials = %d\n"
			"\t\tweight    = %f"
			,numtrials
			,weight);
	table.print(banner);
	return 0;
}
