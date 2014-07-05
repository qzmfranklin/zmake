#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "comp_geo/geo_utils.h"
#include "closest_pair.h"
#include "utils/Table.h"
#include "utils/TimeStat.h"

#define fprintf(stderr,...)

class test {
	private:
		int status;
		int size;
		double *data;
	public:
		test(void) { status=0; }
		~test() { reset(); }

		double n()     const { return size; }
		double nlgn()  const { return size*log(size)/log(2.0); }
		double nlgn2() const { return size*pow(log(size)/log(2.0),2.0); }
	private:
		void reset() {
			fprintf(stderr,"\treset\n");
			if (status>0) free(data);
			status=0;
		}
		void alloc() {
			fprintf(stderr,"\talloc: size = %d\n",size);
			assert(status==0);
			assert(size>0);
			data = (double *)malloc(sizeof(double)*size*2);
			assert(data);
			status=1;
		}
		void gen() {
			fprintf(stderr,"\tgen\n");
			assert(status==1);
			for (int i = 0; i < size*2; i++)
				data[i] = 1.0*rand()/RAND_MAX*1.0E4;
			status=2;
		}
	public:
		void set(const int n) {
			fprintf(stderr,"set\n");
			size = n;
			reset();
			alloc();
			gen();
		}

		double run(const int num_trials) {
			fprintf(stderr,"run\n");
			fprintf(stderr,"size       = %d\n",size);
			fprintf(stderr,"num_trials = %d\n",num_trials);
			fprintf(stderr,"data       = %p\n",data);
			assert(status==2);
			TimeStat clk(num_trials);
			double *tmp=(double*)malloc(sizeof(double)*size*2);
			assert(tmp);
			for (int i = 0; i < num_trials; i++) {
				//printf("\t%d\n",i);
				memcpy(tmp,data,sizeof(double)*size*2);
				double _Complex pos[2];
				clk.tic();
				closest_pair(size,tmp,pos);
				clk.toc();
			}
			free(tmp);
			status=3;
			return clk.median();
		}
};

int main(int argc, char const* argv[])
{
	const int m = 9;
	const int n = 4;
	const char* rows[m] = {"10","50","100","500","5000",
		"10*1000","1000*1000","10*1000*1000","50*1000*1000"};
	const char* cols[n] = {"median","/n","/nlgn","/nlgn2"};
	const int array_size[m]={10,50,100,500,5000,
		10000,1000000,10000000,40000000};
	double data[m*n];

	test t;
	const int num_trials = 10;
	for (int i = 0; i < m; i++) {
		//printf("%d\n",i);
		t.set(array_size[i]);
		const double cycles = t.run(num_trials);
		data[i*n  ] = cycles;
		data[i*n+1] = cycles/t.n();
		data[i*n+2] = cycles/t.nlgn();
		data[i*n+3] = cycles/t.nlgn2();
	}

	Table table;
	table.dim(m,n);
	table.rows(rows);
	table.cols(cols);
	table.data(data);
	char banner[BUFSIZ];
	sprintf(banner,"Measure the speed of finding the closest pair of points\n");
	strcat(banner,"in terms of CPU cycles. Observe the scaling is nlgn.");
	table.print(banner);
	return 0;
}
