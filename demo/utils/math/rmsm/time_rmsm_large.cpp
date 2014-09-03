#include "rmsm.h"
#include "utils/TimeStat.h"
#include "utils/Table.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>

class time_rmsm_large {
private:
	int status;
	int len; // size of matrix/vector
	double weight; // [0,1] fraction of matrix to be filled
	struct st_rmsm *m;  // row-major-sparse-matrix
	double *in;
	double *out;
public:
	time_rmsm_large() { status = 0; }

	~time_rmsm_large() { release(); }

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

	double run(const int numtrials)
	{
		assert(status==3);
		TimeStat clk(numtrials);
		for (int i = 0; i < numtrials; i++) {
				clk.tic();
				rmsm_mul(m,in,out);
				clk.toc();
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

		const int num_elements = weight * len * len;

		srand(time(NULL));
		for (int i = 0; i < num_elements; i++) {
			const int row = rand() % len;
			const int col = rand() % len;
			const double val = 1.0 * rand() / RAND_MAX;
			rmsm_add(m,val,row,col);
		}

		rmsm_pack(m,RMSM_RELERR,1E-10);

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
};

int main(int argc, char const* argv[])
{
	if (argc<3) {
		fprintf(stderr,"Usage: time_rmsm_large.exe [numtrials] [weight]\n");
		exit(1);
	}
	int numtrials;
	double weight;
	sscanf(argv[1],"%d",&numtrials);
	sscanf(argv[2],"%lf",&weight);

	const int m = 10;
	const int n = 2;
	const char* rows[m] = {"64","200","256","1000","1024",
		"4096","15000","100*1000","1000*1000","10*1000*1000"};
	const char* cols[n] = {"median","median/num_ops"};
	double data[m*n];

	const int lenlist[m] = {64,200,256,1000,1024,
		4096,15000,100*1000,1000*1000,10*1000*1000};

	time_rmsm_large t;
	for(int i=0; i < m; i++) {
		printf("\t%d %d\n",i,lenlist[i]);
		t.set(lenlist[i],weight);
		const double cycles = t.run(numtrials);
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
			"\t(Row Major Sparse Matrix)\n\n"
			"\tTime is in CPU cycles.\n\n"
			"\t\tnumtrials = %d\n"
			"\t\tweight    = %f"
			,numtrials
			,weight);
	table.print(banner);
	return 0;
}
